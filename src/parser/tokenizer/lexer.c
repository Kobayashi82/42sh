/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:15:32 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/22 20:01:24 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/lexer.h"
#include "utils/libft.h"
#include <string.h>
#include <stdlib.h>

t_lx_token	*lx_tokens;

// ""		double quote
// ''		single quote
// $""		translatable string
// $''		ANSI-C quoting
// ${		parameter expression
// {		brace expansion
// {[space]	command group
// [space]}	command group close
// }		brace close

// (		subshell
// ((		arithmetic
// $(		command substitution
// $((		arithmetic substitution
// )		parenthesis close
// ))		double parenthesis close
// ``		command substitution
// &		background
// &&		and
// |		pipe
// ||		or
// ;		semicolon
// >		redirección de salida
// >>		redirección de salida (append)
// <		redirección de entrada estándar
// <<		here-document
// <<-		here-document con tabs ignorados
// <<<		here-string
// <>		abrir archivo para lectura/escritura
// <&		duplica descriptor de entrada
// >&		duplica descriptor de salida
// &>		redirige stdout y stderr
// &>>		append de stdout y stderr
// [n]>&-	redirige fd 3 a archivo o fd, o si es - lo cierra
// >|		sobrescribe incluso con noclobber activado
// #		comentario

// Esto no
// $$		PID del shell actual
// $?		exit status del último comando
// $!		PID del último proceso en background
// $#		número de argumentos
// $@, $*	todos los argumentos
// $0		nombre del script
// $1...	argumentos posicionales
// $var		expansión simple de variable

// Palabras reservadas (keywords)
// No son operadores pero tu lexer probablemente necesita reconocerlos:

// Operadores de control:

// ;; → fin de caso en case statements
// ;& → fallthrough en case (bash 4+)
// ;;& → continuar testeando casos (bash 4+)
// ! → negación de pipeline

// if, then, else, elif, fi
// case, esac, in
// while, until, do, done
// for, select
// function
// time
// ! (cuando es keyword, no operador)
// [[, ]] (test compuesto en bash)
// coproc (bash)

// Check if a character needs nesting tracking
// static int	is_opening_bracket(char c) {
// 	return (c == '(' || c == '{' || c == '[' || c == '"' || c == '\'' || c == '`');
// }

// static int	matching_close(char open) {
// 	if (open == '(') return (')');
// 	if (open == '{') return ('}');
// 	if (open == '[') return (']');

// 	return (open);
// }

// Extract a complex expansion like $() ${{}} ${} etc
static char	*extract_expansion(const char *input, int *i) {
	int		start = *i;
	int		j = start;
	int		depth = 0;
	char	quote = 0;
	bool	escaped = false;

	while (input[j]) {
		char c = input[j];

		if (escaped) {
			escaped = false;
			j++; continue;
		}

		if (!quote && c == '\\') {
			escaped = true;
			j++; continue;
		}

		// Handle quotes
		if (!quote && (c == '"' || c == '\'' || c == '`')) {
			quote = c;
			j++; continue;
		}
		if (quote && c == quote) {
			quote = 0;
			j++; continue;
		}

		if (quote) {
			j++; continue;
		}

		// Track nesting
		if (c == '(' || c == '{' || c == '[')
			depth++;
		else if (c == ')' || c == '}' || c == ']') {
			depth--;
			if (depth < 0) break;
			if (depth == 0 && (c == ')' || c == '}' || c == ']')) { j++; break; }
		}

		j++;
	}

	if (j > start) {
		char *expansion = malloc(j - start + 1);
		if (!expansion) return (NULL);
		strncpy(expansion, &input[start], j - start);
		expansion[j - start] = '\0';
		*i = j - 1;
		return (expansion);
	}

	return (NULL);
}

static t_lx_type	get_token_at_pos(const char *input, int *i, char **token_out) {
	int		curr = *i;
	char	c = input[curr];
	char	next = input[curr + 1];
	char	next2 = input[curr + 2];

	// $ expansions
	if (c == '$') {
		if (next == '(') {
			if (next2 == '(')			{ *token_out = extract_expansion(input, i);			return (LX_CMD_ARIT);			}	// $(( ... )) - arithmetic substitution
			else						{ *token_out = extract_expansion(input, i);			return (LX_CMD_SHELL);			}	// $( ... ) - command substitution
		}
		else if (next == '{')			{ *token_out = extract_expansion(input, i);			return (LX_VAR);				}	// ${ ... } - parameter expansion
	}

	// { } constructs
	if (c == '{') {
		if (next == ' ' || next == '\t' || next == '\n') { *token_out = ft_strdup("{");		return (LX_BRACE_GROUP);		}	// Check if it's a command group: { [space]
		else if (next && next != '}')	{ *token_out = extract_expansion(input, i);			return (LX_BRACE);				}	// Otherwise it's brace expansion: {content}
	}
	if (c == '}')						{ *token_out = ft_strdup("}");						return (LX_BRACE);				}	// Handle closing brace

	if (c == '(' && next == '(')		{ (*i)++; *token_out = extract_expansion(input, i);	return (LX_CMD_ARIT);			}	// Handle (( )) - arithmetic

	if (c == '[' && next == '[')		{ (*i)++; *token_out = extract_expansion(input, i);	return (LX_SHELL);				}	// Handle [[ ]] - conditional

	// Standard operators
	if (c == '&' && next == '&')		{ (*i)++; *token_out = ft_strdup("&&");				return (LX_AND);				}
	if (c == '|' && next == '|')		{ (*i)++; *token_out = ft_strdup("||");				return (LX_OR);					}
	if (c == '|')						{ *token_out = ft_strdup("|");						return (LX_PIPE);				}
	if (c == ';')						{ *token_out = ft_strdup(";");						return (LX_SEMICOLON);			}

	if (c == '>' && next == '>')		{ (*i)++; *token_out = ft_strdup(">>");				return (LX_REDIR_APPEND);		}
	if (c == '>' && next == '|')		{ (*i)++; *token_out = ft_strdup(">|");				return (LX_REDIR_OUT);			}
	if (c == '>')						{ *token_out = ft_strdup(">");						return (LX_REDIR_OUT);			}
	if (c == '<' && next == '<')		{ (*i)++;
		if (next2 == '<')				{ (*i)++; *token_out = ft_strdup("<<<");			return (LX_REDIR_HERESTRING);	}
		else							{ *token_out = ft_strdup("<<");						return (LX_REDIR_HEREDOC);		}
	}
	if (c == '<')						{ *token_out = ft_strdup("<");						return (LX_REDIR_IN);			}
	if (c == ')')						{ *token_out = ft_strdup(")");						return (LX_ARIT);				}

	return (LX_WORD);
}

static char	*extract_word(const char *input, int *i) {
	int		start = *i;
	int		j = start;
	char	quote = 0;
	bool	escaped = false;

	while (input[j]) {
		char c = input[j];

		// Handle escape sequences
		if (escaped) {
			escaped = false;
			j++; continue;
		}
		if (!quote && c == '\\') {
			escaped = true;
			j++; continue;
		}

		// Handle quotes
		if (!quote && (c == '"' || c == '\'' || c == '`')) {
			quote = c;
			j++; continue;
		}
		if (quote == c) {
			quote = 0;
			j++; continue;
		}

		// If we're in quotes, continue
		if (quote) { j++; continue; }

		// Handle $ in word context (will be expanded later)
		if (c == '$' && j + 1 < (int)ft_strlen(input)) {
			char next = input[j + 1];
			// If it's a complex expansion, stop here
			if (next == '(' || next == '{' || next == '[') break;
			// Otherwise it's part of the word ($var, $1, etc)
			j++; continue;
		}

		// Outside quotes: stop at operators or whitespace
		if (c == ' ' || c == '\t' || c == '\n' || 
		    c == '|' || c == '&' || c == ';' || 
		    c == '>' || c == '<' || c == '(' || c == ')' || 
		    c == '{' || c == '}' || c == '[' || c == ']')
			break;

		j++;
	}

	if (j > start) {
		char *word = malloc(j - start + 1);
		if (!word) return (NULL);
		strncpy(word, &input[start], j - start);
		word[j - start] = '\0';
		*i = j - 1;

		return (word);
	}

	return (NULL);
}

int lexer(const char *input) {
	t_lx_token	*current = NULL;
	t_lx_type	type;
	char		*token_str;
	int			i = 0;

	if (!input) return (LX_FAILED);

	while (input[i]) {
		t_lx_token	*new_token;
		char		c = input[i];

		// Skip whitespace
		if (c == ' ' || c == '\t' || c == '\n') { i++; continue; }

		// Create new token
		new_token = calloc(1, sizeof(t_lx_token));
		if (!new_token) return (LX_FAILED);

		// Get token type and content
		type = get_token_at_pos(input, &i, &token_str);

		if (type != LX_WORD && token_str == NULL) {
			// Simple operator, create the token string
			token_str = malloc(2);
			if (!token_str) {
				free(new_token);
				return (LX_FAILED);
			}
			token_str[0] = input[i];
			token_str[1] = '\0';
		} else if (type == LX_WORD && token_str == NULL) {
			token_str = extract_word(input, &i);
			if (!token_str) {
				free(new_token);
				i++; continue;
			}
		}

		new_token->status = LX_COMPLETE;
		new_token->type = type;
		new_token->token = token_str;
		new_token->prev = current;
		new_token->next = NULL;

		// Add to linked list
		if (!lx_tokens)	lx_tokens = new_token;
		else			current->next = new_token;
		current = new_token;

		i++;
	}

	return (LX_COMPLETE);
}

void lexer_print() {
	t_lx_token *token = lx_tokens;
	while (token) {
		printf("[%d] %s - %d\n", token->type, token->token, token->status);
		token = token->next;
	}
}

		// std::string Utils::environment_expand(std::map<std::string, std::string>& env, const std::string& line) {
		// 	std::string	result;
		// 	char		quoteChar = 0;
		// 	bool		escaped = false;

		// 	for (size_t i = 0; i < line.length(); ++i) {
		// 		char c = line[i];

		// 		if (escaped)								{ escaped = false;	result += c;	continue; }
		// 		if (quoteChar != '\'' && c == '\\')			{ escaped = true;	result += c;	continue; }
		// 		if (!quoteChar && (c == '"' || c == '\''))	{ quoteChar = c;	result += c;	continue; }
		// 		if (quoteChar && c == quoteChar)			{ quoteChar = 0;	result += c;	continue; }

		// 		if (quoteChar != '\'' && c == '$') {
		// 			if (i + 1 < line.length() && line[i + 1] == '{') {							// ${VAR}
		// 				size_t	start = i + 2;
		// 				size_t	end = start;

		// 				while (end < line.length() && line[end] != '}') ++end;

		// 				if (end < line.length()) {
		// 					std::string var_expr = line.substr(start, end - start);
		// 					result += environment_expand_expr(env, var_expr); 
		// 					i = end;															continue;
		// 				}
		// 			} else {																	// $VAR
		// 				size_t	start = i + 1;
		// 				size_t	end = start;

		// 				while (end < line.length() && (isalnum(static_cast<unsigned char>(line[end])) || line[end] == '_')) ++end;

		// 				if (end > start) {
		// 					std::string var_name = line.substr(start, end - start);
		// 					auto it = env.find(var_name);
		// 					if (it != env.end()) result += it->second;
		// 					i = end - 1; continue;
		// 				}
		// 			}
		// 		}

		// 		result += c;
		// 	}

		// 	if (quoteChar || escaped) throw std::runtime_error("unclosed quote or unfinished escape sequence");

		// 	return (result);
		// }