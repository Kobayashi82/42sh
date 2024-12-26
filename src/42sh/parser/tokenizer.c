/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:30:16 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/26 17:57:17 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

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

static size_t handle_separators(const char *input, size_t start, size_t len) {
	size_t end = start + 1;

	if (start + 2 < len && input[start] == '<' && input[start + 1] == '<' && input[start + 2] == '<')		end += 2;	//	<<<	Herestring
	else if (start + 1 < len && input[start] == '<' && input[start + 1] == '<')								end += 1;	//	<<	Heredoc
	else if (start + 1 < len && input[start] == '<' && input[start + 1] == '>')								end += 1;	//	<>	Read & Write
	else if (start + 1 < len && input[start] == '>' && input[start + 1] == '>')								end += 1;	//	>>	Append
	else if (start + 1 < len && input[start] == '|' && input[start + 1] == '&')								end += 1;	//	|&	Pipe with stderr
	else if (start + 1 < len && input[start] == '|' && input[start + 1] == '|')								end += 1;	//	||	Or
	else if (start + 1 < len && input[start] == '&' && input[start + 1] == '&')								end += 1;	//	&&	And
	else if (start + 1 < len && (input[start] == '<' || input[start] == '>') && input[start + 1] == '&')	end += 1;	//	n>&n or n<&n
	else if (start + 1 < len && (input[start] == '<' || input[start] == '>') && input[start + 1] == '(')	end = handle_parenthesis(input, start + 1, len);

	return (end);
}

static size_t handle_word(const char *input, size_t start, size_t len) {
	size_t end = start;

	while (end < len && !ft_isspace(input[end]) && !ft_strchr(";|&><(){}", input[end])) {
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
		else if (ft_strchr(";|&><", input[end]))					end = handle_separators(input, end, len);
		else														end = handle_word(input, end, len);
		if (end > 0 && ft_strchr(";|&><(){}", input[end - 1])) break;
		if (!only_space || ft_strchr(";|&><(){}", input[end])) break;
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
			free(word);
			exit_error(NO_MEMORY, 1, NULL, true);
		}

		new_arg->value = word;
		t_arg *tmp = tokens.args;

		if (!tmp) tokens.args = new_arg;
 		else {
 			while (tmp->next) tmp = tmp->next;
 			tmp->next = new_arg;
 		}

		printf("Word: %s\n", word);
	}
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

//	n< o <n	Redirige al descriptor n un archivo.
//	n>		Redirige el descriptor n a un archivo (sobrescribe).
//	n>>		Redirige el descriptor n a un archivo (añade al final).
//	n<>		Abre un archivo en modo bidireccional (lectura y escritura).
//	n>&m	Redirige el descriptor n al mismo destino que el descriptor m.
//	n<&m	Redirige el descriptor n para leer desde el mismo origen que m.
//	n<&-	Cierra el descriptor n para lectura.
//	n>&-	Cierra el descriptor n para escritura.

//	&>		Redirige tanto stdout como stderr a un archivo (sobrescribe).


//	PARSER
//	
//	


// Tilde expansion (~) ocurre antes de la expansión de variables.
// Globbing (*, ?, [...]) ocurre después de la expansión de variables