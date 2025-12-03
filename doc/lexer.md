# Bash Lexer - Tokens Completos

Esta lista contiene **únicamente los tokens que el lexer debe reconocer**, no las expansiones semánticas que ocurren después.

## 1. Tokens de Operadores

### Operadores de Control de Flujo
- [x] `&&` - AND list
- [x] `||` - OR list
- [x] `;` - command separator
- [ ] `;;` - case clause terminator
- [ ] `;;&` - case fallthrough (bash 4+)
- [ ] `;&` - case fallthrough sin test (bash 4+)
- [x] `&` - background
- [x] `|` - pipe
- [x] `|&` - pipe stderr y stdout
- [ ] `!` - pipeline negation (cuando es keyword)
- [ ] newline - separador de comandos

### Operadores de Redirección
- [x] `<` - input redirection
- [x] `>` - output redirection
- [x] `>>` - append output
- [x] `<<` - here-document
- [ ] `<<<` - here-string
- [x] `<&` - duplicate input fd
- [x] `>&` - duplicate output fd
- [x] `<>` - open for reading and writing
- [x] `>|` - force overwrite (noclobber)
- [x] `&>` - redirect stdout and stderr
- [x] `&>>` - append stdout and stderr
- [ ] `[n]<` - numbered input redirection (ej: `3<`)
- [ ] `[n]>` - numbered output redirection (ej: `2>`)
- [ ] `[n]>>` - numbered append
- [ ] `[n]<&` - numbered duplicate input
- [ ] `[n]>&` - numbered duplicate output
- [ ] `[n]<>` - numbered read/write

### Process Substitution
- [x] `<(` - process substitution input
- [x] `>(` - process substitution output

## 2. Tokens de Agrupación

### Paréntesis y Llaves
- [x] `(` - subshell start / arithmetic start
- [x] `)` - subshell end / arithmetic end
- [x] `((` - arithmetic evaluation
- [x] `))` - arithmetic evaluation end
- [x] `{` - group command start / brace expansion
- [x] `}` - group command end / brace expansion
- [ ] `[[` - conditional expression start
- [ ] `]]` - conditional expression end
- [ ] `[` - test command (puede ser tratado como palabra)
- [ ] `]` - test command end

## 3. Tokens de Expansión

### Command Substitution
- [x] `$(` - command substitution start
- [x] `` ` `` - backtick command substitution

### Parameter Expansion
- [x] `${` - parameter expansion start
- [x] `$` - simple variable expansion (cuando va seguido de palabra)

### Quoting
- [x] `$"` - locale-translated string
- [x] `$'` - ANSI-C quoting
- [ ] `"` - double quote
- [ ] `'` - single quote
- [ ] `\` - escape character

## 4. Palabras Reservadas (Keywords)

Estas son palabras que tienen significado especial para el shell:

### Condicionales
- [ ] `if`
- [ ] `then`
- [ ] `elif`
- [ ] `else`
- [ ] `fi`
- [ ] `case`
- [ ] `in`
- [ ] `esac`

### Loops
- [ ] `for`
- [ ] `while`
- [ ] `until`
- [ ] `do`
- [ ] `done`
- [ ] `select`

### Funciones y Otros
- [ ] `function`
- [ ] `time`
- [ ] `coproc`
- [ ] `!` (como keyword de negación)

### Keywords de Control
- [ ] `break`
- [ ] `continue`
- [ ] `return`

## 5. Palabras (Words)

Las palabras son secuencias de caracteres que no son operadores ni whitespace. El lexer debe reconocer:

- [x] **WORD** - palabra normal (variable names, comandos, argumentos)
- [ ] **NAME** - nombre válido (para variables): `[a-zA-Z_][a-zA-Z0-9_]*`
- [ ] **ASSIGNMENT_WORD** - palabra con asignación: `NAME=value`
- [ ] **NUMBER** - número (para redirecciones numeradas)
- [ ] **IO_NUMBER** - número seguido de operador de redirección

### Caracteres Especiales en Palabras
Estos caracteres pueden aparecer en palabras pero tienen significado especial:
- Globbing: `*`, `?`, `[`, `]` (no son tokens, son parte de WORD)
- Brace expansion: `,`, `..` dentro de `{}` (no son tokens separados)
- Tilde: `~` (no es token, es parte de WORD)

## 6. Caracteres Especiales y Delimitadores

- [ ] **WHITESPACE** - espacios y tabs (delimitadores)
- [ ] **NEWLINE** - `\n` (separador de comandos)
- [ ] **EOF** - end of file
- [ ] `#` - inicio de comentario (hasta newline)

## 7. Tokens Especiales para Arrays

- [ ] `(` y `)` en contexto de array: `array=(values)`
- [ ] `[` y `]` en contexto de array: `array[index]=value`

## 8. Operadores Aritméticos (dentro de `(())` o `$(("))`)

Estos operadores solo son relevantes dentro de contexto aritmético:
- [ ] `+`, `-`, `*`, `/`, `%` - aritméticos básicos
- [ ] `++`, `--` - incremento/decremento
- [ ] `**` - exponenciación
- [ ] `==`, `!=`, `<`, `>`, `<=`, `>=` - comparaciones
- [ ] `<<`, `>>` - bit shifts
- [ ] `&`, `|`, `^`, `~` - operadores bit a bit
- [ ] `&&`, `||` - lógicos (dentro de aritmética)
- [ ] `?` `:` - ternario
- [ ] `=`, `+=`, `-=`, `*=`, `/=`, `%=`, etc. - asignaciones

## 9. Operadores dentro de `[[]]`

- [ ] `=`, `==`, `!=` - comparación de strings
- [ ] `=~` - regex match
- [ ] `<`, `>` - comparación lexicográfica
- [ ] `-eq`, `-ne`, `-lt`, `-le`, `-gt`, `-ge` - comparaciones numéricas
- [ ] `-z`, `-n` - tests de string vacío
- [ ] `-e`, `-f`, `-d`, `-r`, `-w`, `-x`, `-s` - tests de archivos
- [ ] `!` - negación
- [ ] `&&`, `||` - lógicos (dentro de `[[]]`)
- [ ] `(`, `)` - agrupación dentro de `[[]]`

## Notas Importantes para el Lexer

### 1. Contexto Importa
El lexer debe mantener contexto para saber cómo tokenizar:
- Dentro de quotes (`"..."`, `'...'`)
- Dentro de expansiones (`$(...)`, `${...}`, `$((...))`)
- Dentro de condicionales (`[[...]]`)
- Dentro de aritmética (`((...))`))
- Después de operadores de redirección (buscar nombres de archivo)

### 2. Longest Match
Muchos operadores se componen de caracteres que individualmente también son operadores:
- `>` vs `>>` vs `>&`
- `<` vs `<<` vs `<<<` vs `<&`
- `&` vs `&&` vs `&>`
- `;` vs `;;` vs `;;&`
- `(` vs `((`
- `[` vs `[[`

El lexer debe hacer **longest match**.

### 3. Whitespace
El whitespace es significativo:
- `{cmd}` NO es lo mismo que `{ cmd; }`
- `((expression))` requiere tokens pegados
- Algunas keywords solo son keywords al inicio de comando

### 4. Keywords vs Words
Las keywords solo son keywords en posiciones específicas:
- `if` al inicio de comando → keyword
- `if` como argumento → word normal
- Esto puede requerir que el parser indique al lexer qué esperar

### 5. Números en Redirecciones
El número antes de una redirección puede ser parte del token:
- `2>file` → `2>` es un token de redirección
- `2 >file` → `2` es un WORD, `>` es redirección

### 6. Escaping y Quoting
- `\` escapa el siguiente carácter (excepto en single quotes)
- Dentro de `'...'` todo es literal excepto `'`
- Dentro de `"..."` se permiten `$`, `` ` ``, `\`, `"`, newline
- `$'...'` interpreta secuencias escape como `\n`, `\t`, etc.

### 7. Comentarios
- `#` inicia comentario solo al inicio de palabra
- `echo a#b` → `a#b` es una palabra, no tiene comentario
- `echo a #b` → `a` es palabra, `#b` es comentario