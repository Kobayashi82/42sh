# Bash Lexer - Lista Completa de Elementos

## 1. Expansiones

### Expansión de Parámetros
- [x] `$var` - expansión simple de variable
- [x] `${}` - expansión de parámetro con llaves
  - `${var}` - forma básica
  - `${var:-default}` - valor por defecto si no está set
  - `${var:=default}` - asignar default si no está set
  - `${var:?error}` - error si no está set
  - `${var:+alternate}` - usar alternate si está set
  - `${var:offset:length}` - substring
  - `${#var}` - longitud
  - `${var#pattern}` - eliminar prefijo corto
  - `${var##pattern}` - eliminar prefijo largo
  - `${var%pattern}` - eliminar sufijo corto
  - `${var%%pattern}` - eliminar sufijo largo
  - `${var/pattern/replacement}` - reemplazar primera ocurrencia
  - `${var//pattern/replacement}` - reemplazar todas las ocurrencias
  - `${var^}` - uppercase primer carácter
  - `${var^^}` - uppercase todo
  - `${var,}` - lowercase primer carácter
  - `${var,,}` - lowercase todo
  - `${!prefix*}` - expansión indirecta (nombres que empiezan con prefix)
  - `${!var}` - indirección

### Expansión Aritmética
- [x] `$(())` - expansión aritmética
- [x] `$((expression))` - evaluación de expresiones aritméticas

### Sustitución de Comandos
- [x] `$()` - sustitución de comando (moderna)
- [x] `` ` ` `` - sustitución de comando (legacy)

### Expansión de Tilde
- [ ] `~` - home directory del usuario actual
- [ ] `~user` - home directory de usuario específico
- [ ] `~+` - directorio actual ($PWD)
- [ ] `~-` - directorio anterior ($OLDPWD)

### Brace Expansion
- [x] `{a,b,c}` - lista de valores
- [ ] `{1..10}` - rango numérico
- [ ] `{a..z}` - rango alfabético
- [ ] `{01..10}` - rango con padding
- [ ] `{a..z..2}` - rango con incremento

### Pathname Expansion (Globbing)
- [ ] `*` - cualquier secuencia de caracteres
- [ ] `?` - un carácter cualquiera
- [ ] `[...]` - conjunto de caracteres
- [ ] `[^...]` o `[!...]` - negación de conjunto
- [ ] `[a-z]` - rango de caracteres
- [ ] `**` - globbing recursivo (si globstar está activo)

### Extended Globbing (si extglob está activo)
- [ ] `?(pattern-list)` - cero o una ocurrencia
- [ ] `*(pattern-list)` - cero o más ocurrencias
- [ ] `+(pattern-list)` - una o más ocurrencias
- [ ] `@(pattern-list)` - exactamente una
- [ ] `!(pattern-list)` - cualquier cosa excepto

### Quoting y Strings
- [x] `$""` - ANSI-C quoting con expansiones
- [x] `$''` - ANSI-C quoting sin expansiones
- [ ] `"..."` - double quotes (permite expansiones)
- [ ] `'...'` - single quotes (literal, sin expansiones)
- [ ] `\` - backslash escape

## 2. Redirecciones

### Redirecciones Básicas
- [x] `>` - redirigir stdout (sobrescribir)
- [x] `>>` - redirigir stdout (append)
- [x] `<` - redirigir stdin
- [x] `<<` - here-document
- [ ] `<<<` - here-string
- [x] `>&` - redirigir stdout y stderr
- [x] `<&` - duplicar file descriptor
- [x] `>|` - sobrescribir aunque noclobber esté activo
- [x] `&>` - redirigir stdout y stderr (forma alternativa)
- [x] `&>>` - append stdout y stderr

### Redirecciones Avanzadas
- [x] `<()` - process substitution (input)
- [x] `>()` - process substitution (output)
- [x] `n<` - redirigir desde file descriptor n
- [x] `n>` - redirigir a file descriptor n
- [x] `n<>` - abrir file descriptor n para lectura/escritura

## 3. Operadores y Control de Flujo

### Operadores de Listas
- [x] `&&` - AND lógico (ejecutar siguiente si anterior tiene éxito)
- [x] `||` - OR lógico (ejecutar siguiente si anterior falla)
- [x] `;` - separador de comandos secuencial
- [x] `&` - ejecutar en background
- [x] `|` - pipe
- [x] `|&` - pipe stdout y stderr

### Agrupación de Comandos
- [x] `()` - subshell
- [x] `{}` - group command (en shell actual)
- [x] `{ cmd ; }` - sintaxis con espacios necesarios

### Expresiones Condicionales
- [x] `((expression))` - evaluación aritmética
- [ ] `[[ expression ]]` - conditional expression (bash)
- [ ] `[ expression ]` - test command (POSIX)
- [ ] Operadores dentro de `[[]]`:
  - `-eq`, `-ne`, `-lt`, `-le`, `-gt`, `-ge` - comparaciones numéricas
  - `=`, `==`, `!=` - comparaciones de strings
  - `=~` - regex match
  - `-z`, `-n` - string vacío/no vacío
  - `-e`, `-f`, `-d`, `-r`, `-w`, `-x` - tests de archivos
  - `!` - negación
  - `&&`, `||` - operadores lógicos dentro de `[[]]`

## 4. Estructuras de Control (Keywords)

Mencionas que no cuentas keywords, pero tienen sintaxis léxica especial:

### Condicionales
- [ ] `if ... then ... elif ... else ... fi`
- [ ] `case ... in ... esac`
  - Patrones con `|` para alternativas
  - `;;&` - fallthrough (bash 4+)
  - `;&` - fallthrough sin evaluar siguiente patrón (bash 4+)
  - `;;` - break normal
  - `;&` - continue to next pattern

### Bucles
- [ ] `for ... in ... do ... done`
- [ ] `for (( init; condition; increment )); do ... done`
- [ ] `while ... do ... done`
- [ ] `until ... do ... done`
- [ ] `select ... in ... do ... done`

### Otros
- [ ] `function name { }` o `name() { }`
- [ ] `!` - negación de pipeline
- [ ] `time` - medir tiempo de ejecución
- [ ] `coproc` - coprocess (bash 4+)

## 5. Variables y Parámetros Especiales

### Parámetros Posicionales
- [ ] `$0` - nombre del script
- [ ] `$1`, `$2`, ... `$9`, `${10}`, ... - argumentos posicionales
- [ ] `$#` - número de argumentos
- [ ] `$@` - todos los argumentos (como array)
- [ ] `$*` - todos los argumentos (como string)

### Parámetros de Estado
- [ ] `$$` - PID del shell actual
- [ ] `$!` - PID del último proceso en background
- [ ] `$?` - exit status del último comando
- [ ] `$-` - flags actuales del shell
- [ ] `$_` - último argumento del comando anterior

### Arrays
- [ ] `array=(a b c)` - declaración de array
- [ ] `array[index]=value` - asignación a índice
- [ ] `${array[index]}` - acceso a elemento
- [ ] `${array[@]}` - todos los elementos
- [ ] `${array[*]}` - todos los elementos (como string)
- [ ] `${#array[@]}` - tamaño del array
- [ ] `${!array[@]}` - índices del array

### Arrays Asociativos (bash 4+)
- [ ] `declare -A assoc` - declaración
- [ ] `assoc[key]=value` - asignación
- [ ] `${assoc[key]}` - acceso

## 6. Asignaciones y Declaraciones

- [ ] `var=value` - asignación simple
- [ ] `var+=value` - append
- [ ] `declare` / `typeset` - declarar variables con atributos
- [ ] `local` - variable local en función
- [ ] `export` - exportar variable a subprocesos
- [ ] `readonly` - variable de solo lectura
- [ ] `unset` - eliminar variable

## 7. Comentarios y Otros

- [ ] `#` - comentario hasta fin de línea
- [ ] History expansion:
  - `!!` - último comando
  - `!n` - comando número n
  - `!string` - último comando que empieza con string
  - `!?string` - último comando que contiene string
  - `^old^new` - sustituir en último comando

## 8. Caracteres Especiales y Metacaracteres

- [ ] ` ` (espacio/tab) - separadores
- [ ] `\n` - newline (separador de comandos)
- [ ] `\` - escape character
- [ ] `|` - pipe
- [ ] `&` - background/redirección
- [ ] `;` - separador de comandos
- [ ] `(` `)` - subshell/agrupación
- [ ] `<` `>` - redirección
- [ ] Caracteres especiales en diferentes contextos

## 9. Palabras Reservadas (Keywords)

- [ ] `if`, `then`, `elif`, `else`, `fi`
- [ ] `case`, `in`, `esac`
- [ ] `for`, `while`, `until`, `do`, `done`
- [ ] `select`
- [ ] `function`
- [ ] `{`, `}`
- [ ] `!`
- [ ] `[[`, `]]`
- [ ] `time`
- [ ] `coproc`

## Notas Importantes

1. **Contexto**: Muchos caracteres tienen significados diferentes según el contexto (dentro de quotes, en expansiones, etc.)
2. **Compatibilidad**: Algunos elementos son específicos de bash y no POSIX sh
3. **Orden de Expansión**: Las expansiones ocurren en un orden específico que el lexer debe respetar
4. **Whitespace**: El manejo de espacios es crítico en bash (ej: `{cmd;}` vs `{ cmd; }`)