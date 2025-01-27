/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:30:16 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/27 12:31:54 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/terminal.h"
	#include "parser/args.h"
	#include "parser/parser.h"

#pragma endregion

#pragma region "Variables"

	t_token	tokens;

#pragma endregion

static size_t handle_single_quotes(const char *input, size_t start, size_t len) {
	size_t end = start + 1;

	while (end < len && input[end] != '\'')	end++;

	if (end < len && input[end] == '\'')	end++;

	return (end);
}

static size_t handle_double_quotes(const char *input, size_t start, size_t len) {
	size_t end = start + 1;

	while (end < len) {
		if (input[end] == '\\' && end + 1 < len)	end++;
		else if (input[end] == '"')					break;
		end++;
	}

	if (end < len && input[end] == '"')	end++;

	return (end);
}

static size_t handle_parenthesis(const char *input, size_t start, size_t len) {
	size_t end = start + 1;

	int parens = 1;
	while (end < len && parens > 0) {
		if (input[end] == '\\' && end + 1 < len)	end++;
		else if (input[end] == '(')					parens++;
		else if (input[end] == ')')					parens--;
		end++;
	}

	return (end);
}

static size_t handle_braces(const char *input, size_t start, size_t len) {
	size_t end = start + 1;

	int braces = 1;
	while (end < len && braces > 0) {
		if (input[end] == '\\' && end + 1 < len)	end++;
		else if (input[end] == '{')					braces++;
		else if (input[end] == '}')					braces--;
		end++;
	}

	return (end);
}

static size_t handle_variables(const char *input, size_t start, size_t len) {
	size_t end = start + 1;

	if (end < len && (input[end] == '$' || ft_isdigit(input[end])))
		end++;
	else if (end < len && input[end] == '\'')
		end = handle_single_quotes(input, end, len);
	else if (end < len && input[end] == '"')
		end = handle_double_quotes(input, end, len);
	else if (end < len && input[end] == '(') {
		end = handle_parenthesis(input, end, len);
	} else if (end < len && input[end] == '{') {
		end = handle_braces(input, end, len);
	} else
		while (end < len && (ft_isalpha(input[end]) || input[end] == '_')) end++;

	return (end);
}

static size_t handle_redirection(const char *input, size_t start, size_t len, bool is_fd) {
	size_t end = start;

	if (!is_fd) {

	} else {
		if (input[end] == '-') end++;
		else if (!ft_isspace(input[end]))
			while(end < len && !ft_isspace(input[end])) end++;
		else if (ft_isspace(input[end])) {
			size_t tmp = end;
			while(tmp < len && ft_isspace(input[tmp])) tmp++;
			if (tmp == len) return (end);
			end = tmp;
			if (input[end] == '-') end++;
			else
				while(end < len && !ft_isspace(input[end])) end++;
		}
	}

	return (end);
}

static size_t handle_separators(const char *input, size_t start, size_t len) {
	size_t end = start + 1;

	if (start + 2 < len && input[start] == '<' && input[start + 1] == '<' && input[start + 2] == '<')		end = handle_redirection(input, end + 2, len, false);	//	<<<	Herestring
	else if (start + 1 < len && input[start] == '<' && input[start + 1] == '<')								end = handle_redirection(input, end + 1, len, false);	//	<<	Heredoc
	else if (start + 1 < len && input[start] == '<' && input[start + 1] == '>')								end = handle_redirection(input, end + 1, len, false);	//	<>	Read & Write
	else if (start + 1 < len && input[start] == '>' && input[start + 1] == '>')								end = handle_redirection(input, end + 1, len, false);	//	>>	Append
	else if (start + 2 < len && input[start] == '&' && input[start + 1] == '>' && input[start + 2] == '>')	end = handle_redirection(input, end + 2, len, false);	//	&>>	stdin & stdout to file (Append)
	else if (start + 1 < len && input[start] == '&' && input[start + 1] == '>')								end = handle_redirection(input, end + 1, len, false);	//	&>	stdin & stdout to file

	else if (start + 1 < len && (input[start] == '<' || input[start] == '>') && input[start + 1] == '&')	end = handle_redirection(input, end + 1, len, true);	//	n>&m or n<&m
	else if (start + 1 < len && (input[start] == '<' || input[start] == '>') && input[start + 1] == '(')	end = handle_parenthesis(input, start + 1, len);
	
	else if (start + 1 < len && input[start] == '&' && input[start + 1] == '&')								end += 1;												//	&&	And
	else if (start + 1 < len && input[start] == '|' && input[start + 1] == '|')								end += 1;												//	||	Or
	else if (start + 1 < len && input[start] == '|' && input[start + 1] == '&')								end += 1;												//	|&	Pipe with stderr

	return (end);
}

static size_t handle_word(const char *input, size_t start, size_t len) {
	size_t end = start;

	while (end < len && !ft_isspace(input[end]) && !ft_strchr(";|&<>(){}", input[end])) {
		if (input[end] == '\\' && end + 1 < len) end += 2;
		else end++;
	}

	return (end);
}


char *get_next_word(const char *input, size_t *pos, bool only_space) {
    size_t start = *pos, len = ft_strlen(input);

	while (start < len && ft_isspace(input[start])) start++;

	if (start >= len) { *pos = start; return (NULL); }

	size_t end = start;
	char *word = NULL;

	while (end < len && !ft_isspace(input[end])) {
		if (input[end] == '\'')										end = handle_single_quotes(input, end, len);
		else if (input[end] == '"')									end = handle_double_quotes(input, end, len);
		else if (input[end] == '(')									end = handle_parenthesis(input, end, len);
		else if (input[end] == '{')									end = handle_braces(input, end, len);
		else if (input[end] == '$')									end = handle_variables(input, end, len);
		else if (ft_strchr(";|&<>", input[end]))					end = handle_separators(input, end, len);
		else {
			if (ft_isdigit(input[end])) {
				size_t tmp = end;
				while(tmp < len && ft_isdigit(input[tmp])) tmp++;
				if (tmp != len && ft_strchr("<>", input[tmp])) {	end = handle_separators(input, tmp, len); break; }
				else												end = handle_word(input, end, len);
			} else													end = handle_word(input, end, len);
		}

		if (end > 0 && ft_strchr(";|&", input[end - 1])) break;
		if (!only_space || ft_strchr(";|&<>", input[end]) || (ft_strchr("{(", input[end]) && !(end > 0 && input[end -1] == '$'))) break;
	}

	word = ft_strndup(&input[start], end - start); *pos = end;															// Crear la palabra

	return (word);
}

void first_step() {
	size_t pos = 0;
	char *word = NULL;

	while ((word = get_next_word(terminal.input, &pos, true)) != NULL) {
		t_arg *new_arg = ft_calloc(1, sizeof(t_arg));
		if (!new_arg) {
			ft_printf(1, "Memory allocation failed\n");
			break;
		}

		new_arg->value = word;
		t_arg *tmp = tokens.args;

		if (!tmp) tokens.args = new_arg;
		else {
			while (tmp->next) tmp = tmp->next;
			tmp->next = new_arg;
		}

		ft_printf(1, "Word: %s\n", word);
	}

	t_arg *current = tokens.args;
	while (current) {
		t_arg *next = current->next;
		sfree(current->value);
		sfree(current);
		current = next;
	}
	tokens.args = NULL;
	sfree(terminal.input);
}



//	TOKENIZER
//
//	Recorre la entrada y busca alias para cada primer comando despues de un operador	| && || ; & $( ` {
//	Comprueba sintaxis
//	Se vuelve a analizar la cadena separando comandos en base a los operadores			&& || ; &

//	LEXER
//
//	Asignaciones y redirecciones se sacan aparte										<> < > << >> <<< |& n> n>> n>&m n<&m &> n<&- 		>n tambien sirve, por ejemplo
//	Por cada comando se separan las palabras											" ' $ $() ${} $(()) $" $' () {} (())

//	REDIRECCIONES
//
//	<		Redirige la entrada estándar (stdin) desde un archivo.
//	>		Redirige la salida estándar (stdout) a un archivo (sobrescribe).
//	>>		Redirige la salida estándar (stdout) a un archivo (añade al final).
//	<>		Abre un archivo para lectura y escritura simultáneamente.
//	<<		Heredoc: Usa texto literal como entrada estándar (stdin).
//	<<<		Here-string: Usa una cadena como entrada estándar (stdin).

//	n<		Redirige al descriptor n un archivo.
//	n>		Redirige el descriptor n a un archivo (sobrescribe).
//	n>>		Redirige el descriptor n a un archivo (añade al final).
//	n<>		Redirige el descriptor n a un archivo en modo bidireccional (lectura y escritura).
//	n>&m	Redirige el descriptor n al mismo destino que el descriptor m.
//	n<&m	Redirige el descriptor n para leer desde el mismo origen que m.
//	n<&-	Cierra el descriptor n para lectura.
//	n>&-	Cierra el descriptor n para escritura.

//	&>		Redirige tanto stdout como stderr a un archivo (sobrescribe).
//	&>>		Redirige tanto stdout como stderr a un archivo (añade al final).


//	PARSER
//	
//	


// Tilde expansion (~) ocurre antes de la expansión de variables.
// Globbing (*, ?, [...]) ocurre después de la expansión de variables