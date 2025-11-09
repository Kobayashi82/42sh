#ALIAS CANNOT BE TESTED
#
#     Create and list simple aliases
unalias -a ; alias foo="echo Foo" ; alias bar="ls -l" ; alias -p

#     Overwrite an existing alias
unalias -a ; alias foo="echo Foo" ; alias foo="echo Bar" ; alias -p

#     Attempt to run alias without quotes
unalias -a ; alias foo=echo Foo ; foo

#     Alias with spaces and no quotes
unalias -a ; alias foo=echo\ Foo ; foo

#     Alias with single quotes inside
unalias -a ; alias foo='echo '\''Hola'\''' ; foo

#     Create alias that contains another alias
unalias -a ; alias foo="echo Foo" ; alias bar="foo && echo Bar" ; bar

#     Attempt recursive alias
unalias -a ; alias foo="foo" ; foo

#     Alias with redirection
unalias -a ; alias foo="echo Foo > out.txt" ; foo ; cat out.txt ; rm out.txt

#     Alias using another alias as a command
unalias -a ; alias foo="echo Foo" ; alias bar="foo && echo Bar" ; bar

#     Remove specific alias
unalias -a ; alias foo="echo Foo" ; alias bar="echo Bar" ; unalias foo ; alias -p

#     Remove all aliases with unalias -a
unalias -a ; alias foo="echo Foo" ; alias bar="echo Bar" ; unalias -a ; alias -p

#     Alias with pipes (|)
unalias -a ; alias foo="echo Foo | grep F" ; foo

#     Alias with variables
unalias -a ; alias foo="echo $HOME" ; foo

#     Alias with && and ||
unalias -a ; alias foo="false" ; alias bar="foo || echo Fallback" ; bar

#     Alias with arguments (does not work)
unalias -a ; alias foo="echo Foo" ; foo bar

#     Alias inside a script
unalias -a ; echo "alias foo=\"echo Foo\"" > test.sh ; bash test.sh ; alias -p

#     Create alias and delete with unalias -a in the same line
unalias -a ; alias foo="echo Foo" ; alias bar="echo Bar" ; unalias -a ; alias -p

#     Alias with cd and effect in the shell
unalias -a ; alias gohome="cd ~" ; gohome ; pwd
