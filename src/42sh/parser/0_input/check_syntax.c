/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:45:33 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/05 10:54:51 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	numeros negativos en aritmetica

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/print.h"
	#include "parser/input.h"
	#include "main/shell.h"
	#include "main/project.h"

#pragma endregion

#pragma region "Variables"

	typedef enum e_syntax_error {
		IN_TOKEN,
		IN_TOKEN_EOF,
		TOKEN_NEAR,
		ARGS_ARITHMETIC,
		ARGS_SUBSHELL,
	} t_syntax_error;

#pragma endregion

#pragma region "Is Separator"
	
	bool is_separator(const char *input, size_t *i, char *last_token) {
			// Check for 2-character operators
		if (!ft_strncmp(&input[*i], "&&", 2) ||		// Logical AND
			!ft_strncmp(&input[*i], "||", 2) ||		// Logical OR
			!ft_strncmp(&input[*i], "$(", 2) ||		// Command Substitution
			!ft_strncmp(&input[*i], "${", 2) || 	// Parameter Expansion
			!ft_strncmp(&input[*i], "{ ", 2)) { 	// Command Group
			ft_strncpy(last_token, &input[*i], 2); last_token[2] = '\0';
			*i += 1;
			return (true);
		}	// Check for 1-character operators
		else if (ft_strchr(";|&\n", input[*i])) {
			ft_strncpy(last_token, &input[*i], 1); last_token[1] = '\0';
			return (true);
		}

		last_token[0] = '\0';
		return (false);
	}

	static bool is_separator_arithmetic(const char *input, size_t *i, int *nvalue) {
			// Check for 3-character operators
		if (!ft_strncmp(&input[*i], "**=", 2) ||		// Power assignment
			!ft_strncmp(&input[*i], "<<=", 3) ||		// Left shift with assignment
			!ft_strncmp(&input[*i], ">>=", 3)) {		// Right shift with assignment
			*i += 3; *nvalue = 2; return (true);
		}	// Check for 2-character operators
		else if (!ft_strncmp(&input[*i], "&&", 2) ||	// Logical AND
			!ft_strncmp(&input[*i], "||", 2) ||			// Logical OR
			!ft_strncmp(&input[*i], "**", 2) ||			// Power
			!ft_strncmp(&input[*i], "<<", 2) || 		// Left Shift
			!ft_strncmp(&input[*i], ">>", 2) || 		// Right Shift
			!ft_strncmp(&input[*i], "==", 2) || 		// Equality
			!ft_strncmp(&input[*i], "!=", 2) || 		// Inequality
			!ft_strncmp(&input[*i], "<=", 2) || 		// Less than or equal
			!ft_strncmp(&input[*i], ">=", 2) || 		// Greater then or equal
			!ft_strncmp(&input[*i], "+=", 2) || 		// Addition assignment
			!ft_strncmp(&input[*i], "-=", 2) || 		// Subtraction assignment
			!ft_strncmp(&input[*i], "*=", 2) || 		// Multiplication assignment
			!ft_strncmp(&input[*i], "/=", 2) || 		// Division assignment
			!ft_strncmp(&input[*i], "%=", 2) || 		// Modulo assignment
			!ft_strncmp(&input[*i], "&=", 2) || 		// Bitwise AND assignment
			!ft_strncmp(&input[*i], "|=", 2) || 		// Bitwise OR assignment
			!ft_strncmp(&input[*i], "^=", 2)) { 		// Bitwise XOR assignment
			*i += 2; *nvalue = 2; return (true);
		}	// Check for 1-character operators
		else if (ft_strchr("|&+-*/%=<>!^", input[*i])) {			
			*i += 1; *nvalue = 2; return (true);
		}

		return (false);
	}

	bool is_not_separator_arithmetic(const char *input, size_t *i) {
		if (ft_strchr("|&+-*/%=<>!^", input[*i]) || ft_isspace(input[*i])) {
			return (true);
		}

		return (false);
	}

	bool is_not_separator(char c) {
		return (c == '$' || c == '`' || c == '(' || c == ')' || c == '\'' || c == '"' || c == '{' || c == '}' || c == ';' || c == '&' || c == '|' || c == '\n' || ft_isspace(c));
	}

	bool is_end_arguments(const char *input, size_t i) {
		return (input[i] == '(' || input[i] == ')' || (input[i] == '{' && ft_isspace(input[i + 1])) || input[i] == '}' || input[i] == '\n' || ft_isspace(input[i]));
	}

#pragma endregion

#pragma region "Is Arithmetic"

	bool is_arithmetic(const char *input) {
		size_t	i = 0;
		int		parenthesis = 0;
		bool	in_quotes = false, in_dquotes = false, escape = false;

		while (input[i] && parenthesis >= 0) {
			// Manejar escapes
			if (escape)								{ escape = false;			i++; continue; }
			if (input[i] == '\\' && !in_quotes)		{ escape = true;			i++; continue; }

			// Manejar comillas
			if (input[i] == '\'' && !in_dquotes)	{ in_quotes  = !in_quotes;	i++; continue; }
			if (input[i] == '"'  && !in_quotes)		{ in_dquotes = !in_dquotes;	i++; continue; }

			if (input[i] == ')' && input[i + 1] == ')' && !parenthesis) return (true);
			if (input[i] == '(') parenthesis++;
			if (input[i] == ')') parenthesis--;
			i++;
	}

	if (!input[i]) return (true);

	return (false);
}

#pragma endregion

#pragma region "Syntax"

	#pragma region "Error"

		void syntax_error(t_syntax_error error, char *value, int line) {
			char *pname = ft_strjoin(PROYECTNAME, ": ", 0);
			if (shell.as_argument) pname = ft_strjoin(pname, "-c: ", 1);
			if (!shell.interactive && line >= 0) { 
				pname = ft_strjoin_sep(pname, "line ", ft_itoa(line), 6);
				pname = ft_strjoin(pname, ": ", 1);
			}

			if (value) value = ft_strjoin_sep("`", value, "'", 2);

			print(STDERR_FILENO, NULL, RESET);

			if (error == IN_TOKEN)
				print(STDERR_FILENO, ft_strjoin(pname, "syntax error: unexpected end of file", 0), FREE_JOIN);
			if (error == IN_TOKEN_EOF)
				print(STDERR_FILENO, ft_strjoin_sep(pname, "unexpected EOF while looking for matching ", value, 0), FREE_JOIN);
			if (error == TOKEN_NEAR)
				print(STDERR_FILENO, ft_strjoin_sep(pname, "syntax error near unexpected token ", value, 0), FREE_JOIN);
			if (error == ARGS_ARITHMETIC)
				print(STDERR_FILENO, ft_strjoin(pname, "syntax error: invalid arithmetic expression ", 0), FREE_JOIN);
			if (error == ARGS_SUBSHELL)
				print(STDERR_FILENO, ft_strjoin(pname, "syntax error: invalid subshell ", 0), FREE_JOIN);

			print(STDERR_FILENO, "\n", PRINT);
			sfree(pname); sfree(value);
		}

	#pragma endregion

	// comillas en operaciones aritmeticas se ignoran

	//	Comprobar si {a,b,c} es válido, si no, no lo cuenta como stack
	//	Comprobar si { cmd; } es válido, si no, no lo cuenta como stack
	//	Básicamente {   } se considera literal
	//	Saber cuando es comando o argumento

	static int syntax_arithmetic(const char *input, size_t *i, t_context *context, char *last_token, int *line) {
		if (!input || !*input || !context) return (0);
		if (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP)) return (0);
		
		int result = 0;
		if (!context->stack->nvalue) context->stack->nvalue = 2;

		while (input[*i]) {
				//	\	Handle Escape
			if (context->in_escape) {
				if (input[*i + 1]) context->stack->nvalue = 1;
				context->in_escape = false; *i += 1; continue;
			} else if (input[*i] == '\\' && (!context->stack || context->stack->type != CTX_QUOTE)) {
				ft_strcpy(last_token, "\\");
				context->in_escape = true; *i += 1; continue;
			}
			
			//		Handle Spaces
			if (input[*i] == '\n') *line += 1;
			if (input[*i] != '\n' && ft_isspace(input[*i])) { *i += 1; continue; }
			if (input[*i] != '"') { *i += 1; continue; }

			if (context->stack->nvalue == 2 && is_separator_arithmetic(input, i, &context->stack->nvalue)) {
				return (syntax_error(TOKEN_NEAR, ft_strdup("p"), *line), 2);
			}
			else if (is_separator_arithmetic(input, i, &context->stack->nvalue)) {
				continue;
			}
			
				//	))	Close Arithmetic Expansion or Arithmetic Expression
			if (!ft_strncmp(&input[*i], "))", 2) && context->stack && context->stack->type == CTX_ARITHMETIC) {
				if (context->stack->nvalue == 2) return (syntax_error(TOKEN_NEAR, ft_strdup("end with operator"), *line), 2);
				stack_pop(&context->stack); *i += 2;
				if (context->stack) context->stack->nvalue = 1;
				return(0);
			}	//	)	Close Command Substitution or Subshell or Arithmetic Group
			else if (input[*i] == ')' && context->stack && context->stack->type == CTX_ARITHMETIC_GROUP) {
				if (context->stack->nvalue == 2) return (syntax_error(TOKEN_NEAR, ft_strdup("end with operator"), *line), 2);
				stack_pop(&context->stack); *i += 1;
				if (context->stack) context->stack->nvalue = 1;
				return(0);
			}

				//	$((	Open Arithmetic Expansion
			else if (!ft_strncmp(&input[*i], "$((", 3) && is_arithmetic(&input[*i + 3])) {
				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
				stack_push(&context->stack, CTX_ARITHMETIC);
				*i += 3; continue;
			}	//	((	Open Arithmetic Expression
			else if (!ft_strncmp(&input[*i], "((", 2) && (!context->stack || context->stack->type != CTX_DQUOTE) && is_arithmetic(&input[*i + 2])) {
				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
				stack_push(&context->stack, CTX_ARITHMETIC);
				*i += 2;
				if ((result = syntax_arithmetic(input, i, context, last_token, line))) return (result);
				continue;
			}	//	(	Open Arithmetic Group
			else if (input[*i] == '(') {
				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
				stack_push(&context->stack, CTX_ARITHMETIC_GROUP);
				*i += 1;
				if ((result = syntax_arithmetic(input, i, context, last_token, line))) return (result);
				continue;
			}	//	$(	Open Command Substitution
			else if (!ft_strncmp(&input[*i], "$(", 2)) {
				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
				stack_push(&context->stack, CTX_SUBSHELL);
				*i += 2; continue;
			}	//	`	Open Backtick
			else if (input[*i] == '`' && !is_context(context->stack, CTX_BACKTICK)) {
				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
				stack_push(&context->stack, CTX_BACKTICK);
				*i += 1; continue;
			}	//	${	Open Parameter Expansion
			else if (!ft_strncmp(&input[*i], "${", 2)) {
				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
				stack_push(&context->stack, CTX_BRACE_PARAM);
				*i += 2; continue;

				//	Forbidden Tokens

			}	//	<(	Open Process Substitution In/Out
			else if (!ft_strncmp(&input[*i], "<(", 2) || !ft_strncmp(&input[*i], ">(", 2)) {
				return (syntax_error(TOKEN_NEAR, ft_strdup("}"), *line), 2);
			}	//	{ 	Open Command Group
			else if (input[*i] == '{' && ft_isspace(input[*i + 1])) {
				return (syntax_error(TOKEN_NEAR, ft_strdup("}"), *line), 2);
			}	//	{	Open Brace Expansion
			else if (input[*i] == '{') {
				return (syntax_error(TOKEN_NEAR, ft_strdup("}"), *line), 2);	
			}

			if (context->stack->nvalue == 1) {
				return (syntax_error(TOKEN_NEAR, ft_strdup("two numbers"), *line), 2);
			}

			while (input[*i] && (input[*i] == '"' || !is_not_separator_arithmetic(input, i))) *i += 1;
			context->stack->nvalue = 1;
		}

		return (0);
	}


	#pragma region "Shell"
	
		static int syntax_shell(const char *input, size_t *i, t_context *context, char *last_token, int *line) {
			if (!input || !*input || !context) return (0);

			bool command_start = true, is_argument = false;
			int result = 0;
			(void) is_argument;

			while (input[*i]) {
				if (context->stack && (context->stack->type == CTX_ARITHMETIC || context->stack->type == CTX_ARITHMETIC_GROUP)) {
					if ((result = syntax_arithmetic(input, i, context, last_token, line))) return (result);
				}

					//	\	Handle Escape
				if (context->in_escape) {
					context->in_escape = false; *i += 1; continue;
				} else if (input[*i] == '\\' && (!context->stack || context->stack->type != CTX_QUOTE)) {
					ft_strcpy(last_token, "\\");
					context->in_escape = true; *i += 1; continue;
				}
		
				if (input[*i] == '\n') *line += 1;
				//	'	Handle Single Quotes
				if (context->stack && context->stack->type == CTX_QUOTE) {
					if (input[*i] == '\'') stack_pop(&context->stack);
					command_start = false; *i += 1; continue;
				} else if (input[*i] == '\'') {
					stack_push(&context->stack, CTX_QUOTE);
					command_start = false; *i += 1; continue;
				}
				
					//		Handle Spaces
				if (input[*i] != '\n' && ft_isspace(input[*i])) { *i += 1; continue; }
				
					//	}	Close Brace Expansion
				if (context->stack && context->stack->type == CTX_BRACE) {
					if (input[*i] == '}') stack_pop(&context->stack);
					command_start = false; *i += 1; continue;
				}	//	"	Close Double Quotes
				if (input[*i] == '"' && context->stack && context->stack->type == CTX_DQUOTE) {
					stack_pop(&context->stack);
					command_start = false; *i += 1; continue;
				}	//	))	Close Arithmetic Expansion or Arithmetic Expression
				else if (!ft_strncmp(&input[*i], "))", 2) && context->stack && context->stack->type == CTX_ARITHMETIC) {
					stack_pop(&context->stack);
					command_start = false; *i += 2; continue;
				}	//	)	Close Command Substitution or Subshell or Arithmetic Group
				else if (input[*i] == ')' && context->stack && (context->stack->type == CTX_SUBSHELL || context->stack->type == CTX_ARITHMETIC_GROUP || context->stack->type == CTX_PROCESS_SUB_IN || context->stack->type == CTX_PROCESS_SUB_OUT)) {
					stack_pop(&context->stack);
					command_start = false; *i += 1; continue;
				}	//	`	Close Backtick
				else if (input[*i] == '`' && is_context(context->stack, CTX_BACKTICK)) {
					while (context->stack && context->stack->type != CTX_BACKTICK) stack_pop(&context->stack);
					stack_pop(&context->stack);
					command_start = false; *i += 1; continue;
				}	//	}	Close Parameter Expansion or Group Command or Brace Expansion
				else if (input[*i] == '}' && (context->stack->type == CTX_BRACE_PARAM || context->stack->type == CTX_BRACE_COMMAND || context->stack->type == CTX_BRACE)) {
					stack_pop(&context->stack);
					command_start = false; *i += 1; continue;
				}
		
				//	"	Open Double Quotes
				if (input[*i] == '"' && (!context->stack || context->stack->type != CTX_DQUOTE)) {
					stack_push(&context->stack, CTX_DQUOTE);
					command_start = false; *i += 1; continue;
				}	//	$((	Open Arithmetic Expansion
				else if (!ft_strncmp(&input[*i], "$((", 3) && is_arithmetic(&input[*i + 3])) {
					stack_push(&context->stack, CTX_ARITHMETIC);
					command_start = false; *i += 3;
					if ((result = syntax_arithmetic(input, i, context, last_token, line))) return (result);
					continue;
				}	//	((	Open Arithmetic Expression
				else if (!ft_strncmp(&input[*i], "((", 2) && (!context->stack || context->stack->type != CTX_DQUOTE) && is_arithmetic(&input[*i + 2])) {
					if (!command_start) return (syntax_error(ARGS_ARITHMETIC, NULL, *line), 2);
					stack_push(&context->stack, CTX_ARITHMETIC);
					command_start = false; *i += 2;
					if ((result = syntax_arithmetic(input, i, context, last_token, line))) return (result);
					continue;
				}	//	$(	Open Command Substitution
				else if (!ft_strncmp(&input[*i], "$(", 2)) {
					stack_push(&context->stack, CTX_SUBSHELL);
					command_start = true; *i += 2; continue;
				}	//	<(	Open Process Substitution In
				else if (!ft_strncmp(&input[*i], "<(", 2)) {
					stack_push(&context->stack, CTX_PROCESS_SUB_IN);
					command_start = true; *i += 2; continue;
				}	//	>(	Open Process Substitution Out
				else if (!ft_strncmp(&input[*i], ">(", 2)) {
					stack_push(&context->stack, CTX_PROCESS_SUB_OUT);
					command_start = true; *i += 2; continue;	
				}	//	(	Open Arithmetic Group
				else if (input[*i] == '(' && context->stack && (context->stack->type == CTX_ARITHMETIC || context->stack->type == CTX_ARITHMETIC_GROUP)) {
					stack_push(&context->stack, CTX_ARITHMETIC_GROUP);
					command_start = false; *i += 1;
					if ((result = syntax_arithmetic(input, i, context, last_token, line))) return (result);
					continue;
				}	//	(	Open Subshell
				else if (input[*i] == '(' && (!context->stack || (context->stack->type != CTX_DQUOTE && context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
					if (!command_start) return (syntax_error(ARGS_SUBSHELL, NULL, *line), 2);
					stack_push(&context->stack, CTX_SUBSHELL);
					command_start = true; *i += 1; continue;	
				}	//	`	Open Backtick
				else if (input[*i] == '`' && !is_context(context->stack, CTX_BACKTICK)) {
					stack_push(&context->stack, CTX_BACKTICK);
					command_start = true; *i += 1; continue;
				}	//	${	Open Parameter Expansion
				else if (!ft_strncmp(&input[*i], "${", 2)) {
					stack_push(&context->stack, CTX_BRACE_PARAM);
					command_start = false; *i += 2; continue;
				}	//	{ 	Open Group Command
				else if (input[*i] == '{' && ft_isspace(input[*i + 1]) && (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
					if (!command_start) return (syntax_error(TOKEN_NEAR, ft_strdup("}"), *line), 2);
					stack_push(&context->stack, CTX_BRACE_COMMAND);
					command_start = true; *i += 1; continue;
				}	//	{	Open Brace Expansion
				else if (input[*i] == '{' && (!context->stack || (context->stack->type != CTX_DQUOTE && context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
					stack_push(&context->stack, CTX_BRACE);
					command_start = false; *i += 1; continue;
				}	//	;	&	&&	|	||	\n	Command Separator
				else if (is_separator(input, i, last_token) && (!context->stack || (context->stack->type != CTX_DQUOTE && context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
					command_start = true; *i += 1; continue;
				}

				if (command_start && (!context->stack || (context->stack->type != CTX_QUOTE && context->stack->type != CTX_DQUOTE && context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
					size_t start = *i, end = *i;
					(void) start;
					while (input[end] && !is_not_separator(input[end])) end++;
					is_argument = !is_end_arguments(input, *i);
					is_separator(input, i, last_token);
					if (*last_token) is_argument = true;
					if (!input[end]) break;
					*i = end;
				} else *i += 1;		
				
				command_start = false;
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Check"
	
		int	syntax_check(const char *input, t_context *context, int line) {
			if (!input || !*input || !context) return (1);

			char last_token[4]; last_token[0] = '\0';
			size_t i = 0;
		
			int result = syntax_shell(input, &i, context, last_token, &line);

			context->in_token = !is_context(context->stack, CTX_QUOTE) && !is_context(context->stack, CTX_DQUOTE) && (!ft_strncmp(last_token, "&&", 2) || !ft_strncmp(last_token, "||", 2) || *last_token == '|' || *last_token == '\\');
			context->in_escape = context->in_token && *last_token == '\\';

			// si hay cosas sin cerrar, error
			if (!shell.interactive && (context->stack || context->in_token)) {
				if (context->in_token) {
					syntax_error(IN_TOKEN, NULL, line);
				} else {
					char *value = NULL;
					if (context->stack->type == CTX_QUOTE) value = ft_strdup("'");
					else if (context->stack->type == CTX_DQUOTE) value = ft_strdup("\"");
					else if (context->stack->type == CTX_BACKTICK) value = ft_strdup("`");
					else if (context->stack->type == CTX_BRACE || context->stack->type == CTX_BRACE_PARAM || context->stack->type == CTX_BRACE_COMMAND) value = ft_strdup("}");
					else value = ft_strdup(")");
					syntax_error(IN_TOKEN_EOF, value, line);
				}
				result = 2;
			}

			if (result) shell.exit_code = result;

			return (result);
		}

	#pragma endregion
	
#pragma endregion

//	En parameter expansion:

//	Primera parte literal
//	Pperado literal
//	Valor: $(()), $(), `, ${}, globbing

//	┌──────────────────────────┬──────────────────────────┬─────────────────────────────┬───────────────────────────────────────────────┐
//	│         Concepto         │         Sintaxis         │           Ejemplo           │                  Descripción                  │
//	├──────────────────────────┼──────────────────────────┼─────────────────────────────┼───────────────────────────────────────────────┤
//	│ Acceso básico            │ ${variable}              │ echo "${user}"              │ Obtiene el valor de la variable.              │
//	│ Valor predeterminado     │ ${var:-valor}            │ echo "${name:-Anónimo}"     │ Usa valor si var no está definida o vacía.    │
//	│ Asignación predeter.     │ ${var:=valor}            │ echo "${count:=0}"          │ Asigna valor a var si no existe.              │
//	│ Error si no definida     │ ${var:?mensaje}          │ echo "${file:?Error}"       │ Muestra mensaje y termina si var no existe.   │
//	│ Valor alternativo        │ ${var:+valor}            │ echo "${debug:+ON}"         │ Usa valor solo si var está definida.          │
//	│ Longitud de cadena       │ ${#var}                  │ echo "${#str}"              │ Devuelve la cantidad de caracteres en var.    │
//	│ Subcadena (offset, len)  │ ${var:inicio:longitud}   │ echo "${str:2:4}"           │ Extrae longitud caracteres desde inicio.      │
//	│ Subcadena (solo offset)  │ ${var:inicio}            │ echo "${str:5}"             │ Extrae desde inicio hasta el final.           │
//	│ Eliminar prefijo (corto) │ ${var#patrón}            │ echo "${path#*/}"           │ Elimina el prefijo más corto.                 │
//	│ Eliminar prefijo (largo) │ ${var##patrón}           │ echo "${path##*/}"          │ Elimina el prefijo más largo.                 │
//	│ Eliminar sufijo (corto)  │ ${var%patrón}            │ echo "${file%.*}"           │ Elimina el sufijo más corto.                  │
//	│ Eliminar sufijo (largo)  │ ${var%%patrón}           │ echo "${file%%.*}"          │ Elimina el sufijo más largo.                  │
//	│ Reemplazo (primero)      │ ${var/antiguo/nuevo}     │ echo "${text/foo/bar}"      │ Reemplaza la primera ocurrencia.              │
//	│ Reemplazo (todos)        │ ${var//antiguo/nuevo}    │ echo "${text//foo/bar}"     │ Reemplaza todas las ocurrencias.              │
//	│ Mayúsculas               │ ${var^^}                 │ echo "${palabra^^}"         │ Convierte toda la cadena a mayúsculas.        │
//	│ Minúsculas               │ ${var,,}                 │ echo "${PALABRA,,}"         │ Convierte toda la cadena a minúsculas.        │
//	│ Primera mayúscula        │ ${var^}                  │ echo "${palabra^}"          │ Convierte solo la primera letra.              │
//	│ Conversión selectiva     │ ${var^^patrón}           │ echo "${str,,[A-Z]}"        │ Convierte solo ciertos caracteres.            │
//	│ Expansión indirecta      │ ${!var}                  │ var=a; a=b; echo ${!var}    │ Obtiene el valor de otra variable.            │
//	│ Arreglo: elemento        │ ${array[indice]}         │ echo "${frutas[0]}"         │ Obtiene un elemento del arreglo.              │
//	│ Arreglo: todos elementos │ ${array[@]}              │ echo "${frutas[@]}"         │ Lista todos los elementos del arreglo.        │
//	│ Arreglo: longitud        │ ${#array[@]}             │ echo "${#frutas[@]}"        │ Devuelve el número de elementos.              │
//	│ Arreglo: subarreglo      │ ${array[@]:inicio:long}  │ echo "${frutas[@]:1:2}"     │ Extrae un subarreglo.                         │
//	│ Arreglo: índices         │ ${!array[@]}             │ echo "${!frutas[@]}"        │ Lista los índices del arreglo.                │
//	│ Variables con prefijo    │ ${!prefijo*}             │ echo "${!var*}"             │ Lista variables que inician con prefijo.      │
//	│ Número de argumentos     │ $#                       │ echo "Args: $#"             │ Muestra la cantidad de argumentos.            │
//	│ Todos los argumentos     │ $@                       │ echo "$@"                   │ Lista los argumentos como elementos.          │
//	│ Todos como cadena        │ $*                       │ echo "$*"                   │ Une los argumentos en una cadena.             │
//	│ Código de salida         │ $? o ${?}                │ echo "Código: $?"           │ Código de salida del último comando.          │
//	│ Expansión segura         │ ${var@Q}                 │ str="hola"; echo "${str@Q}" │ Expande con escapado seguro.                  │
//	└──────────────────────────┴──────────────────────────┴─────────────────────────────┴───────────────────────────────────────────────┘

//	Operators Begining:	!	#
//	Operators Middle:	:-	:=	:?	:+	:	#	##	%	%%	/	//	,,	^	^^	...

//	Las operaciones de eliminación de prefijo/sufijo usan sintaxis de globbing, por lo que aceptan comodines como *, ?, [a-z], [!a-z], etc.

//	Obligados
//
//	${parameter:-word}
//	${parameter:=word}
//	${parameter:?word}
//	${parameter:+word}
//	${#parameter}
//	${parameter%}
//	${parameter%%}
//	${parameter#}
//	${parameter##}
