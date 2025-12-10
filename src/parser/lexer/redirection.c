/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:31:12 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/10 14:47:54 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/lexer.h"
	#include "utils/libft.h"

#pragma endregion

#pragma region "Redirection"

	t_token *redirection(t_lexer *lexer) {
		t_string	string;
		int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
		char		*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;
		char		c = peek(lexer, 0);
		
		string_init(&string);

		if (c == '<' && peek(lexer, 1) == '<' && peek(lexer, 2) == '<') {
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			return (token_create(lexer, TOKEN_REDIRECT_HERESTRING, string.value, line, full_line));
		}
		if (c == '<' && peek(lexer, 1) == '<') {
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			return (token_create(lexer, TOKEN_REDIRECT_HEREDOC, string.value, line, full_line));
		}
		if (c == '<' && peek(lexer, 1) == '>') {
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			return (token_create(lexer, TOKEN_REDIRECT_IN_OUT, string.value, line, full_line));
		}
		if (c == '<' && peek(lexer, 1) == '&') {
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			return (token_create(lexer, TOKEN_REDIRECT_DUP_IN, string.value, line, full_line));
		}
		if (c == '<') {
			segment_append(segment,advance(lexer));
			return (token_create(lexer, TOKEN_REDIRECT_IN, string.value, line, full_line));
		}

		if (c == '>' && peek(lexer, 1) == '>') {
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			return (token_create(lexer, TOKEN_REDIRECT_APPEND, string.value, line, full_line));
		}
		if (c == '>' && peek(lexer, 1) == '|') {
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			return (token_create(lexer, TOKEN_REDIRECT_FORCE_OUT, string.value, line, full_line));
		}
		if (c == '>' && peek(lexer, 1) == '&') {
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			return (token_create(lexer, TOKEN_REDIRECT_DUP_OUT, string.value, line, full_line));
		}
		if (c == '>') {
			segment_append(segment,advance(lexer));
			return (token_create(lexer, TOKEN_REDIRECT_OUT, string.value, line, full_line));
		}

		if (c == '&' && peek(lexer, 1) == '>' && peek(lexer, 1) == '>') {
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			return (token_create(lexer, TOKEN_REDIRECT_APPEND_ALL, string.value, line, full_line));
		}
		if (c == '&' && peek(lexer, 1) == '>') {
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			return (token_create(lexer, TOKEN_REDIRECT_OUT_ALL, string.value, line, full_line));
		}

		free(full_line);
		free(string.value);
		return (NULL);
	}

#pragma endregion

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

// Operadores de Redirección
// - [x] `<` - input redirection
// - [x] `>` - output redirection
// - [x] `>>` - append output
// - [x] `<<` - here-document
// - [ ] `<<<` - here-string
// - [x] `<&` - duplicate input fd
// - [x] `>&` - duplicate output fd
// - [x] `<>` - open for reading and writing
// - [x] `>|` - force overwrite (noclobber)
// - [x] `&>` - redirect stdout and stderr
// - [x] `&>>` - append stdout and stderr
// - [ ] `[n]<` - numbered input redirection (ej: `3<`)
// - [ ] `[n]>` - numbered output redirection (ej: `2>`)
// - [ ] `[n]>>` - numbered append
// - [ ] `[n]<&` - numbered duplicate input
// - [ ] `[n]>&` - numbered duplicate output
// - [ ] `[n]<>` - numbered read/write