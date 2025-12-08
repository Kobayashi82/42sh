# 42sh

## Terminal
- TMOUT: Si está definido, cerrar la shell automáticamente después de N segundos de inactividad en el prompt (sin input del usuario)
- set -o ignoreeof: Si está activado, ignorar EOF (Ctrl + D) y muestra 'Use "logout" to leave the shell.' en stderr en lugar de cerrar la shell inmediatamente. Pero si se pulsa (CTRL + D) 11 veces seguidas, tambien se cierra. (No se si logout seria lo correcto en mi caso, porque no es el shell usado para login)

## Parser
- Para heredoc open, unlink, lseek, así cuando cierre el fd se elimina automáticamente.

## Builtins
- command -v no es identico a bash (ni lo tiene que ser... ya lo miraré)
- hash, implementar opciones -tpdr

## Others
- Makefile no "recompile" (usar Makefile de taskmaster como referencia)

## 42

- parse_subshell()
- parse_arithmetic()
- parser_conditional()
- process_heredocs()
- create_heredoc()
- Señales en parser
- Mensajes de error (syntax)
- Arreglar MemSafe
- exec builtin
- Funciones de argumentos (unid, dividir, array)
- Funciones de redirección (aplicar, heredoc)
