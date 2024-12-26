/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:30:16 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/26 12:14:19 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

static size_t handle_single_quotes(const char *input, size_t start, size_t len) {
    size_t end = start + 1;																							// Saltar comilla inicial

    while (end < len && input[end] != '\'')	end++;																	// Todo es literal dentro de comillas simples
    if (end < len && input[end] == '\'')	end++;																	// Incluir comilla de cierre

    return (end);
}

static size_t handle_double_quotes(const char *input, size_t start, size_t len) {
    size_t end = start + 1;																							// Saltar comilla inicial

    while (end < len) {
        if (input[end] == '\\' && end + 1 < len && (input[end + 1] == '"' || input[end + 1] == '\\'))	end += 2; 	// Saltar carácter escapado
        else if (input[end] == '"')																		break;		// Fin de las comillas dobles
        else																							end++;
    }

    if (end < len && input[end] == '"')	end++;																		// Incluir comilla de cierre

    return (end);
}

static size_t handle_separators(const char *input, size_t start, size_t len) {
    size_t end = start + 1;																							// Separador de un carácter
	(void) len;

    if ((input[start] == '>' || input[start] == '<') && input[end] == input[start]) end++;							// Doble redirección (>> o <<)

    return (end);
}

static size_t handle_normal_word(const char *input, size_t start, size_t len) {
    size_t end = start;

    while (end < len && !ft_strchr(" \t\n;|&><(){}", input[end])) {
        if (input[end] == '\\') {
            size_t backslash_count = 0;
            while (end + backslash_count < len && input[end + backslash_count] == '\\')	backslash_count++;

            if (backslash_count % 2 != 0) {
                end += backslash_count;																				// Saltar barras invertidas
                if (end < len) end++;																				// Incluir carácter escapado
            } else end += backslash_count;
        } else end++;
    }

    return (end);
}

static size_t handle_variable_expansion(const char *input, size_t start, size_t len) {
    size_t end = start + 1;																							// Saltar el símbolo '$'

    if (end < len && input[end] == '$')
		return (end + 1);																							// $$ (pid o especial)
    else if (end < len && input[end] == '{') {
        end++;																										// Abrir brace
        while (end < len && input[end] != '}') {
            if (input[end] == '\\' && end + 1 < len) end++;															// Saltar carácter escapado
            end++;
        }
        if (end < len && input[end] == '}') end++;																	// Cerrar brace
    } else if (end < len && input[end] == '(') {
        int parens = 1;
        end++;																										// Abrir paréntesis
        while (end < len && parens > 0) {
            if (input[end] == '\\' && end + 1 < len)	end++;														// Saltar carácter escapado
            else if (input[end] == '(')					parens++;
            else if (input[end] == ')')					parens--;
            end++;
        }
    } else
        while (end < len && (ft_isalnum(input[end]) || input[end] == '_')) end++;									// Variable normal

    return (end);
}

char *get_next_word(const char *input, size_t *pos) {
    size_t start = *pos, len = ft_strlen(input);


	while (start < len && (input[start] == ' ' || input[start] == '\t' || input[start] == '\n')) start++;			// Saltar espacios iniciales

    if (start >= len) { *pos = start; return (NULL); }																// No hay más palabras

    size_t end = start;
    char *word = NULL;

    if (input[start] == '$')						end = handle_variable_expansion(input, start, len);				// Manejo modular según el primer carácter
	else if (input[start] == '\'')					end = handle_single_quotes(input, start, len);
	else if (input[start] == '"')					end = handle_double_quotes(input, start, len);
	else if (ft_strchr(";|&><(){}", input[start]))	end = handle_separators(input, start, len);
	else											end = handle_normal_word(input, start, len);

    word = strndup(&input[start], end - start); *pos = end;															// Crear la palabra

    return (word);
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