/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:31:12 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 14:25:30 by vzurera-         ###   ########.fr       */
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
	size_t	start = lexer.input->position;
	char	c = peek(0);

	if (c == '<' && peek(1) == '<' && peek(2) == '<') {
		advance(3);
		return (token_create(TOKEN_REDIRECT_HERESTRING, start));
	}
	if (c == '<' && peek(1) == '<') {
		advance(2);
		return (token_create(TOKEN_REDIRECT_HEREDOC, start));
	}
	if (c == '<' && peek(1) == '>') {
		advance(2);
		return (token_create(TOKEN_REDIRECT_IN_OUT, start));
	}
	if (c == '<' && peek(1) == '&') {
		advance(2);
		return (token_create(TOKEN_REDIRECT_DUP_IN, start));
	}
	if (c == '<') {
		advance(1);
		return (token_create(TOKEN_REDIRECT_IN, start));
	}

	if (c == '>' && peek(1) == '>') {
		advance(2);
		return (token_create(TOKEN_REDIRECT_APPEND, start));
	}
	if (c == '>' && peek(1) == '|') {
		advance(2);
		return (token_create(TOKEN_REDIRECT_FORCE_OUT, start));
	}
	if (c == '>' && peek(1) == '&') {
		advance(2);
		return (token_create(TOKEN_REDIRECT_DUP_OUT, start));
	}
	if (c == '>') {
		advance(1);
		return (token_create(TOKEN_REDIRECT_OUT, start));
	}

	if (c == '&' && peek(1) == '>' && peek(1) == '>') {
		advance(3);
		return (token_create(TOKEN_REDIRECT_APPEND_ALL, start));
	}
	if (c == '&' && peek(1) == '>') {
		advance(2);
		return (token_create(TOKEN_REDIRECT_OUT_ALL, start));
	}

	if (c == '|' && peek(1) == '&') {
		advance(2);
		return (token_create(TOKEN_REDIRECT_PIPE_ALL, start));
	}
	if (c == '|') {
		advance(1);
		return (token_create(TOKEN_REDIRECT_PIPE, start));
	}

	return (NULL);
}
