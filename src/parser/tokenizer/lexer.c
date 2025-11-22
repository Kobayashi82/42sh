/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:15:32 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/22 15:12:28 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/lexer.h"
#include "utils/libft.h"

t_lx_token	*lx_tokens;

int lexer(const char *input) {
	t_lx_token *current = NULL;

	while (*input) {
		t_lx_token *new_token = calloc(1, sizeof(t_lx_token));

		new_token->type = LX_WORD;
		new_token->status = LX_COMPLETE;
		new_token->token = ft_strdup(input);
		if (lx_tokens) {
			new_token->prev = current;
			current->next = new_token;
			current = new_token;
		} else {
			lx_tokens = current = new_token;
		}

		input++;
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