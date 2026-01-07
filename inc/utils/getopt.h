/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getopt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:26:41 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/07 16:32:06 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>

	#define MAX_OPTIONS	128

#pragma endregion

#pragma region "Variables"

	#pragma region "Structures"

		enum e_getopt {
			NO_ARGUMENT,
			REQUIRED_ARG,
			OPTIONAL_ARG,
			IGNORE_OFF,
			IGNORE_NUMBER,
			IGNORE_STRING
		};

		typedef struct s_long_option {
			const char			*name;				// Nombre de la opción (ej: "help")
			int					has_arg;			// NO_ARG, REQUIRED_ARG, OPTIONAL_ARG
			char				short_equiv;		// Equivalente corto (0 si no tiene)
		} t_long_option;

		typedef struct s_opt_value {
			char				opt;				// Opción ('a', 'b', etc)
			char				*value;				// Valor (NULL si no tiene)
			int					is_long;			// 1 si era opción larga, 0 si corta
			char				*long_name;			// Nombre largo (NULL si era corta)
			struct s_opt_value	*next;
		} t_opt_value;

		typedef struct s_parse_result {
			t_opt_value			*options;			// Lista de opciones encontradas
			t_opt_value			*plus_options;		// Opciones con '+' encontradas
			char				**argv_original;	// Argumentos originales
			int					argc;				// Número de argumentos no-opción
			char				**argv;				// Argumentos no-opción
			const char			*shell_name;		// Nombre del shell (lo que aparece en argv[0])
			const char			*name;				// nombre del comando que llama a getopt
			const char			*usage;				// syntax del comando que llama a getopt
			char				invalid_opt;		// Opción inválida encontrada (0 si no hay)
			char				*invalid_long;		// Opción larga inválida (NULL si no hay)
			char				missing_arg;		// Opción que requiere argumento (0 si no hay)
			int					error;				// 2 si MAX_OPTIONS, 1 si hubo algún error, 0 si no
			int					silent_mode;		// ignora mensajes de error en opciones '-'
			int					silent_mode_plus;	// ignora mensajes de error en opciones '+'
		} t_parse_result;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	// Short
	t_opt_value     *find_option(t_parse_result *result, char opt);
	int             has_option(t_parse_result *result, char opt);
	const char      *get_option_value(t_parse_result *result, char opt);

	// Short Plus
	t_opt_value     *find_plus_option(t_parse_result *result, char opt);
	int             has_plus_option(t_parse_result *result, char opt);
	const char      *get_plus_option_value(t_parse_result *result, char opt);

	// Long
	t_opt_value     *find_long_option(t_parse_result *result, const char *name);
	int             has_long_option(t_parse_result *result, const char *name);
	const char      *get_long_option_value(t_parse_result *result, const char *name);

	// Free
	void			free_argv_original(t_parse_result *result);
	void            free_options(t_parse_result *result);

	// Parsing
	t_parse_result *parse_options(int argc, char **argv, const char *short_opts, const char *short_opts_plus, const t_long_option *long_opts, const char *usage, int ignore_mode);

#pragma endregion
