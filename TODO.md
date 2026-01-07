# 42sh

- Redirections
- Syntax error
- Arithmetic expresion
- Arithmetic expansion
- Conditional expresion
- Braces expansion
- Parameter expansion

## Límites
- Límite de expansión
- Límite de eval

## Terminal
- CTRL + D no deberia terminar input, deberia hacer beep si hay algo en el buffer
- TMOUT: Si está definido, cerrar la shell automáticamente después de N segundos de inactividad en el prompt (sin input del usuario)
- set -o ignoreeof: Si está activado, ignorar EOF (Ctrl + D) y muestra 'Use "logout" to leave the shell.' en stderr en lugar de cerrar la shell inmediatamente. Pero si se pulsa (CTRL + D) 11 veces seguidas, tambien se cierra. (No se si logout seria lo correcto en mi caso, porque no es el shell usado para login)

## Parser
- Para heredoc open, unlink, lseek, así cuando cierre el fd se elimina automáticamente
- Pasar process_substitution a expansion

## Builtins
- getopt (builtin) debe soportar multibyte chars

## History
- Cambiar sistema de -a -n para que use timestamp y ordene automaticamente
- fc en script
- Script se ejecutan en contexto actual
- Input separar script de interactive
- Si editar y output no es tty, error?
- Si editar y input no es tty, error?
- Input es el que añade a historial (somehow)
- Input hace un while y va enviando líneas hasta que
- line =null
- ast = null (no interactivo y no script)

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


### Ni puta idea de porqué escribí esto

- Modo vi de fc hace beep si falla?
- Argumentos del editor en fc
- Quizás lo más razonable sea usar el editor tal cual y llamar a parse_and_execute mío con el comando formado.
- Editor tmp_file
- Y que falle lo que tenga que fallar y tal. Cuando termina el comando, si es 0 continua, si no, error
- Devuelvo en default_editor el primero available, da igual si existe o no.
- O -e arg
