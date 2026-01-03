# 42sh

- Redirections
- Syntax error
- Arithmetic expresion
- Arithmetic expansion
- Conditional expresion
- Braces expansion
- Parameter expansion

## History

```bash
history -a                            # Añade solo comandos nuevos al archivo
history -n                            # Lee solo las líneas nuevas del archivo (las que aún no están en memoria)
fc                                    # Arreglar guardado de comandos editados
```

### Carga
- Carga todo el archivo


Cuando cargue el archivo al iniciar o -r
- mem_new_pos (índice donde empiezan los comandos nuevos. Si se elimina una entrada inferior a ese índice, se disminuye y si se vacía el historial se pone a 0.
- mem_new_number (este es el número de comando último cargado. Si el archivo tiene más comandos que este cuando se usa -n se insertan entre la posición de ese comando y mem_new_number. Luego se actualiza mem_new_number y mem_new_pos.

## Terminal
- TMOUT: Si está definido, cerrar la shell automáticamente después de N segundos de inactividad en el prompt (sin input del usuario)
- set -o ignoreeof: Si está activado, ignorar EOF (Ctrl + D) y muestra 'Use "logout" to leave the shell.' en stderr en lugar de cerrar la shell inmediatamente. Pero si se pulsa (CTRL + D) 11 veces seguidas, tambien se cierra. (No se si logout seria lo correcto en mi caso, porque no es el shell usado para login)

## Parser
- Para heredoc open, unlink, lseek, así cuando cierre el fd se elimina automáticamente
- Pasar process_substitution a expansion

## Builtins
- command -v no es identico a bash (ni lo tiene que ser... ya lo miraré)
- hash, implementar opciones -tpdr


## 42sh

- `declare` - Unfinished
- `history`
- `logout` - Unfinished
- `shopt`

## Regular
- `alias`
- `cd`
- `command`
- `false`
- `fc`
- `hash`
- `true`
- `type`
- `ulimit`
- `unalias`

## Special
- `dots`
- `exec`
- `exit`
- `export`
- `readonly`
- `unset`


## Source
- Opciones de arranque (mirar Bash_Options.md)

## 42

- parse_subshell()
- parse_arithmetic()
- parser_conditional()
- process_heredocs()
- create_heredoc()
- Señales en parser
- Mensajes de error (syntax)
- Funciones de argumentos (unid, dividir, array)
- Funciones de redirección (aplicar, heredoc)

---

### No Interactivo

- Cuando el input llega a EOF y lexer->input no es NULL;

### Interactivo

- Cuando se llega a EOF de input de usuario y hay:
- Contexto abierto
- Termina en \
- Termina en operdor de continuación (&&, ||, |, |&)
