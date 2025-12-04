# 42sh

## Readline

- \'"'!!'"\' CTRL + Left / CTRL + Right jodido y leaks

## Parser

- Asignaciones en comandos
- Para heredoc open, unlink, lseek, así cuando cierre el fd se elimina automáticamente.
- Redirs detectar dígitos sin consumir y si hay redirección al final. Si se cumple, es redirección.
- Tratar casos como echo popo && ((echo lala) && echo pipi) o (echo lala && (echo pipi))

### Command
- Asignation
- Redirection
- Argument
### Grouping
- ()
- { ; }

## Builtins

- cdspell effect only in interactive
- command -v no es identico a bash
- hash no se como añadir manualmente
- fc no guarda comando en historial al ejecutar (el modificado, no fc en sí)
- ignoreeof of set "Use "logout" to leave the shell." en stderr

## Others

- Makefile no "recompile"
- Caracteres octal y hexadecimal
- TMOUT close shell after prompt
