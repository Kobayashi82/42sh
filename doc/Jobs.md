# Jobs

## Conceptos Fundamentales

### ¿Qué es un Job?

Un **job** es una unidad de ejecución completa desde la perspectiva del shell. Puede ser:
- Un comando simple: `ls`
- Un pipeline completo: `ls | grep foo | wc -l`
- Un subshell: `(cmd1; cmd2; cmd3)`

**Punto clave**: Un pipeline es UN solo job, aunque tenga múltiples procesos. El shell los trata como una unidad.

### ¿Qué NO es un Job único?

Los operadores de secuencia crean jobs **separados**:
- `cmd1 ; cmd2` → DOS jobs (ejecuta cmd1, espera, luego cmd2)
- `cmd1 && cmd2` → DOS jobs (cmd2 solo si cmd1 tuvo éxito)
- `cmd1 || cmd2` → DOS jobs (cmd2 solo si cmd1 falló)

### Process Group ID (PGID)

Todos los procesos de un job deben estar en el **mismo grupo de procesos**. Esto permite:
- Enviar señales a todo el job de una vez con `killpg()`
- Que Ctrl-C mate todo el pipeline, no solo un proceso
- Que Ctrl-Z suspenda todo el job como unidad
- Controlar quién tiene acceso al terminal

El primer proceso del job se convierte en el **líder del grupo**, y su PID se usa como PGID para todo el grupo.

## Estructura de Datos

### Estructura Recomendada

```c
typedef struct s_process {
    pid_t pid;
    int status;
    char *command;
    struct s_process *next;
} t_process;

typedef struct s_job {
    int job_id;              // [1], [2], etc.
    pid_t pgid;              // Process Group ID
    char *command_line;      // Para mostrar en 'jobs'
    t_process *processes;    // Lista de procesos del job
    int is_background;       // En background o foreground
    int is_stopped;          // Recibió SIGSTOP (Ctrl-Z)
    int is_completed;        // Todos los procesos terminaron
    struct s_job *next;
} t_job;

typedef struct s_job_control {
    t_job *jobs;             // Lista enlazada de jobs
    int next_job_id;         // Siguiente ID disponible
    pid_t shell_pgid;        // PGID del shell
    int shell_terminal;      // FD del terminal (STDIN_FILENO)
} t_job_control;
```

### ¿Por qué esta estructura?

**Job vs Proceso**: Si ejecutas `ls | grep foo | wc`, eso es UN job con tres procesos. Necesitas la lista de procesos para saber cuándo TODO el pipeline ha terminado.

**PGID separado**: No puedes usar solo PIDs porque necesitas enviar señales a todo el grupo a la vez.

**Estado del job**: Un job puede estar running, stopped, o completed. Necesitas trackear esto para comandos como `jobs`, `fg`, `bg`.

## Creación de Jobs y PGID

### El Problema de la Race Condition

Cuando haces fork, no sabes quién ejecuta primero (padre o hijo). Si solo el hijo hace `setpgid()`, el padre podría intentar operaciones sobre el grupo antes de que exista.

### La Solución: Llamar setpgid() Dos Veces

Se hace en **ambos sitios** (padre e hijo). El kernel ignora la redundancia si ya está hecho:

**En el primer proceso del pipeline:**
```c
// HIJO
setpgid(0, 0);  // Me hago líder de mi grupo

// PADRE
setpgid(first_pid, first_pid);  // Redundante pero seguro
pgid = first_pid;
```

**En procesos siguientes del pipeline:**
```c
// HIJO
setpgid(0, pgid);  // Me uno al grupo del primero

// PADRE
setpgid(second_pid, pgid);  // También desde el padre
```

### Ventajas

- No importa quién ejecute primero
- El padre mantiene control del PGID
- `setpgid()` es idempotente (llamarlo dos veces con el mismo valor no causa error)

## Control del Terminal

### ¿Qué es el Foreground Process Group?

En Unix, solo UN process group puede controlar el terminal en un momento dado. Este grupo:
- Recibe señales del teclado (Ctrl-C → SIGINT, Ctrl-Z → SIGTSTP)
- Puede leer del terminal sin bloquearse
- Puede escribir al terminal sin ser suspendido

### Dar el Terminal a un Job

Se hace **UNA sola vez** al process group completo, no a procesos individuales:

```c
// Después de crear TODOS los procesos del pipeline
tcsetpgrp(shell_terminal, pgid);  // Le doy el terminal al grupo

// Espero a que el job termine
waitpid(...);

// Recupero el terminal para el shell
tcsetpgrp(shell_terminal, shell_pgid);
```

### Ejemplo con Pipeline

Para `ls | grep | wc`:
1. Creas los tres procesos, todos con el mismo PGID
2. Llamas `tcsetpgrp()` UNA vez con ese PGID
3. Los tres procesos automáticamente tienen acceso al terminal
4. Ctrl-C mata a los tres porque están en el foreground group

## Pipelines vs Secuencias

### Pipeline: `cmd1 | cmd2 | cmd3`

- **UN job**, UN process group
- Los tres procesos se ejecutan **simultáneamente**
- Conectados por pipes
- Comparten el mismo PGID
- Se controlan como unidad

### Secuencia: `cmd1 ; cmd2`

- **DOS jobs separados**
- Primero ejecutas cmd1 completamente
- Esperas a que termine
- Luego ejecutas cmd2
- Cada uno tiene su propio PGID

### Con Background

`cmd1 | cmd2 &` → Un job (el pipeline completo) en background

`cmd1 ; cmd2 &` → Dos jobs: cmd1 en foreground, cmd2 en background

## Subshells

### ¿Qué es un Subshell?

Un subshell `(comandos)` es un fork del shell que ejecuta comandos en un contexto aislado. Cambios dentro del subshell no afectan al shell padre.

### Subshell como Job

Desde la perspectiva del shell padre, el subshell es **un solo proceso**:

```
Shell padre ve:
[1] (sleep 5; echo done) &
    └─ Un proceso: el subshell
```

Dentro del subshell pueden ocurrir múltiples comandos, pipelines, etc., pero el padre solo trackea el proceso subshell.

### Subshell en Background

`(comandos) &` es perfectamente válido y común:
- Todo el subshell se ejecuta en background
- El shell principal no espera
- El subshell es tratado como un job normal

### Contexto Limpio en Subshells

Cuando haces fork para crear el subshell, **heredas copias** de:
- File descriptors
- Variables de entorno
- Working directory
- Toda la memoria del proceso padre (incluyendo estructuras de datos)

Pero debes **resetear/limpiar**:

1. **Job Control**: Crear una nueva instancia de `t_job_control` para el subshell
2. **Señales**: Restaurar a comportamiento default (el shell padre probablemente las tiene ignoradas)
3. **PGID**: Crear nuevo grupo con `setpgid(0, 0)`
4. **Control del terminal**: El subshell generalmente NO debe hacer `tcsetpgrp()`

### Comandos en Background Dentro de Subshells

`(cmd1 & cmd2 &) &`

Aquí tienes **tres niveles**:
- El shell principal ve el subshell como un job
- El subshell ve cmd1 y cmd2 como jobs internos (si implementas job control completo)
- Son independientes

**Recomendación**: Para 42sh, el nivel básico es suficiente. El subshell ejecuta comandos con `&` haciendo fork sin esperar. Los procesos quedan como huérfanos del sistema. No necesitas job control completo dentro de subshells a menos que el subject lo pida explícitamente.

## Command Substitution vs Subshell

### Command Substitution: `$(cmd)`

- **Propósito**: Capturar el output
- **Background `&`**: No tiene mucho sentido porque quieres esperar el resultado para capturarlo
- **Recomendación**: Ignora el `&` dentro de `$()` o da error

### Subshell: `(cmd)`

- **Propósito**: Agrupar comandos en contexto aislado
- **Background `&`**: Muy común y útil
- **Uso típico**: `(cmd1; cmd2; cmd3) &` para ejecutar secuencia en background

## Operaciones Esenciales

### Crear Job
Cuando ejecutas un comando o pipeline, creas una entrada en la lista de jobs con toda la información relevante.

### Añadir a Foreground/Background
- **Foreground**: Das el terminal al job con `tcsetpgrp()` y esperas
- **Background**: No das el terminal, no esperas, continúas el prompt

### Wait por Job
Usas `waitpid()` para esperar a que los procesos terminen. Importante usar `WNOHANG` para polling sin bloquear cuando checkeas jobs en background.

### Actualizar Estado
Periódicamente (o cuando recibes SIGCHLD) checkeas el estado de los jobs para ver si terminaron o se detuvieron.

### Limpiar Jobs Completados
Eliminas de la lista los jobs que ya terminaron completamente.

### Listar Jobs
Para el comando `jobs`, recorres la lista y muestras información de cada job activo.

## Señales y Job Control

### Señales Clave

- **SIGINT** (Ctrl-C): Interrumpe el foreground job
- **SIGTSTP** (Ctrl-Z): Suspende el foreground job
- **SIGCONT**: Resume un job suspendido
- **SIGCHLD**: Notifica al padre que un hijo cambió de estado

### Manejo en el Shell

El shell principal debe:
- **Ignorar** SIGINT y SIGTSTP para no morir con Ctrl-C o Ctrl-Z
- **Capturar** SIGCHLD para actualizar estado de jobs
- Los jobs ejecutados deben tener señales en modo **default**

### Manejo en Subshells

Los subshells deben **restaurar** todas las señales a comportamiento default porque son contextos independientes.

## Flujo Completo de Ejecución

### Comando Simple en Foreground

1. Parser identifica comando
2. Fork proceso
3. Hijo hace `setpgid(0, 0)` y exec
4. Padre hace `setpgid(child_pid, child_pid)`
5. Padre da terminal: `tcsetpgrp(terminal, child_pid)`
6. Padre espera: `waitpid(child_pid, ...)`
7. Padre recupera terminal: `tcsetpgrp(terminal, shell_pgid)`

### Pipeline en Background

1. Parser identifica pipeline
2. Creas los tres procesos, todos con mismo PGID
3. **NO** haces `tcsetpgrp()` (es background)
4. **NO** esperas con waitpid
5. Añades job a la lista
6. Imprimes `[job_id] pgid`
7. Continúas con el prompt

### Subshell en Background

1. Parser identifica subshell
2. Fork para el subshell
3. Hijo crea nuevo job_control, resetea señales
4. Hijo parsea y ejecuta comandos internos
5. Hijo sale con exit status
6. Padre trackea el subshell como un job simple (un solo proceso)
7. Padre no espera, continúa

## Consideraciones Importantes

### Un Job Está Completo Cuando...
TODOS sus procesos han terminado. No basta con que uno termine, tienes que esperar a todos los del pipeline.

### PGID es Fundamental
Sin él, no puedes controlar el job como unidad. Ctrl-C solo mataría un proceso del pipeline, dejando los otros huérfanos.

### El Terminal es Recurso Único
Solo un process group puede controlarlo. Por eso foreground y background jobs se comportan diferente respecto a señales del teclado.

### Subshells son Independientes
Aunque heredan memoria del padre (es un fork), operan en contexto aislado. Cambios internos no afectan al padre.

### Background Jobs y Huérfanos
Si un subshell en background lanza comandos con `&` y termina sin esperarlos, esos procesos quedan huérfanos y son adoptados por init/systemd. Esto es normal y aceptable en implementaciones básicas.

## Resumen Visual

```
Shell Principal (42sh)
│
├─ Job 1: ls | grep | wc        [Foreground]
│   ├─ Proceso: ls    (PID 100, PGID 100)
│   ├─ Proceso: grep  (PID 101, PGID 100)
│   └─ Proceso: wc    (PID 102, PGID 100)
│
├─ Job 2: sleep 10 &            [Background]
│   └─ Proceso: sleep (PID 200, PGID 200)
│
└─ Job 3: (cmd1; cmd2) &        [Background, Subshell]
    └─ Proceso: subshell (PID 300, PGID 300)
        │
        └─ Dentro del subshell (invisible para el padre):
            ├─ cmd1 ejecutándose
            └─ cmd2 esperando
```

## Conclusión

El control de jobs requiere:
1. **Estructuras** que representen jobs y procesos
2. **PGID** para controlar grupos como unidad
3. **Control del terminal** para foreground/background
4. **Gestión de señales** apropiada
5. **Contextos limpios** en subshells

La clave está en entender que un job puede tener múltiples procesos, pero todos se controlan como una unidad mediante el process group ID.
