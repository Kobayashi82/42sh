/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:31:12 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/25 18:30:48 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/lexer.h"
#include "utils/libft.h"

// REDIRECTION
//
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

t_token *redirection() {
	t_token *token = NULL;
	char	c = peek(0);

	if (c == '<' && peek(1) == '<' && peek(2) == '<') {
		advance(3);
		return (lexer_token_create(TOKEN_REDIRECT_HERESTRING, NULL, is_space(-4), is_space(0)));
	}
	if (c == '<' && peek(1) == '<') {
		advance(2);
		return (lexer_token_create(TOKEN_REDIRECT_HEREDOC, NULL, is_space(-3), is_space(0)));
	}
	if (c == '<' && peek(1) == '>') {
		advance(2);
		return (lexer_token_create(TOKEN_REDIRECT_IN_OUT, NULL, is_space(-3), is_space(0)));
	}
	if (c == '<' && peek(1) == '&') {
		advance(2);
		return (lexer_token_create(TOKEN_REDIRECT_DUP_IN, NULL, is_space(-3), is_space(0)));
	}
	if (c == '<') {
		advance(1);
		return (lexer_token_create(TOKEN_REDIRECT_IN, NULL, is_space(-2), is_space(0)));
	}

	if (c == '>' && peek(1) == '>') {
		advance(2);
		return (lexer_token_create(TOKEN_REDIRECT_APPEND, NULL, is_space(-3), is_space(0)));
	}
	if (c == '>' && peek(1) == '|') {
		advance(2);
		return (lexer_token_create(TOKEN_REDIRECT_FORCE_OUT, NULL, is_space(-3), is_space(0)));
	}
	if (c == '>' && peek(1) == '&') {
		advance(2);
		return (lexer_token_create(TOKEN_REDIRECT_DUP_OUT, NULL, is_space(-3), is_space(0)));
	}
	if (c == '>') {
		advance(1);
		return (lexer_token_create(TOKEN_REDIRECT_OUT, NULL, is_space(-2), is_space(0)));
	}

	if (c == '&' && peek(1) == '>' && peek(1) == '>') {
		advance(3);
		return (lexer_token_create(TOKEN_REDIRECT_APPEND_ALL, NULL, is_space(-4), is_space(0)));
	}
	if (c == '&' && peek(1) == '>') {
		advance(2);
		return (lexer_token_create(TOKEN_REDIRECT_OUT_ALL, NULL, is_space(-3), is_space(0)));
	}

	if (c == '|' && peek(1) == '&') {
		advance(2);
		return (lexer_token_create(TOKEN_REDIRECT_PIPE_ALL, NULL, is_space(-3), is_space(0)));
	}
	if (c == '|') {
		advance(1);
		return (lexer_token_create(TOKEN_REDIRECT_PIPE, NULL, is_space(-2), is_space(0)));
	}

	return (token);
}
