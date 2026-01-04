# FC (Fix Command) - Especificación de Implementación

## Orden de Procesamiento de Opciones

### 1. Parsear todas las opciones
Leer y almacenar: `-e`, `-l`, `-n`, `-r`, `-s`

### 2. Caso especial: `-e -`
Si `-e` tiene como argumento `-` (un solo guión), tratarlo como `-s` (modo sustitución).

### 3. Determinar el modo de operación

**Prioridad de modos:**

1. **Si hay `-s` (o `-e -`)** → Modo sustitución
   - Ignora todas las demás opciones (`-l`, `-n`, `-r`, `-e`)
   
2. **Si hay `-l`** → Modo lista
   - Respeta `-n` (omitir números) y `-r` (orden inverso)
   - Ignora `-e`
   
3. **Si hay `-e EDITOR`** → Modo edición con editor especificado
   - Ignora `-n` y `-r`
   
4. **Si no hay opciones** → Modo edición con editor por defecto
   - Ignora `-n` y `-r`

### 4. Ejecutar según el modo seleccionado

---

## Requisitos POSIX para `fc`

### Opciones Obligatorias

- `-e editor` - Especificar editor para edición
- `-l` - Listar comandos en lugar de editar
- `-n` - Omitir números de línea (solo con `-l`)
- `-r` - Invertir orden (solo con `-l`)
- `-s` - Sustituir y ejecutar

### Modo Sustitución (`-s`)

**Sintaxis:**
```bash
fc -s [pat=rep ...] [command]
```

**Comportamiento:**
- Acepta múltiples sustituciones `pat=rep`
- Acepta **exactamente UN** comando (número, offset o string)
- **ERROR si hay más de un argumento no-sustitución** (POSIX estricto)
- Ejecuta inmediatamente después de sustituir
- Imprime el comando modificado antes de ejecutarlo
- El comando modificado se añade al historial
- El comando `fc` mismo NO se añade al historial

**Ejemplo:**
```bash
fc -s a=b echo        # Válido: sustituye y ejecuta
fc -s a=b b=c echo    # Válido: múltiples sustituciones
fc -s 1 10            # ERROR: demasiados argumentos
```

### Modo Lista (`-l`)

**Sintaxis:**
```bash
fc -l [-n] [-r] [first] [last]
```

**Comportamiento:**
- Muestra comandos del historial con números de evento
- `-n` omite los números
- `-r` invierte el orden de visualización
- Formato: `número<TAB>comando` (o espacios, no es crítico)
- Rango por defecto: últimos 16 comandos
- **Si `first > last`**: se intercambian automáticamente

### Modo Edición (`-e` o sin opciones)

**Sintaxis:**
```bash
fc [-e editor] [first] [last]
fc [first] [last]
```

**Comportamiento:**
- Edita comandos en archivo temporal
- Ejecuta los comandos del archivo después de editar
- Los comandos editados se añaden al historial
- El comando `fc` mismo NO se añade al historial cuando se edita
- **Si `first > last`**: se intercambian automáticamente (NO invierte orden en editor)

### Selección de Editor (por prioridad)

1. `-e EDITOR` (si se especifica en la línea de comandos)
2. `$FCEDIT` (variable de entorno)
3. `$EDITOR` (variable de entorno)
4. `$VISUAL` (variable de entorno) - **Extensión opcional**
5. `/usr/bin/editor` (enlace simbólico del sistema) - **Extensión opcional**
6. `nano` (editor amigable) - **Extensión opcional**
7. `ed` (editor POSIX por defecto)

**Nota:** POSIX solo requiere buscar `$FCEDIT`, `$EDITOR` y luego `ed`. Los pasos 4-6 son extensiones opcionales para mejor experiencia de usuario.

### Especificación de Comandos

#### Números de Evento

- **Número positivo sin signo** (ej: `500`): Número de evento absoluto
- **Número negativo** (ej: `-5`): Offset desde el último comando
  - `-1` = último comando
  - `-2` = penúltimo comando
- **Cero** (`0`): 
  - Con `-l`: desde el comando 0 hasta el último
  - Con edición: inválido (error)

#### Strings

- Busca el comando más reciente que **comience** con ese string
- La búsqueda es desde el último comando hacia atrás
- Si no se encuentra: error "no command found"

#### Rangos

**Con `-l` (modo lista):**
- `[first]` solo: desde `first` hasta el último comando
- `[first] [last]`: rango inclusivo
- **Si `first > last`**: se intercambian automáticamente
- Sin argumentos: últimos 16 comandos

**Con edición (con o sin `-e`):**
- `[first]` solo: edita solo ese comando
- `[first] [last]`: edita el rango de comandos
- **Si `first > last`**: se intercambian automáticamente (orden cronológico en el editor)
- Sin argumentos: edita el último comando

### Validaciones Requeridas

1. **Con `-s`**: Error si hay más de un argumento que no sea `pat=rep`
2. **Archivo vacío tras edición**: Error, no ejecutar nada
3. **Comando no encontrado**: Error "no command found"
4. **Número de evento fuera de rango**: Usar límites del historial (0 o último)

### Códigos de Salida

- `0` - Éxito
- `>0` - Error (sintaxis, comando no encontrado, etc.)
- Si ejecuta comandos: retorna el código de salida del último comando ejecutado

### Comportamiento del Historial

1. **Al listar (`-l`)**: El comando `fc` SÍ se añade al historial
2. **Al editar o sustituir**: El comando `fc` NO se añade al historial
3. **Tras editar**: Los comandos editados SÍ se añaden al historial
4. **Tras sustituir**: El comando modificado SÍ se añade al historial

---

## Manejo de Rangos `first > last`

### Comportamiento según bash:

Del código fuente de bash (líneas 383-391):
```c
if (histend < histbeg)
{
  i = histend;
  histend = histbeg;
  histbeg = i;
  
  reverse = 1;  // Activa reverse implícitamente
}
```

### Implementación recomendada:

**Modo lista (`-l`):**
```bash
fc -l 10 5     # Se intercambian: muestra del 5 al 10
fc -l -r 10 5  # Se intercambian: muestra del 10 al 5 (inverso)
```
- Intercambiar `first` y `last`
- Respetar el flag `-r` explícito si está presente
- Bash activa `-r` implícitamente, pero no es necesario hacerlo

**Modo edición:**
```bash
fc 10 5        # Edita del 5 al 10 (orden: 5, 6, 7, 8, 9, 10)
fc -e vi 10 5  # Mismo comportamiento
```
- Intercambiar `first` y `last`
- Mostrar en el editor en orden cronológico (NO invertir)

---

## Notas sobre `history`

El comando `history` **NO es POSIX**. Puedes implementarlo con las opciones que desees sin restricciones de estándar.

Opciones comunes en bash (todas opcionales):
- `-c` - Limpiar historial
- `-d offset` - Borrar entrada
- `-a` - Añadir a archivo
- `-n` - Leer del archivo
- `-r` - Leer del archivo (reemplazar)
- `-w` - Escribir a archivo
- `-p` - Expansión sin ejecutar
- `-s` - Añadir al historial

Variables de entorno relacionadas (ninguna es POSIX):
- `HISTFILE` - Archivo donde se guarda el historial
- `HISTSIZE` - Número máximo de comandos en memoria
- `HISTFILESIZE` - Número máximo de líneas en el archivo
- `HISTTIMEFORMAT` - Formato de tiempo para mostrar

---

## Resumen de Diferencias con Bash

### Tu implementación (POSIX por defecto):
- **Siempre** valida un solo comando con `-s` (error si hay más)
- Selección de editor incluye `$VISUAL`, `/usr/bin/editor` y `nano` antes de `ed`
- Comportamiento POSIX activo por defecto (sin necesidad de `set -o posix`)

### Bash (modo no-POSIX):
- Ignora argumentos extra con `-s` (solo en modo no-POSIX)
- Solo busca `$FCEDIT`, `$EDITOR`, luego `vi` (no `ed` por defecto en bash)
- Requiere `set -o posix` para validación estricta
- Activa `-r` implícitamente cuando `first > last` en modo lista

---

## Casos de Prueba Importantes

```bash
# Modo sustitución
fc -s                      # Re-ejecuta último comando
fc -s echo                 # Re-ejecuta último "echo"
fc -s a=b echo             # Sustituye a por b y ejecuta
fc -s a=b b=c c=d          # Múltiples sustituciones en cadena
fc -s 1 2                  # ERROR: demasiados argumentos

# Modo lista
fc -l                      # Últimos 16 comandos
fc -l 5                    # Del 5 al último
fc -l 5 10                 # Del 5 al 10
fc -l 10 5                 # Del 5 al 10 (intercambiados)
fc -l -r 10 5              # Del 10 al 5 (inverso)
fc -l -n                   # Sin números de línea
fc -l -nr 1 10             # Sin números, orden inverso

# Modo edición
fc                         # Edita último comando
fc 5                       # Edita comando 5
fc 5 10                    # Edita del 5 al 10
fc 10 5                    # Edita del 5 al 10 (intercambiados)
fc -e vi                   # Edita último con vi
fc -e nano 5 10            # Edita rango con nano

# Casos especiales
fc -e -                    # Equivalente a fc -s
fc -e - a=b                # Sustitución con -e -
fc -sl                     # Modo -s (ignora -l)
fc -ls                     # Modo -s (ignora -l)
```