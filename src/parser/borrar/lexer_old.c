/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:15:32 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/23 11:33:16 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/lexer.h"
#include "utils/libft.h"

t_lx_token	*lx_tokens;


// Esto no
// $$		PID del shell actual
// $?		exit status del último comando
// $!		PID del último proceso en background
// $#		número de argumentos
// $@, $*	todos los argumentos
// $0		nombre del script
// $1...	argumentos posicionales
// $VAR		expansión simple de variable


// Extract a complex expansion like $() ${{}} ${} etc
static char	*extract_expansion(const char *input, int *i) {
	int		start = *i;
	int		j = start;
	int		paren_depth = 0;
	int		brace_depth = 0;
	int		bracket_depth = 0;
	char	quote = 0;
	bool	escaped = false;
	char	opening = input[j];

	// Determine what we're expanding
	if (opening == '$' && j + 1 < (int)ft_strlen(input)) {
		char next = input[j + 1];
		if (next == '(') {
			j += 2;
			paren_depth = 1;
			opening = '(';
		} else if (next == '{') {
			j += 2;
			brace_depth = 1;
			opening = '{';
		}
	} else if (opening == '(') {
		j++;
		paren_depth = 1;
	} else if (opening == '{') {
		j++;
		brace_depth = 1;
	} else if (opening == '[') {
		j++;
		bracket_depth = 1;
	}

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

		// Track nesting for each bracket type separately
		if (c == '(')			paren_depth++;
		else if (c == ')')		{ paren_depth--; if (paren_depth < 0) paren_depth = 0; }
		else if (c == '{')		brace_depth++;
		else if (c == '}')		{ 
			if (brace_depth > 0) brace_depth--;
			// Check if we should stop here
			if (brace_depth == 0 && paren_depth == 0 && bracket_depth == 0 && opening == '{') {
				j++;
				break;
			}
		}
		else if (c == '[')		bracket_depth++;
		else if (c == ']')		{ bracket_depth--; if (bracket_depth < 0) bracket_depth = 0; }

		// Check if we've closed all brackets for non-brace openings
		if (opening != '{' && paren_depth == 0 && brace_depth == 0 && bracket_depth == 0) {
			j++;
			break;
		}

		j++;
	}

	if (j > start) {
		char *expansion = malloc(j - start + 1);
		if (!expansion) return (NULL);
		strncpy(expansion, &input[start], j - start);
		expansion[j - start] = '\0';
		// Ensure j doesn't go beyond string bounds
		int len = ft_strlen(input);
		if (j > len) j = len;
		if (j > 0) *i = j - 1;
		else *i = 0;
		return (expansion);
	}

	return (NULL);
}

static t_lx_type	get_token_at_pos(const char *input, int *i, char **token_out) {
	int		curr = *i;
	char	c = input[curr];
	char	next = (input[curr + 1] != '\0') ? input[curr + 1] : '\0';
	char	next2 = (input[curr + 2] != '\0') ? input[curr + 2] : '\0';

	// $ expansions
	if (c == '$') {
		if (next == '(') {
			if (next2 == '(')			{ (*i)++; *token_out = extract_expansion(input, i);	return (LX_CMD_ARIT);			}	// $(( ... )) - arithmetic substitution
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
	if (c == '(')						{ *token_out = ft_strdup("(");						return (LX_CMD_SHELL);			}	// Handle ( - subshell

	if (c == '[' && next == '[')		{ (*i)++; *token_out = extract_expansion(input, i);	return (LX_SHELL);				}	// Handle [[ ]] - conditional

	// Standard operators
	if (c == '&' && next == '&')		{ (*i)++; *token_out = ft_strdup("&&");				return (LX_AND);				}
	if (c == '&')						{ *token_out = ft_strdup("&");						return (LX_BACKGROND);			}	// background
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
		if (quote && c == quote) {
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

		// Outside quotes: stop at operators or whitespace (only if not escaped)
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
		token_str = NULL;
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
	const char *type_names[] = {
		"WORD", "PIPE", "BACKGROND", "AND", "OR", "BRACE", "BRACE_GROUP", "VAR", "CMD_SHELL",
		"SHELL", "CMD_ARIT", "ARIT", "SEMICOLON", "REDIR_HEREDOC", "REDIR_IN",
		"REDIR_OUT", "REDIR_APPEND", "REDIR_HERESTRING"
	};
	while (token) {
		printf("[%s] %s\n", type_names[token->type], token->token);
		token = token->next;
	}
}
