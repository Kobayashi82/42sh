# Notas

## AutoComplete

### Requisitos
- Command with char position
- Multichar column 0
- Guardar historial con `SIGHUP`, `SIGTERM`

### Componentes Necesarios
- Alias
- Variables
- Builtins
- Options

---

### Variables de Historial
- Si se usan `set` o `unset` con variables del historial, actualizar variables internas

### Guion como stdin/stdout
- El guion `-` indica stdin o stdout en comandos como `cat`
- En este proyecto se trata como texto normal. No creo que tenga que hacer nada.

---

## Constantes en C

### El valor es constante
```c
const int *ptr = &value;
// No puedes modificar *ptr, pero puedes cambiar ptr
```

### El puntero es constante
```c
int *const ptr = &value;
// Puedes modificar *ptr, pero ptr siempre apuntará al mismo lugar
```

### Ambos son constantes
```c
const int *const ptr = &value;
// Ni *ptr ni ptr pueden cambiar
```

---

## Shell Expansions (Orden)

1. Brace Expansion
2. Tilde Expansion
3. Shell Parameter Expansion
4. Command Substitution
5. Arithmetic Expansion
6. Process Substitution
7. Word Splitting
8. Filename Expansion
9. Pattern Matching
10. Quote Removal

---

## Simple Command Expansion

Cuando se ejecuta un comando simple, el shell realiza las siguientes operaciones en orden:

1. **Guarda asignaciones y redirecciones**
   - Las palabras marcadas como variable assignments (antes del comando)
   - Las redirecciones

2. **Expande las palabras** que no son assignments ni redirecciones
   - Si quedan palabras después de la expansión:
     - Primera palabra = nombre del comando
     - Palabras restantes = argumentos

3. **Realiza las redirecciones**

4. **Procesa las asignaciones de variables**
   - El texto después del `=` sufre:
     - Tilde expansion
     - Parameter expansion
     - Command substitution
     - Arithmetic expansion
     - Quote removal

5. **Manejo según resultado:**
   - **Sin nombre de comando**: las asignaciones afectan al shell actual
   - **Con nombre de comando**: las variables se añaden al entorno del comando ejecutado (no afectan al shell actual)
   - **Variable readonly**: error y exit con status no-zero

6. **Redirecciones sin comando**
   - Se realizan pero no afectan al shell actual
   - Error de redirección = exit con status no-zero

7. **Exit status**
   - Si hubo command substitution: exit status del último
   - Sin command substitution: exit status 0

---

## Quoting

### Comentarios
- `#` inicia un comentario
- No aplica si está escapada, entre comillas o junto a una palabra anterior

### Comillas Simples `'...'`
- Todo es literal
- **No se puede** poner una comilla simple dentro de comillas simples

### Comillas Dobles `"..."`
- `$`, `` ` ``, y `\` mantienen su función
- `!` solo mantiene función cuando POSIX está off
- `\` solo se aplica con: `$`, `` ` ``, `"`, `\`, y newline
  - En estos casos se elimina la `\`
  - Con `\newline` se quita la `\` y el salto de línea
- Expansión de `!` se elimina pero NO se elimina la `\`

### Sintaxis Especial

#### `$"string"`
```bash
$"hello, world"  # Se trata como "hello, world", eliminando el $
```

#### `$'string'` - Cadenas Literales con Escape
```bash
$'hello\nworld'  # Expande caracteres de escape
# Se convierte en: 'hello'$'\n''world'
```

**Caracteres de escape soportados:**
- `\a` - alert (bell)
- `\b` - backspace
- `\e`, `\E` - escape character (no ANSI C)
- `\f` - form feed
- `\n` - newline
- `\r` - carriage return
- `\t` - horizontal tab
- `\v` - vertical tab
- `\\` - backslash
- `\'` - single quote
- `\"` - double quote
- `\?` - question mark
- `\nnn` - carácter octal (1-3 dígitos)
- `\xHH` - carácter hexadecimal (1-2 dígitos)
- `\uHHHH` - Unicode (1-4 hex dígitos)
- `\UHHHHHHHH` - Unicode (1-8 hex dígitos)
- `\cx` - carácter control-x

---

## Reserved Words

Palabras reconocidas como reservadas cuando están sin comillas y son la primera palabra de un comando:

```
if      then    elif    else    fi      time
for     in      until   while   do      done
case    esac    coproc  select  function
{       }       [[      ]]      !
```

**Excepciones:**
- `in` es reservada si es la tercera palabra de `case` o `select`
- `in` y `do` son reservadas si son la tercera palabra de `for`

---

## Special Parameters

Parámetros especiales de solo lectura (no se pueden asignar):

### $*
Expande a los parámetros posicionales desde el 1.
- Sin comillas: cada parámetro es una palabra separada
- Con comillas `"$*"`: una sola palabra con valores separados por el primer carácter de `IFS`

### $@
Expande a los parámetros posicionales desde el 1.
- Sin comillas: cada parámetro es una palabra separada
- Con comillas `"$@"`: cada parámetro es una palabra separada
- Sin parámetros: `"$@"` y `$@` no expanden a nada

### $#
Número de parámetros posicionales en decimal.

### $?
Exit status del último pipeline ejecutado en foreground.

### $-
Flags de opciones actuales (especificadas en invocación, por `set`, o por el shell mismo como `-i`).

### $$
Process ID del shell.
- En subshell: PID del shell invocador, no del subshell.

### $!
Process ID del último job puesto en background.

### $0
Nombre del shell o nombre del script usado para invocar Bash

---

## Utilidades

### Detectar si es tab copiado o pegado
```c
int has_more_bytes(int fd) {
    int bytes_pending;
    if (ioctl(fd, FIONREAD, &bytes_pending) == -1) {
        return 0;  // Error
    }
    return bytes_pending > 0;  // 1 si hay más bytes
}
```

---

## Validación de Sintaxis

### Contextos Válidos por Tipo de Token

#### a. Primer Token
- ❌ No puede ser operador lógico (`&&`, `||`, `;`)
- ❌ No puede ser pipe (`|`)

#### b. Después de Comando/Argumento
- ✅ Operador lógico (`&&`, `||`, `;`)
- ✅ Redirección (`<`, `>`, `>>`)
- ✅ Pipe (`|`)

#### c. Después de Operador Lógico
- ✅ Comando
- ✅ Paréntesis de apertura `(`
- ✅ Redirección
- ❌ Pipe
- ❌ Otro operador lógico

#### d. Después de Pipe
- ✅ Comando
- ✅ Paréntesis de apertura `(`
- ✅ Redirección
- ❌ Operador lógico
- ❌ Otro pipe

#### e. Después de Redirección
- ✅ Archivo (string de ruta)
- ❌ Pipe
- ❌ Operador lógico
- ❌ Paréntesis de apertura `(`

#### f. Después de `(`
- ✅ Comando
- ✅ Paréntesis de apertura `(` (subcomando)
- ✅ Redirección
- ❌ Operador lógico
- ❌ Pipe

#### g. Después de `)`
- ✅ Operador lógico
- ✅ Pipe
- ✅ Final de línea
- ✅ Redirección
- ❌ Comando

### Final del Análisis
- ❌ No puede haber operador lógico, pipe o redirección al final
- ❌ No puede haber comando vacío (ej: después de pipe sin comando)

---

### Secciones de Help
1. Input
2. History
3. Variables
4. Wildcards
5. Builtins
6. Redirections
