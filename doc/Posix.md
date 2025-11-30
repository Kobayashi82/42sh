# Guía Completa para Implementar un Shell POSIX

Esta guía cubre TODO lo necesario según la especificación POSIX.1-2017 (IEEE Std 1003.1-2017) para implementar un shell completamente conforme.

## 1. ARQUITECTURA GENERAL DEL SHELL

### 1.1 Flujo de Operación
1. **Lectura de entrada**: desde archivo, `-c`, stdin, o funciones del sistema
2. **Tokenización**: dividir entrada en palabras y operadores
3. **Parsing**: construir comandos simples y compuestos
4. **Expansiones**: realizar todas las expansiones en orden específico
5. **Redirecciones**: aplicar redirecciones y eliminar operadores
6. **Ejecución**: ejecutar función, built-in, ejecutable o script
7. **Espera**: opcionalmente esperar completación y recoger exit status

### 1.2 Entorno de Ejecución del Shell
Debe mantener:
- Archivos abiertos heredados al invocar + los controlados por `exec`
- Directorio de trabajo actual (por `cd`)
- Máscara de creación de archivos (por `umask`)
- Traps actuales (por `trap`)
- Parámetros del shell establecidos por asignación de variables
- Funciones del shell
- Opciones activadas al inicio o por `set`
- PIDs de últimos comandos en listas asíncronas
- Aliases del shell

## 2. TOKENIZACIÓN Y RECONOCIMIENTO DE TOKENS

### 2.1 Caracteres Especiales que Requieren Quoting
```
| & ; < > ( ) $ ` \ " ' <space> <tab> <newline>
```

### 2.2 Caracteres que Pueden Requerir Quoting
```
* ? [ # = %
```

### 2.3 Reglas de Tokenización (en orden de aplicación)
1. Si es fin de entrada → delimitar token actual
2. Si el carácter previo forma un operador y el actual puede extenderlo → agregarlo al operador
3. Si el carácter previo forma un operador y el actual NO puede extenderlo → delimitar operador
4. Si es `\`, `'`, o `"` sin quote → afecta quoting hasta el fin del texto quotado
5. Si es `$` o `` ` `` sin quote → identificar expansiones de parámetros, sustitución de comandos, o expansión aritmética
6. Si puede iniciar un nuevo operador → delimitar token actual y comenzar operador
7. Si es `<blank>` sin quote → delimitar token y descartar el blank
8. Si el carácter previo era parte de una palabra → agregar a esa palabra
9. Si es `#` → descartar como comentario hasta `<newline>`
10. Usar como inicio de nueva palabra

### 2.4 Operadores Reconocidos
**Operadores de control**: `&`, `&&`, `(`, `)`, `;`, `;;`, `|`, `||`, `<newline>`

**Operadores de redirección**: `<`, `>`, `>|`, `>>`, `<&`, `>&`, `<<`, `<<-`, `<>`

## 3. QUOTING

### 3.1 Escape Character (Backslash)
- Un `\` sin quote preserva el valor literal del siguiente carácter
- **Excepción**: `\<newline>` se interpreta como continuación de línea (se elimina completamente)

### 3.2 Single-Quotes
- Preserva el valor literal de TODOS los caracteres
- NO se puede incluir un `'` dentro de single-quotes

### 3.3 Double-Quotes  
- Preserva el valor literal EXCEPTO: `` ` ``, `$`, `\`, y ocasionalmente `!`
- Dentro de double-quotes funcionan: `$()`, `` `...` ``, `${}`, `$var`
- El `\` solo escapa: `$`, `` ` ``, `"`, `\`, `<newline>`
- El parámetro especial `@` tiene comportamiento especial en double-quotes

## 4. PALABRAS RESERVADAS

### 4.1 Palabras Reservadas Obligatorias
```
!  case  do  done  elif  else  esac  fi  for  if  in  then  until  while  {  }
```

### 4.2 Palabras Reservadas Opcionales (comportamiento no especificado)
```
[[  ]]  function  select
```

### 4.3 Nombres Reservados
- Concatenación de `name` + `:` (como `label:`) está reservada

### 4.4 Reconocimiento de Palabras Reservadas
Se reconocen SOLO cuando:
- Ningún carácter está entre quotes
- Es la primera palabra de un comando
- Es la primera palabra después de: `!`, `case`, `do`, `elif`, `else`, `for`, `if`, `until`, `while`, `{`
- Es la tercera palabra en un comando `case` (solo `in` es válido)
- Es la tercera palabra en un comando `for` (solo `in` y `do` son válidos)

## 5. PARÁMETROS Y VARIABLES

### 5.1 Tipos de Parámetros
- **Parámetro**: nombre, número, o carácter especial
- **Variable**: parámetro denotado por un nombre
- **Parámetro posicional**: número decimal ≥ 1 (0 también existe pero es especial)
- **Parámetros especiales**: `@`, `*`, `#`, `?`, `-`, `$`, `!`, `0`

### 5.2 Parámetros Especiales

| Parámetro | Significado |
|-----------|-------------|
| `@` | Todos los parámetros posicionales (como campos separados) |
| `*` | Todos los parámetros posicionales (como un solo campo) |
| `#` | Número de parámetros posicionales |
| `?` | Exit status del último comando |
| `-` | Flags actuales del shell |
| `$` | PID del shell actual |
| `!` | PID del último comando background |
| `0` | Nombre del shell o script |

### 5.3 Variables del Shell que DEBEN Afectar la Ejecución

| Variable | Descripción |
|----------|-------------|
| `HOME` | Directorio home del usuario |
| `IFS` | Internal Field Separator (por defecto: `<space><tab><newline>`) |
| `LANG` | Locale por defecto |
| `LC_*` | Variables de locale específicas |
| `LINENO` | Número de línea actual en script/función |
| `PATH` | Ruta de búsqueda de comandos |
| `PPID` | PID del proceso padre |
| `PS1` | Prompt primario (interactivo) |
| `PS2` | Prompt secundario (interactivo) |
| `PS4` | Prompt para `set -x` |
| `PWD` | Directorio de trabajo actual |

### 5.4 Variable ENV
- Solo para shells interactivos
- Se expande al iniciar y el resultado se usa como archivo de comandos a ejecutar
- Se ignora si los UID/GID reales y efectivos son diferentes

## 6. EXPANSIONES (en orden estricto)

### 6.1 Orden de Expansiones
1. **Tilde expansion**
2. **Parameter expansion**  
3. **Command substitution**
4. **Arithmetic expansion**
5. **Field splitting** (si no está en double-quotes)
6. **Pathname expansion** (si `set -f` no está activo)
7. **Quote removal** (siempre al final)

### 6.2 Tilde Expansion
- `~` → `$HOME`
- `~user` → directorio home de `user`
- En asignaciones: después de `=` y después de cada `:` sin quote

### 6.3 Parameter Expansion

#### Formas básicas:
```bash
$parameter
${parameter}
```

#### Formas con valores por defecto:
```bash
${parameter:-word}    # Si unset o null, substituir word
${parameter-word}     # Si unset, substituir word  
${parameter:=word}    # Si unset o null, asignar word
${parameter=word}     # Si unset, asignar word
${parameter:?word}    # Si unset o null, error
${parameter?word}     # Si unset, error
${parameter:+word}    # Si set y non-null, substituir word
${parameter+word}     # Si set, substituir word
```

#### Operaciones de substring:
```bash
${#parameter}           # Longitud
${parameter%word}       # Eliminar sufijo más corto
${parameter%%word}      # Eliminar sufijo más largo
${parameter#word}       # Eliminar prefijo más corto
${parameter##word}      # Eliminar prefijo más largo
```

### 6.4 Command Substitution
```bash
$(command)      # Forma preferida
`command`       # Forma antigua (backquoted)
```

**Reglas importantes**:
- Se ejecuta en subshell
- Se eliminan `<newline>` al final de la salida
- Si contiene null bytes, el comportamiento es no especificado
- No se procesa para más expansiones (excepto las internas al comando)

### 6.5 Arithmetic Expansion
```bash
$((expression))
```

**Características**:
- Solo aritmética de enteros con signo (mínimo: signed long)
- Soporta constantes: decimal, octal (0...), hexadecimal (0x...)
- **NO requiere**: `sizeof()`, `++`, `--` (prefix/postfix)
- **NO soporta**: sentencias de selección, iteración, jump
- Los cambios a variables persisten después de la expansión

**Operadores soportados**:
```
() ++ -- (unary)+ (unary)- ~ !
* / %
+ -
<< >>
< <= > >=
== !=
&
^
|
&&
||
? : (ternary)
= += -= *= /= %= <<= >>= &= ^= |=
```

### 6.6 Field Splitting
- Aplica a resultados de expansiones que NO están en double-quotes
- Usa `IFS` como delimitadores
- Si `IFS` es null → no hay field splitting
- Si `IFS` está unset o es `<space><tab><newline>` → comportamiento especial de whitespace

### 6.7 Pathname Expansion (Globbing)

#### Patrones:
- `*` → cualquier string (incluyendo null)
- `?` → cualquier carácter único
- `[...]` → bracket expression (como en regex)

#### Reglas especiales para nombres de archivo:
- `/` debe ser explícitamente matched (no por `*` o `?`)
- `.` al inicio debe ser explícitamente matched (no por `*`, `?`, o `[!...]`)
- Si el patrón no coincide con nada → se deja sin cambios

## 7. REDIRECCIONES

### 7.1 Formato General
```
[n]operador word
```

### 7.2 File Descriptors Estándar
- 0: stdin
- 1: stdout  
- 2: stderr
- Mínimo requerido: 0-9

### 7.3 Tipos de Redirección

#### Input Redirection:
```bash
[n]<word         # Abrir word para lectura en fd n (o stdin)
```

#### Output Redirection:
```bash
[n]>word         # Truncar/crear word para escritura
[n]>|word        # Igual, ignorando noclobber
[n]>>word        # Append a word
```

#### Duplicación de File Descriptors:
```bash
[n]<&word        # Duplicar fd de entrada o cerrar
[n]>&word        # Duplicar fd de salida o cerrar
[n]<&-           # Cerrar fd n
[n]>&-           # Cerrar fd n
```

#### Here-Documents:
```bash
[n]<<delimiter
contenido
delimiter

[n]<<-delimiter  # Ignora tabs al inicio de líneas
contenido
	delimiter
```

**Reglas de here-documents**:
- Si `delimiter` tiene quotes → NO se expande el contenido
- Si `delimiter` NO tiene quotes → se expanden parámetros, comandos y aritmética
- Los `"` dentro del here-document NO son especiales

#### Input/Output:
```bash
[n]<>word        # Abrir word para lectura Y escritura
```

## 8. TIPOS DE COMANDOS

### 8.1 Simple Commands
```
[var=value ...] [redirections ...] [command [arguments ...]] [redirections ...]
```

**Orden de evaluación**:
1. Identificar asignaciones y redirecciones
2. Expandir palabras restantes
3. Realizar redirecciones
4. Expandir asignaciones de variables
5. Ejecutar comando (si hay)

### 8.2 Pipelines
```bash
[!] command1 [| command2 ...]
```

- El stdout de cada comando se conecta al stdin del siguiente
- Exit status: el del último comando (o NOT del último si hay `!`)

### 8.3 Lists

#### Sequential (;):
```bash
command1 ; command2
```

#### Asynchronous (&):
```bash
command1 & command2
```
- Sin job control: stdin de comando async → `/dev/null`
- Exit status: siempre 0

#### AND (&&):
```bash
command1 && command2
```
- `command2` solo se ejecuta si `command1` retorna 0

#### OR (||):
```bash
command1 || command2  
```
- `command2` solo se ejecuta si `command1` retorna no-0

### 8.4 Compound Commands

#### Grouping:
```bash
(commands)           # En subshell
{ commands; }        # En shell actual (requiere ; antes de })
```

#### Loops:
```bash
for name [in words...]; do commands; done
while commands1; do commands2; done
until commands1; do commands2; done
```

#### Conditionals:
```bash
if commands1; then commands2; [elif commands3; then commands4;]... [else commands5;] fi

case word in
  pattern1) commands1 ;;
  pattern2 | pattern3) commands2 ;;
esac
```

### 8.5 Function Definitions
```bash
fname() compound-command [redirections]
```

**Reglas**:
- `fname` debe ser un nombre válido (no un special built-in)
- Las expansiones NO ocurren en la definición
- Los parámetros posicionales se reemplazan temporalmente al llamar
- `return` sale de la función

## 9. BÚSQUEDA Y EJECUCIÓN DE COMANDOS

### 9.1 Orden de Búsqueda (si el nombre NO contiene `/`)

1. **Special built-ins** (tienen prioridad absoluta)
2. **Functions** (definidas en el shell)
3. **Regular built-ins** (comandos internos normales)
4. **Búsqueda en PATH** (ejecutables externos)

### 9.2 Si el Nombre Contiene `/`
- Se ejecuta directamente como pathname
- No se busca en PATH

### 9.3 Tabla de Utilidades Ambiguas
Estas utilidades pueden causar resultados no especificados:
```
alias   bg      cd      command   fc      fg      getopts
jobs    kill    newgrp  read      type    ulimit  umask   unalias   wait
```

### 9.4 Regular Built-ins que DEBEN Poder Invocarse
Estas utilidades deben estar disponibles como programas externos:
```
command  sh  true  false
```

Y opcionalmente (User Portability Utilities):
```
type  ulimit
```

## 10. SPECIAL BUILT-IN UTILITIES (14 obligatorios)

### 10.1 Características Especiales
1. Error de sintaxis → puede causar que el shell aborte (non-interactive)
2. Asignaciones de variables precedentes persisten después de completar
3. Se buscan ANTES que las funciones

### 10.2 Lista Completa de Special Built-ins

#### 10.2.1 `break [n]`
- Sale del n-ésimo bucle `for`, `while`, o `until` envolvente
- Por defecto: n=1
- Exit status: 0 si exitoso, >0 si n no es entero válido

#### 10.2.2 `: [arguments...]`
- No hace nada, solo expande argumentos
- Útil para asignaciones de variables o redirecciones
- Exit status: siempre 0

#### 10.2.3 `continue [n]`
- Retorna al inicio del n-ésimo bucle envolvente
- Por defecto: n=1
- Exit status: 0 si exitoso, >0 si n no es entero válido

#### 10.2.4 `. file`
- Ejecuta comandos del archivo en el entorno actual
- Si `file` no contiene `/` → busca en PATH
- El archivo no necesita ser ejecutable
- Exit status: el del último comando ejecutado, o 0 si ninguno

#### 10.2.5 `eval [arguments...]`
- Concatena argumentos (separados por espacios)
- Lee y ejecuta el comando resultante
- Exit status: el del comando ejecutado, o 0 si no hay argumentos

#### 10.2.6 `exec [command [arguments...]]`
- Sin comando: realiza solo redirecciones en subshell
- Con comando: reemplaza el shell con el comando (sin crear proceso nuevo)
- Exit status: el del programa que reemplazó al shell, 127 si no encontrado, 126 si no ejecutable

#### 10.2.7 `exit [n]`
- Sale del shell con exit status n
- Por defecto: exit status del último comando
- Ejecuta trap EXIT antes de terminar
- Exit status: n si especificado (comportamiento no especificado si n > 255)

#### 10.2.8 `export [name[=word]...]` / `export -p`
- Da atributo export a variables
- `-p`: imprime todas las variables exportadas en formato reusable
- Exit status: 0 si éxito, >0 si algún nombre no pudo exportarse

#### 10.2.9 `readonly [name[=word]...]` / `readonly -p`
- Da atributo readonly a variables
- `-p`: imprime todas las variables readonly en formato reusable
- Variables readonly no pueden modificarse ni dejar de estar set
- Exit status: 0 si éxito, >0 si algún nombre no pudo hacerse readonly

#### 10.2.10 `return [n]`
- Sale de una función o script de `.`
- Exit status: n si especificado, o el del último comando
- Comportamiento no especificado si no está en función o `.` script

#### 10.2.11 `set [options] [arguments...]`
Opciones principales (con `-` activa, con `+` desactiva):

| Opción | Nombre largo | Descripción |
|--------|--------------|-------------|
| `-a` | `allexport` | Auto-export variables asignadas |
| `-b` | `notify` | Notificar jobs background inmediatamente |
| `-C` | `noclobber` | No sobreescribir archivos con `>` |
| `-e` | `errexit` | Salir inmediatamente si comando falla |
| `-f` | `noglob` | Deshabilitar pathname expansion |
| `-h` | | Hash utilities al definir funciones |
| `-m` | `monitor` | Job control (proceso por grupo) |
| `-n` | `noexec` | Leer pero no ejecutar (chequeo sintaxis) |
| `-u` | `nounset` | Error al expandir variables unset |
| `-v` | `verbose` | Imprimir input mientras se lee |
| `-x` | `xtrace` | Tracing de comandos al ejecutar |

Argumentos especiales:
- `set --` sin argumentos → unset todos los parámetros posicionales
- `set -- args...` → establecer parámetros posicionales
- `set` sin opciones ni args → imprimir todas las variables

#### 10.2.12 `shift [n]`
- Desplaza parámetros posicionales n posiciones (por defecto: 1)
- `$1` desaparece, `$2` → `$1`, etc.
- `$#` se decrementa en n
- Exit status: 0 si éxito, >0 si n > `$#`

#### 10.2.13 `times`
- Imprime tiempos de usuario y sistema del shell y sus hijos
- Formato: implementación específica
- Exit status: 0

#### 10.2.14 `trap [action condition...]`
- Establece acciones para señales/condiciones
- `trap` sin args → lista traps actuales
- `action` puede ser:
  - Comando a ejecutar
  - `-` (reset a defecto)
  - String vacío o vacío (ignorar señal)
- Condiciones pueden ser:
  - Número de señal
  - Nombre de señal (con o sin SIG)
  - `EXIT` (0) → al salir del shell
  - `ERR` → cuando comando retorna >0 (extensión)
- Exit status: 0 si éxito, >0 si condición inválida

#### 10.2.15 `unset [-v|-f] name...`
- `-v`: unset variable (por defecto)
- `-f`: unset función
- No se puede unset variables readonly o especiales (`*`, `@`, `#`, `?`, `-`, `$`, `!`, `0`)
- Exit status: 0 si éxito, >0 si algún name es readonly

## 11. REGULAR BUILT-INS RECOMENDADOS

Aunque POSIX no los define como special built-ins, típicamente se implementan como built-ins:

### 11.1 `cd [-L|-P] [directory]`
- Cambia directorio de trabajo
- `-L`: lógico (seguir symlinks)
- `-P`: físico (resolver symlinks)
- Sin args → `cd $HOME`
- Actualiza `PWD` y `OLDPWD`

### 11.2 `pwd [-L|-P]`
- Imprime directorio actual
- `-L`: lógico
- `-P`: físico

### 11.3 `read [-r] var...`
- Lee línea del stdin
- Asigna fields a variables
- `-r`: no tratar `\` como escape

### 11.4 `echo [string...]`
- Imprime argumentos a stdout
- Comportamiento de `\n`, `\t`, etc. es no portable

### 11.5 `test` / `[`
- Evalúa expresiones condicionales
- Ver POSIX para operadores completos

### 11.6 `getopts optstring name [args...]`
- Procesa opciones de comandos
- Actualiza `OPTIND` y `OPTARG`

### 11.7 `command [-p|-v|-V] command_name [arguments...]`
- Ejecuta comando suprimiendo búsqueda de funciones
- `-p`: usar PATH por defecto
- `-v`: mostrar pathname o tipo
- `-V`: descripción verbose

## 12. GRAMÁTICA FORMAL

### 12.1 Símbolos Léxicos
```
WORD ASSIGNMENT_WORD NAME NEWLINE IO_NUMBER
AND_IF OR_IF DSEMI                    # && || ;;
DLESS DGREAT LESSAND GREATAND        # << >> <& >&
LESSGREAT DLESSDASH CLOBBER          # <> <<- >|
If Then Else Elif Fi Do Done
Case Esac While Until For
Lbrace Rbrace Bang In               # { } ! in
```

### 12.2 Reglas de Token
**Regla 1**: Palabra reservada o WORD (depende del contexto)
**Regla 2**: Aplicar expansiones de redirección
**Regla 3**: Determinar delimiter de here-document
**Regla 4**: `esac` o WORD
**Regla 5**: NAME (si es nombre válido) o WORD
**Regla 6**: 
  - case: `in` o WORD
  - for: `in`, `do`, o WORD
**Regla 7a/7b**: Detectar ASSIGNMENT_WORD (contiene `=` no quoted)
**Regla 8**: Palabra reservada, NAME, o aplicar regla 7
**Regla 9**: No expandir en definición de función

### 12.3 Gramática BNF (resumida)
```
program               : linebreak complete_commands linebreak
complete_commands     : complete_command | complete_commands newline_list complete_command
complete_command      : list separator_op | list
list                  : and_or | list separator_op and_or
and_or                : pipeline | and_or AND_IF linebreak pipeline 
                      | and_or OR_IF linebreak pipeline
pipeline              : pipe_sequence | Bang pipe_sequence
pipe_sequence         : command | pipe_sequence '|' linebreak command
command               : simple_command | compound_command | function_definition
simple_command        : cmd_prefix cmd_word cmd_suffix | cmd_prefix | cmd_name cmd_suffix | cmd_name
compound_command      : brace_group | subshell | for_clause | case_clause 
                      | if_clause | while_clause | until_clause
```

## 13. MANEJO DE SEÑALES

### 13.1 Señales Heredadas
- Comandos heredan acciones de señal del shell
- **Excepción**: listas asíncronas con job control disabled heredan SIGINT y SIGQUIT ignorados

### 13.2 Traps y Señales
- Trap se ejecuta DESPUÉS de que comando foreground completa
- Si señal llega durante `wait` → `wait` retorna inmediatamente (>128)
- Orden de ejecución de múltiples traps pendientes: no especificado

### 13.3 Señales Importantes
- **SIGINT** (Ctrl+C): No debe terminar shell interactivo
- **SIGQUIT** (Ctrl+\): Similar a SIGINT
- **SIGCHLD**: Indica terminación de proceso hijo
- **SIGTERM**: Terminación
- **SIGTSTP** (Ctrl+Z): Suspender job (si hay job control)

## 14. EXIT STATUS

### 14.1 Valores Especiales
- **0**: Éxito
- **1-125**: Error del comando
- **126**: Comando no ejecutable
- **127**: Comando no encontrado
- **>128**: Comando terminado por señal (128 + número de señal)

### 14.2 Reglas de Exit Status
- Exit status de pipeline: el del último comando (o `! último` si hay `!`)
- Exit status de lista AND: el del último comando ejecutado
- Exit status de lista OR: el del último comando ejecutado
- Exit status de lista async: siempre 0
- Exit status de compound command: el de su último comando

## 15. PATTERN MATCHING

### 15.1 Caracteres Especiales
- `*`: Match cualquier string (incluyendo vacío)
- `?`: Match un carácter
- `[...]`: Bracket expression (como regex)

### 15.2 Reglas Especiales
- `\` escapa el siguiente carácter
- Dentro de bracket expression:
  - `[!...]`: NOT (caracteres que NO están en la lista)
  - `[a-z]`: rango
  - `[:class:]`: clases de caracteres (alnum, alpha, blank, digit, etc.)

### 15.3 Filename Expansion
- `/` NO es matched por `*`, `?`, o `[...]`
- `.` al inicio NO es matched por `*`, `?`, o `[!...]`
- Si no hay match → patrón se deja sin modificar

## 16. JOB CONTROL (Opcional: User Portability Utilities)

### 16.1 Requisitos
- Cada job corre en su propio process group
- Mensaje al completar background job
- Mensaje al suspender foreground job

### 16.2 Job Notation
- `%n`: Job número n
- `%%` o `%+`: Job actual
- `%-`: Job anterior
- `%string`: Job cuyo comando comienza con string
- `%?string`: Job cuyo comando contiene string

### 16.3 Comandos de Job Control
```bash
bg [job_id...]      # Continuar jobs en background
fg [job_id]         # Traer job a foreground
jobs [-l|-p] [job_id...]  # Listar jobs
```

## 17. MODO INTERACTIVO vs NO INTERACTIVO

### 17.1 Shell Interactivo
- Lee comandos de terminal
- Procesa archivo `ENV` al iniciar
- No aborta en errores de sintaxis de comandos normales
- Job control habilitado por defecto
- Manejo especial de `PS1`, `PS2`

### 17.2 Shell No Interactivo
- Lee de archivo o `-c`
- Aborta en errores de sintaxis
- Aborta en special built-in errors
- Aborta en redirection errors con special built-ins

## 18. SUBSHELLS

### 18.1 Cuándo se Crea un Subshell
- Command substitution: `$(...)` o `` `...` ``
- Pipeline: cada comando (la implementación puede ejecutar algunos en shell actual)
- Lista asíncrona: `command &`
- Agrupación con paréntesis: `(commands)`

### 18.2 Herencia de Subshell
- Duplica el entorno del shell padre
- **Excepción**: traps que no son ignored → reset a default
- Cambios NO afectan al shell padre

## 19. ALIAS (Opcional para scripts)

### 19.1 Características
- Sustitución ocurre durante la tokenización
- Solo en primera palabra de comando
- Si alias termina en blank → chequear siguiente palabra
- Definidos con `alias`, eliminados con `unalias`

### 19.2 Prevenir Loops Infinitos
- Si ya se está procesando alias del mismo nombre → no sustituir

## 20. EXPANSION ERRORS Y CONSECUENCIAS

### 20.1 Errores que Causan Exit (shell no interactivo)
- Error de sintaxis
- Error en special built-in
- Error de redirección con special built-in (o función que no es special built-in)
- Error de asignación de variable (escribir a readonly)
- Error de expansión (ej: `${x!y}`)

### 20.2 Shell Interactivo
- NO aborta en estos casos
- Escribe mensaje de error
- No ejecuta más el comando con error

## 21. CONSIDERACIONES DE IMPLEMENTACIÓN

### 21.1 Límites Mínimos
- File descriptors: 0-9 (más son opcionales)
- Líneas de entrada: longitud ilimitada
- Nombre de variables: al menos 8 caracteres portables
- Caracteres en pattern: no hay límite requerido
- Nesting de: `$()`, arithmetic, parameter expansion: al menos 1 nivel

### 21.2 Buffering
- El shell puede bufferear stdin
- Debe manejar correctamente scripts que alternan lectura de comandos y lectura por utilities

### 21.3 Localización
- Respetar `LANG`, `LC_*` variables
- Collating sequence para pathname expansion
- Mensajes en idioma apropiado

## 22. CONFORMANCIA POSIX

### 22.1 Para ser POSIX-compliant
1. Implementar TODOS los special built-ins
2. Respetar orden de búsqueda de comandos
3. Seguir reglas de expansión en orden correcto
4. Implementar gramática completa
5. Manejo correcto de señales y traps
6. Exit status según especificación
7. Respetar quoting rules
8. Pathname expansion según reglas
9. Redirecciones según especificación
10. Variables de entorno del shell

### 22.2 Extensiones Permitidas
- Adicionales built-ins (que no conflictúen)
- Operadores aritméticos adicionales
- Más file descriptors
- Aliases en modo interactivo
- Palabras reservadas adicionales (con precaución)
- Job control (recomendado)

### 22.3 No Permitido
- Cambiar comportamiento de special built-ins
- Ignorar orden de expansiones
- Cambiar sintaxis de palabras reservadas
- Modificar parsing de quoting

## 23. CASOS ESPECIALES Y CORNER CASES

### 23.1 Command Substitution vs Arithmetic
```bash
$((command))     # Arithmetic expansion (2 paréntesis)
$( (command) )   # Command subst con subshell (espacio requerido)
```

### 23.2 Asignaciones y Built-ins
```bash
VAR=value special_builtin  # VAR persiste después
VAR=value regular_builtin  # VAR NO persiste
VAR=value function         # NO especificado
```

### 23.3 Expansión en Word de Redirección
- Tilde expansion: SÍ
- Parameter expansion: SÍ
- Command substitution: SÍ
- Arithmetic expansion: SÍ
- Pathname expansion: NO (shell no interactivo) / Opcional (interactivo, solo si resulta en 1 word)
- Field splitting: NO

### 23.4 Parámetro `@` en Double-Quotes
```bash
"$@"    # Genera campos separados (excepto si $# = 0 → 0 campos)
"$*"    # Genera UN campo
```

### 23.5 Subshell en Función
- Variables locales: no especificadas en POSIX
- Exit en subshell de función: solo sale del subshell, no de la función

## 24. TESTING Y VALIDACIÓN

### 24.1 Casos de Prueba Esenciales
1. **Quoting**: todos los tipos, anidados, edge cases
2. **Expansiones**: orden, interacción, empty results
3. **Redirecciones**: todos los tipos, múltiples, errores
4. **Pipelines**: largos, con errores, con `!`
5. **Control flow**: loops anidados, break/continue con diferentes n
6. **Funciones**: recursión, redirecciones, traps
7. **Special built-ins**: comportamiento de error, persistencia de variables
8. **Exit status**: en todos los contextos
9. **Signals**: traps, comandos interrumpidos
10. **Field splitting e IFS**: todos los casos

### 24.2 Herramientas
- Suite de tests POSIX oficial (si disponible)
- Scripts de otros shells como casos de prueba
- Tests de conformancia de dash, bash --posix

## 25. REFERENCIAS

- **Especificación oficial**: POSIX.1-2017 (IEEE Std 1003.1-2017)
  - Shell & Utilities volume
  - Disponible en: https://pubs.opengroup.org/onlinepubs/9699919799/
  
- **Sección principal**: Chapter 2: Shell Command Language
  - URL: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html

- **Implementaciones de referencia**:
  - dash: shell minimalista POSIX-compliant
  - bash en modo POSIX: `bash --posix` o `set -o posix`
  - Shells certificados UNIX: AT&T ksh, etc.

---

## NOTAS FINALES

Esta guía cubre TODOS los aspectos requeridos por POSIX para implementar un shell conforme. Algunos detalles adicionales específicos pueden consultarse directamente en la especificación oficial, pero esta guía proporciona una base completa y estructurada para comenzar la implementación.

**Recuerda**: La conformidad POSIX es estricta. Cada detalle importa, especialmente:
- El orden de expansiones (no negociable)
- El comportamiento de special built-ins (diferente a regular)  
- Las reglas de quoting (complejas pero críticas)
- La gramática formal (define parsing exacto)
- Los exit status (cada contexto tiene sus reglas)

**Estrategia recomendada**: Implementa incrementalmente, empezando por comandos simples y añadiendo features progresivamente, testeando exhaustivamente cada parte antes de avanzar.