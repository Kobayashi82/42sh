#     Crear y listar alias simples
unalias -a ; alias foo="echo Foo" ; alias bar="ls -l" ; alias -p # prueba

#     Sobreescribir alias existente
unalias -a ; alias foo="echo Foo" ; alias foo="echo Bar" ; alias -p

#     Intentar ejecutar alias sin comillas
unalias -a ; alias foo=echo Foo ; foo

#     Alias con espacios y sin comillas
unalias -a ; alias foo=echo\ Foo ; foo

#     Alias con comillas simples dentro
unalias -a ; alias foo='echo '\''Hola'\''' ; foo

#     Crear alias que contiene otro alias
unalias -a ; alias foo="echo Foo" ; alias bar="foo && echo Bar" ; bar

#     Intentar alias recursivo
unalias -a ; alias foo="foo" ; foo

#     Alias con redirección
unalias -a ; alias foo="echo Foo > out.txt" ; foo ; cat out.txt ; rm out.txt

#     Alias que usa otro alias como comando
unalias -a ; alias foo="echo Foo" ; alias bar="foo && echo Bar" ; bar

#     Borrar alias específico
unalias -a ; alias foo="echo Foo" ; alias bar="echo Bar" ; unalias foo ; alias -p

#     Borrar todos los alias con unalias -a
unalias -a ; alias foo="echo Foo" ; alias bar="echo Bar" ; unalias -a ; alias -p

#     Alias con pipes (|)
unalias -a ; alias foo="echo Foo | grep F" ; foo

#     Alias con variables
unalias -a ; alias foo="echo $HOME" ; foo

#     Alias con && y ||
unalias -a ; alias foo="false" ; alias bar="foo || echo Fallback" ; bar

#     Alias con argumentos (no funcionan)
unalias -a ; alias foo="echo Foo" ; foo bar

#     Alias dentro de un script
unalias -a ; echo "alias foo=\"echo Foo\"" > test.sh ; bash test.sh ; alias -p

#     Crear alias y borrar con unalias -a en la misma línea
unalias -a ; alias foo="echo Foo" ; alias bar="echo Bar" ; unalias -a ; alias -p

#     Alias con cd y efecto en el shell
unalias -a ; alias gohome="cd ~" ; gohome ; pwd