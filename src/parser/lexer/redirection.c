/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:31:12 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 16:27:47 by vzurera-         ###   ########.fr       */
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
	t_string	string;
	char		c = peek(0);
	
	string_init(&string);

	if (c == '<' && peek(1) == '<' && peek(2) == '<') {
		string_append(&string, advance());
		string_append(&string, advance());
		string_append(&string, advance());
		return (token_create(TOKEN_REDIRECT_HERESTRING, string.value));
	}
	if (c == '<' && peek(1) == '<') {
		string_append(&string, advance());
		string_append(&string, advance());
		return (token_create(TOKEN_REDIRECT_HEREDOC, string.value));
	}
	if (c == '<' && peek(1) == '>') {
		string_append(&string, advance());
		string_append(&string, advance());
		return (token_create(TOKEN_REDIRECT_IN_OUT, string.value));
	}
	if (c == '<' && peek(1) == '&') {
		string_append(&string, advance());
		string_append(&string, advance());
		return (token_create(TOKEN_REDIRECT_DUP_IN, string.value));
	}
	if (c == '<') {
		string_append(&string, advance());
		return (token_create(TOKEN_REDIRECT_IN, string.value));
	}

	if (c == '>' && peek(1) == '>') {
		string_append(&string, advance());
		string_append(&string, advance());
		return (token_create(TOKEN_REDIRECT_APPEND, string.value));
	}
	if (c == '>' && peek(1) == '|') {
		string_append(&string, advance());
		string_append(&string, advance());
		return (token_create(TOKEN_REDIRECT_FORCE_OUT, string.value));
	}
	if (c == '>' && peek(1) == '&') {
		string_append(&string, advance());
		string_append(&string, advance());
		return (token_create(TOKEN_REDIRECT_DUP_OUT, string.value));
	}
	if (c == '>') {
		string_append(&string, advance());
		return (token_create(TOKEN_REDIRECT_OUT, string.value));
	}

	if (c == '&' && peek(1) == '>' && peek(1) == '>') {
		string_append(&string, advance());
		string_append(&string, advance());
		string_append(&string, advance());
		return (token_create(TOKEN_REDIRECT_APPEND_ALL, string.value));
	}
	if (c == '&' && peek(1) == '>') {
		string_append(&string, advance());
		string_append(&string, advance());
		return (token_create(TOKEN_REDIRECT_OUT_ALL, string.value));
	}

	if (c == '|' && peek(1) == '&') {
		string_append(&string, advance());
		string_append(&string, advance());
		return (token_create(TOKEN_REDIRECT_PIPE_ALL, string.value));
	}
	if (c == '|') {
		string_append(&string, advance());
		return (token_create(TOKEN_REDIRECT_PIPE, string.value));
	}

	return (NULL);
}
