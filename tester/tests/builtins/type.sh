#     Create and list the type of a simple command
shopt -s expand_aliases ; type foo ; alias foo="echo Foo" ; type foo

#     Type of a command that doesn't exist
type nonexistentcommand

#     Check the type of a built-in command
type echo ; type cd ; type type

#     Create and list type of an alias
shopt -s expand_aliases ; unalias -a ; alias foo="echo Foo" ; type foo

#     Type of a command with arguments (should show that it is not found)
type ls -l

#     Check type of a function
my_function() { echo "Hello, World!"; } ; type my_function

#     Check type of a shell built-in that is also an alias
shopt -s expand_aliases ; unalias -a ; alias cd="echo Custom CD" ; type cd

#     Type of a command with a keyword
type if ; type while ; type for

#     Check type of a command after redefining it
shopt -s expand_aliases ; type echo ; alias echo="echo Custom Echo" ; type echo

#     Type for a command that is an executable file
type /bin/ls ; type /usr/bin/python

#     Check type of a command inside a script
echo -e '#!/bin/bash\ntype ls' > test.sh ; bash test.sh

#     Type after using unalias
shopt -s expand_aliases ; unalias -a ; alias foo="echo Foo" ; unalias foo ; type foo

#     Check type of a built-in command inside a script
echo -e '#!/bin/bash\ntype cd' > test.sh ; bash test.sh ; rm test.sh

#     Type of a function inside a script
echo -e '#!/bin/bash\nmy_function() { echo "Hello, World!"; }\ntype my_function' > test.sh ; bash test.sh ; rm test.sh

#     Check type of a command inside a subshell
shopt -s expand_aliases ; ( alias foo="echo Foo" ; type foo )

#     Type of a command with a non-existent command in the path
type non_existent_command

#     Type after aliasing a built-in command
shopt -s expand_aliases ; unalias -a ; alias echo="echo Custom Echo" ; type echo

#     Type with a complex function (with parameters)
complex_function() { echo "Hello, $1"; } ; type complex_function
