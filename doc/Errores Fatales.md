# Errores que cierran la shell no-interactiva en modo POSIX

## Special Builtins POSIX
`.` `:` `break` `continue` `eval` `exec` `exit` `export` `readonly` `return` `set` `shift` `times` `trap` `unset`

## Errores fatales (cierran la shell no-interactiva)

### 1. Opciones incorrectas a special builtins
```bash
export -z  # Invalid option
```

### 2. Errores de redirección en special builtins
```bash
export VAR=value > /nonexistent/path
```

### 3. Errores de asignación cuando preceden a special builtins
```bash
readonly VAR=old
VAR=new export OTHER=value  # Assignment to readonly variable
```

### 4. Identificadores inválidos en export/readonly/unset
```bash
export 123invalid  # Invalid identifier
readonly "bad var"
unset 1variable
```

### 5. Archivo no encontrado en dot command
```bash
. /path/to/nonexistent/file
source /missing/script
```

### 6. Error de sintaxis en eval/dot/source
```bash
eval "if then"  # Syntax error
. script_with_syntax_error.sh
```

### 7. Variable readonly o nombre inválido en for/select
```bash
readonly i
for i in 1 2 3; do echo $i; done  # readonly iteration variable
```

## Errores que NO cierran la shell (solo retornan código ≠ 0)

- `shift N` donde N > $#
- Cualquier error en builtins normales (no-special)
- Errores de asignación que NO preceden a special builtins
- `cd` a directorio inexistente
- `unset` de variable inexistente (sin -v)
