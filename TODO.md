# 42sh

- Makefile no "recompile"
- Caracteres octal y hexadecimal
- Command -v no es identico a bash
- Hash no se como añadir manualmente
- Realizar testeo completo de builtins (todos las opciones)
- TMOUT close shell after prompt
- interactive variable
- ignoreeof of set "Use "logout" to leave the shell." en stderr
- cdspell effect only in interactive
- \'"'!!'"\' CTRL + Left / CTRL + Right jodido y leaks

Lookahead
Sistema de procesamiento:
Pipeline
And_or
Command 
...

En command, asignaciones, redirecciones, argumentos

En handle quoted, paréntesis
Tratar ) fuera de contextos (error)

Subshell y aritmética (no estoy seguro de este, quizás solo operadores y words), si parser
Operadores como <<- y &<- o <- no se si ponerlos

## Builtins

- fc no guarda comando en historial al ejecutar (el modificado, no fc en sí)