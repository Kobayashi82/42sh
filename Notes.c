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


// SINTAXIS

// 1. Inicio del análisis de sintaxis
// Lo primero es tener un punto de entrada claro para tu análisis: recorrer los tokens, asegurándote de validar la secuencia correcta según las reglas de la shell.

// 2. Validación de tokens en el contexto adecuado
// Recuerda que cada tipo de token (comando, argumento, operador, redirección, etc.) tiene un contexto válido. Por ejemplo, un operador lógico no puede ir al principio, pero sí puede ir después de un comando. Aquí te doy una lista de validaciones comunes:

// a. El primer token:
// No puede ser un operador lógico (&&, ||, ;).
// No puede ser un pipe (|), a menos que sea el único token y se trate de una construcción vacía.
// No puede ser una redirección (<, >, >>).
// Si el primer token es cualquiera de estos, es un error de sintaxis.

// b. Después de un comando o argumento:
// Puede ir un operador lógico (&&, ||, ;).
// Puede ir una redirección (<, >, >>).
// Puede ir un pipe (|).
// Puede ir un paréntesis (( o )), pero debes tener cuidado con los paréntesis desbalanceados.
// c. Después de un operador lógico (&&, ||, ;):
// El siguiente token debe ser un comando o un paréntesis para subcomando.
// No puede ser una redirección, un pipe o un operador lógico.
// Si el siguiente token es un paréntesis (, debes verificar que los paréntesis estén balanceados.
// d. Después de un pipe (|):
// El siguiente token debe ser un comando o un paréntesis.
// No puede ser una redirección, un operador lógico, ni otro pipe.
// e. Después de una redirección (<, >, >>):
// El siguiente token debe ser un archivo (una cadena que represente una ruta).
// Si el siguiente token es un pipe o un operador lógico, es un error de sintaxis.
// f. Después de un paréntesis de apertura (():
// El siguiente token debe ser un comando o un paréntesis de apertura para un subcomando.
// No puede ser una redirección, un operador lógico o un pipe.
// g. Después de un paréntesis de cierre ()):
// El siguiente token debe ser un operador lógico, un pipe, o el final de la línea.
// No puede ser una redirección o un comando.
// 3. Verificación de paréntesis balanceados
// Durante el análisis, debes mantener un contador para verificar que los paréntesis estén balanceados. Si encuentras un (, incrementas el contador, y si encuentras un ), lo decrements. Si en algún momento el contador es negativo, significa que has encontrado un paréntesis de cierre sin su correspondiente apertura, lo que es un error de sintaxis.

// 4. Final del análisis de sintaxis
// Asegúrate de que no haya tokens sobrantes o no válidos al final. Por ejemplo:
// No puede haber un operador lógico, un pipe o una redirección al final de la línea.
// No puede haber un comando vacío (por ejemplo, después de un pipe sin comando).
// 5. Flujo de validación:
// Aquí tienes el flujo general que seguirías para validar cada token:

// Si es el primer token: asegúrate de que no sea un operador lógico, pipe o redirección.
// Si es un comando: permite que vengan operadores lógicos, redirecciones, pipes, o paréntesis.
// Si es un operador lógico: asegúrate de que el siguiente token sea un comando o paréntesis.
// Si es un pipe: asegúrate de que el siguiente token sea un comando o paréntesis.
// Si es una redirección: asegúrate de que el siguiente token sea una ruta de archivo.
// Si es un paréntesis de apertura: asegúrate de que el siguiente token sea un comando o un paréntesis.
// Si es un paréntesis de cierre: asegúrate de que el siguiente token sea un operador lógico, pipe, o el final de la línea.
// 6. Errores específicos
// Si encuentras un error en alguna de las validaciones, puedes usar perror() o mensajes personalizados para describir qué salió mal. Algunos ejemplos:

// "Error de sintaxis: operador lógico inesperado" si un operador lógico está mal ubicado.
// "Error de sintaxis: redirección mal ubicada" si una redirección está en un lugar no válido.
// "Error de sintaxis: paréntesis desbalanceados" si los paréntesis no están correctamente emparejados.
// 7. Ejemplo de flujo de análisis:
// c
// Copiar
// Editar
// void validate_syntax(char **tokens) {
//     int i = 0;
//     int parentheses_count = 0;

//     while (tokens[i]) {
//         if (i == 0) {
//             // El primer token no puede ser un operador lógico, pipe, o redirección
//             if (is_operator(tokens[i]) || is_pipe(tokens[i]) || is_redirection(tokens[i])) {
//                 perror("Error: Operador lógico, pipe o redirección al inicio");
//                 return;
//             }
//         } else {
//             if (is_operator(tokens[i - 1])) {
//                 // Si el token anterior es un operador lógico, el actual debe ser un comando
//                 if (!is_command(tokens[i]) && !is_open_parenthesis(tokens[i])) {
//                     perror("Error: Comando esperado después de operador lógico");
//                     return;
//                 }
//             } else if (is_pipe(tokens[i - 1])) {
//                 // Si el token anterior es un pipe, el actual debe ser un comando
//                 if (!is_command(tokens[i]) && !is_open_parenthesis(tokens[i])) {
//                     perror("Error: Comando esperado después de pipe");
//                     return;
//                 }
//             } else if (is_redirection(tokens[i - 1])) {
//                 // Si el token anterior es una redirección, el actual debe ser un archivo
//                 if (!is_filename(tokens[i])) {
//                     perror("Error: Se espera un archivo después de redirección");
//                     return;
//                 }
//             }
//         }

//         // Manejo de paréntesis balanceados
//         if (is_open_parenthesis(tokens[i])) parentheses_count++;
//         if (is_close_parenthesis(tokens[i])) parentheses_count--;

//         if (parentheses_count < 0) {
//             perror("Error: Paréntesis desbalanceados");
//             return;
//         }

//         i++;
//     }

//     if (parentheses_count != 0) {
//         perror("Error: Paréntesis desbalanceados");
//         return;
//     }
// }

// BRACES

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>

// typedef struct s_arg {
//     char *value;
// } t_arg;

// typedef struct s_list {
//     void *data;
//     struct s_list *next;
// } t_list;

// t_list *new_list(void *data) {
//     t_list *node = malloc(sizeof(t_list));
//     if (!node)
//         return NULL;
//     node->data = data;
//     node->next = NULL;
//     return node;
// }

// void append_to_list(t_list **list, void *data) {
//     t_list *node = new_list(data);
//     if (!*list) {
//         *list = node;
//     } else {
//         t_list *tmp = *list;
//         while (tmp->next)
//             tmp = tmp->next;
//         tmp->next = node;
//     }
// }

// // Función para dividir una cadena por comas (como {a,b,c})
// char **split_by_comma(const char *str) {
//     int count = 1;
//     for (const char *p = str; *p; p++) {
//         if (*p == ',') count++;
//     }

//     char **result = malloc((count + 1) * sizeof(char *));
//     if (!result) return NULL;

//     int i = 0;
//     const char *start = str;
//     for (const char *p = str; ; p++) {
//         if (*p == ',' || *p == '\0') {
//             int len = p - start;
//             result[i] = malloc(len + 1);
//             strncpy(result[i], start, len);
//             result[i][len] = '\0';
//             i++;
//             if (*p == '\0') break;
//             start = p + 1;
//         }
//     }
//     result[i] = NULL;
//     return result;
// }

// // Función para expandir secuencias como {a..z} o {0..9}
// t_list *expand_sequence(const char *start, const char *end, int step) {
//     t_list *result = NULL;
//     if (isalpha(start[0]) && isalpha(end[0])) {  // Expansión de letras
//         for (char c = start[0]; c <= end[0]; c += step) {
//             char *s = malloc(2);
//             s[0] = c;
//             s[1] = '\0';
//             append_to_list(&result, s);
//         }
//     } else if (isdigit(start[0]) && isdigit(end[0])) {  // Expansión de números
//         for (int i = atoi(start); i <= atoi(end); i += step) {
//             char *s = malloc(20);
//             sprintf(s, "%d", i);
//             append_to_list(&result, s);
//         }
//     }
//     return result;
// }

// // Función que maneja la expansión de una sección entre llaves
// t_list *expand_brace_segment(char *segment) {
//     t_list *result = NULL;
//     if (strchr(segment, '..')) {  // Expansión de secuencia {a..z} o {0..9}
//         char *start = strtok(segment, "..");
//         char *end = strtok(NULL, "..");
//         int step = 1;
//         if (strtok(NULL, "..")) {  // Si hay un tercer argumento es el paso
//             step = atoi(strtok(NULL, ".."));
//         }
//         result = expand_sequence(start, end, step);
//     } else {  // Expansión de comas {a,b,c}
//         char **expanded = split_by_comma(segment);
//         for (int i = 0; expanded[i] != NULL; i++) {
//             append_to_list(&result, expanded[i]);
//             free(expanded[i]);
//         }
//         free(expanded);
//     }
//     return result;
// }

// // Función que maneja el globo general de los braces
// t_list *expand_braces(t_arg *arg) {
//     t_list *expansions = NULL;
//     char *pattern = arg->value;
//     char *start = pattern;
//     while (*start) {
//         if (*start == '{') {
//             char *end = strchr(start, '}');
//             if (!end) break;

//             char *brace_content = strndup(start + 1, end - start - 1);
//             t_list *brace_expansion = expand_brace_segment(brace_content);
//             free(brace_content);

//             // Expandir el patrón anterior y posterior
//             char *pre = strndup(pattern, start - pattern);
//             append_to_list(&expansions, pre);
//             for (t_list *node = brace_expansion; node; node = node->next) {
//                 append_to_list(&expansions, node->data);
//             }

//             char *post = strdup(end + 1);
//             if (*post) {
//                 t_arg post_arg = { post };
//                 t_list *post_expansion = expand_braces(&post_arg);
//                 for (t_list *node = post_expansion; node; node = node->next) {
//                     append_to_list(&expansions, node->data);
//                 }
//                 free(post);
//             }
//             free(pre);
//             start = end + 1;
//         } else {
//             char *end = strchr(start, '{');
//             if (!end) {
//                 append_to_list(&expansions, strdup(start));
//                 break;
//             } else {
//                 int len = end - start;
//                 char *no_braces = strndup(start, len);
//                 append_to_list(&expansions, no_braces);
//                 start = end;
//             }
//         }
//     }
//     return expansions;
// }

// // Función principal de prueba
// int main() {
//     t_arg arg = { "{a..z..2}popo{a,{0..9},c}" };
//     t_list *expansions = expand_braces(&arg);

//     // Imprimir todas las expansiones generadas
//     for (t_list *node = expansions; node; node = node->next) {
//         printf("%s\n", (char *)node->data);
//     }

//     return 0;
// }
