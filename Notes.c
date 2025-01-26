// SIGHUP, SIGTERM para guardar historial
// leaks si history file no existe
// 
// fc builtin
// history builtin
// CTRL + R
// !!
// 
// Casos de comillas sin cerrar o terminacion en token
// 
// En set o unset, si son variables del historial, actualizar variables internas
// 
// Vi input
// 
// recordar que - (guion) suele indicar stdin o stdout para algunos comandos, como cat, por ejemplo. en mi caso,
// creo que se trata normal y corriente. No creo que tenga que hacer nada
// 
// !!				substitute for the last command
// !«-n»			substitute for command «n» from the end of history
// !«n»			substitute for command event «n» from history
// !«prefix»		substitute for the last command beginning with “«prefix»”
// !?«substr»?		substitute for the last command containing “«substr»”.


//	CONSTANTES
//
// El valor al que apunta el puntero es constante:
// const int *ptr = &value; // No puedes modificar *ptr, pero puedes cambiar ptr.

// El puntero en sí es constante:
// int *const ptr = &value; // Puedes modificar *ptr, pero ptr siempre apuntará al mismo lugar.

// Ambos son constantes:
// const int *const ptr = &value; // Ni *ptr ni ptr pueden cambiar.



//	SHELL OPERATION
//
// 1. Reads its input from a file (see Shell Scripts), from a string supplied as an argument to the -c invocation option (see Invoking Bash), or from the user’s terminal.
// 2. Breaks the input into words and operators, obeying the quoting rules described in Quoting. These tokens are separated by metacharacters. Alias expansion is performed by this step (see Aliases).
// 3. Parses the tokens into simple and compound commands (see Shell Commands).
// 4. Performs the various shell expansions (see Shell Expansions), breaking the expanded tokens into lists of filenames (see Filename Expansion) and commands and arguments.
// 5. Performs any necessary redirections (see Redirections) and removes the redirection operators and their operands from the argument list.
// 6. Executes the command (see Executing Commands).
// 7. Optionally waits for the command to complete and collects its exit status (see Exit Status).

// Shell Expansions
// Brace Expansion
// Tilde Expansion
// Shell Parameter Expansion
// Command Substitution
// Arithmetic Expansion
// Process Substitution
// Word Splitting
// Filename Expansion
// Pattern Matching
// Quote Removal
// Redirections


//	SIMPLE COMMAND EXPANSION
//
// When a simple command is executed, the shell performs the following expansions, assignments, and redirections, from left to right, in the following order.

// The words that the parser has marked as variable assignments (those preceding the command name) and redirections are saved for later processing.
// The words that are not variable assignments or redirections are expanded (see Shell Expansions). If any words remain after expansion, the first word is taken
// to be the name of the command and the remaining words are the arguments.
// Redirections are performed as described above (see Redirections).
// The text after the ‘=’ in each variable assignment undergoes tilde expansion, parameter expansion, command substitution, arithmetic expansion, and quote removal before being assigned to the variable.
// If no command name results, the variable assignments affect the current shell environment. In the case of such a command (one that consists only of assignment statements and redirections),
// assignment statements are performed before redirections. Otherwise, the variables are added to the environment of the executed command and do not affect the current shell environment.
// If any of the assignments attempts to assign a value to a readonly variable, an error occurs, and the command exits with a non-zero status.

// If no command name results, redirections are performed, but do not affect the current shell environment. A redirection error causes the command to exit with a non-zero status.

// If there is a command name left after expansion, execution proceeds as described below. Otherwise, the command exits. If one of the expansions contained a command substitution,
// the exit status of the command is the exit status of the last command substitution performed. If there were no command substitutions, the command exits with a status of zero.



//	QUOTING
//
// En cuanto hay una #, todo lo que sigue se considera un comentario a no ser que esté escapada o entre comillas

// En comillas simples, todo es literal, pero no se puede poner una comilla simple dentro de comillas simples

// En comillas dobles, $, ` y \ mantienen su funcion y ! solamente cuando POSIX está off
// el \ solo se aplica con $, `, ", \ y un salto de linea. Esto significa que se elimina la \.
// En el caso de \newline se quita la \ y el salto de linea.
// Tambien se elimina la expansion de !, pero en este caso no se elimina la \.

//	$"hello, word"		Se trata como "hello, word", eliminando el $

//	$'hello\nword'		Se trata como una cadena literal con expansion de caracteres, eliminando el $		(al parecer se convierte en 'hello'$'\n''word')
//		\a			alert (bell)
//		\b			backspace
//		\e, \E		an escape character (not ANSI C)
//		\f			form feed
//		\n			newline
//		\r			carriage return
//		\t			horizontal tab
//		\v			vertical tab
//		\\			backslash
//		\'			single quote
//		\"			double quote
//		\?			question mark
//		\nnn		the eight-bit character whose value is the octal value nnn (one to three octal digits)
//		\xHH		the eight-bit character whose value is the hexadecimal value HH (one or two hex digits)
//		\uHHHH		the Unicode (ISO/IEC 10646) character whose value is the hexadecimal value HHHH (one to four hex digits)
//		\UHHHHHHHH	the Unicode (ISO/IEC 10646) character whose value is the hexadecimal value HHHHHHHH (one to eight hex digits)
//		\cx			a control-x character



//	RESERVED WORDS
//
// The following words are recognized as reserved when unquoted and the first word of a command (see below for exceptions):

// if	then	elif	else	fi	time
// for	in	until	while	do	done
// case	esac	coproc	select	function
// {	}	[[	]]	!

// in is recognized as a reserved word if it is the third word of a case or select command.
// in and do are recognized as reserved words if they are the third word in a for command.



//	SPECIAL PARAMETERS
//
// The shell treats several parameters specially. These parameters may only be referenced; assignment to them is not allowed.

// $*	Expands to the positional parameters, starting from one. When the expansion is not within double quotes, each positional
//		parameter expands to a separate word. In contexts where it is performed, those words are subject to further word splitting
//		and filename expansion. When the expansion occurs within double quotes, it expands to a single word with the value of each
//		parameter separated by the first character of the IFS special variable. That is, "$*" is equivalent to "$1c$2c…", where c is
//		the first character of the value of the IFS variable. If IFS is unset, the parameters are separated by spaces. If IFS is null,
//		the parameters are joined without intervening separators.

// $@	Expands to the positional parameters, starting from one. In contexts where word splitting is performed, this expands each
//		positional parameter to a separate word; if not within double quotes, these words are subject to word splitting. In contexts
//		where word splitting is not performed, this expands to a single word with each positional parameter separated by a space.
//		When the expansion occurs within double quotes, and word splitting is performed, each parameter expands to a separate word.
//		That is, "$@" is equivalent to "$1" "$2" …. If the double-quoted expansion occurs within a word, the expansion of the first
//		parameter is joined with the beginning part of the original word, and the expansion of the last parameter is joined with the
//		last part of the original word. When there are no positional parameters, "$@" and $@ expand to nothing (i.e., they are removed).

// $#	Expands to the number of positional parameters in decimal.

// $?	Expands to the exit status of the most recently executed foreground pipeline.

// $-	Expands to the current option flags as specified upon invocation, by the set builtin command, or those set by the shell itself (such as the -i option).

// $$	Expands to the process ID of the shell. In a subshell, it expands to the process ID of the invoking shell, not the subshell.

// $!	Expands to the process ID of the job most recently placed into the background, whether executed as an asynchronous command or using the bg builtin (see Job Control Builtins).

// $0	Expands to the name of the shell or shell script. This is set at shell initialization. If Bash is invoked with a file of
//		commands (see Shell Scripts), $0 is set to the name of that file. If Bash is started with the -c option (see Invoking Bash),
//		then $0 is set to the first argument after the string to be executed, if one is present. Otherwise, it is set to the filename used to invoke Bash, as given by argument zero.



// Readline
// -------------
// Undo
// Switch word
// Fix navigation and switch char/word

// Dumb
// ---------
// Backspace, enter (tab or delete?)

// Vi
// ---
// Undo
// Fix navigation and switch char
// Todas las cosas de vi


// Partes importantes:

// Fix navigation
// Tab
// Undo/Redo

// Opciones relacionadas:
// Historial
// Emacs/vi
// AutoComplete


// Fix Navigation:

// Al mover el cursor, determinar si el caracter es ampliado.

// Esto es importante al cambiar de fila.

// Hay que cambiar:

// Backspace (todos)
// Delete (todos)
// Home y End

// Left y right no afectan en el sentido de que se calcula el carácter en base a la posición actual en el buffer.

// Ademas, optimizar delete word.
// Puede ser que necesite hacer get_cursor solo al principio y al redimensionar.

// Por supuesto switch char y word debe de funcionar cuando está entre filas. Lo mismo que borra palabras


// AutoComplete necesita
// Alias
// Variables
// Builtins
// Options

// History, readinput necesita
// Options

// Los history expansión se hacen antes de devolver el readinput?


// Builtins como builtin que dice los builtins que existen?

// Help?


// Help

// Here are the sections available:

// 1. Input
// 2. History
// 3. Variables
// 4. Wildcards
// 5. Builtins
// 6. Redirections


// Input
//     History
//     AutoComplete
// Parser
//     Alias
//     Variables
//     Expansion
//     Wildcards
// Executer
//     Fork
//     Redirections
//     Builtins


// y - Copy
// d - Delete
// c - Delete & Insertion
// r - Replace char
// R - Replace mode
// u - Undo last change
// U - Undo all
// CTRL + W - Backspace word
// CTRL + _ - Undo last change

// +- numeric & printed

// int has_more_bytes(int fd) {
//  int bytes_pending;
//  if (ioctl(fd, FIONREAD, &bytes_pending) == -1) {
//   return 0; // Devuelve false si hay error
//  }
//  return bytes_pending > 0; // Devuelve true si hay más bytes
// }

// Divido pattern en una lista.
// Optimizar cada value en cada nodo

// Que pasa si hay varios nodos con **/?

// Obtener base dir (directorio inicial)
// Por cada nodo genero un files. Si no es el último nodo, llamo a la misma función con el siguiente nodo y con files como base dir. Tiene que devolver un puntero de tipo files la función.
// Si llega al último nodo y no hay coincidencias, devuelve null, si no, devuelve un puntero a la coincidencia.
// Pero es importante que se hagan merge de los files devueltos.
