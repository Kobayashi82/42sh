/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 20:59:56 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/22 19:48:03 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#define MAX_OPTIONS	128
	#define HASH_SIZE	101

#pragma endregion

#pragma region Variables

	#pragma region Structures

		typedef struct s_builtin t_builtin;

		typedef struct s_opt {
			const char	*options;
			char		valid[MAX_OPTIONS];
			char		invalid[MAX_OPTIONS];
			bool		too_many;
			t_arg		*args;
		}	t_opt;

		typedef struct s_builtin{
			char		*name;
			bool		disabled;
			bool		special;
		    int         (*execute)(t_arg *args);
			t_builtin	*next;
		}	t_builtin;

	#pragma endregion

	extern t_builtin	*builtin_table[HASH_SIZE];

#pragma endregion

#pragma region Methods

	//	======= BUILTIN_HASH =======
	int			builtin_add(const char *name, int disabled, int special, int (*execute)(t_arg *args));
	t_builtin	*builtin_find(const char *name);
	char		**builtin_to_array(int disabled, bool special, bool sort);
	int			builtin_print(int disabled, bool special, bool sort);
	size_t		builtin_length(int disabled, bool special);
	void		builtin_clear();
	int			builtin_delete(const char *name);
	int			builtin_initialize();

	//	========== UTILS ===========
	t_opt		*parse_options(t_arg *args, const char *valid_opts, char opt_char, bool no_invalid);
	int			invalid_option(char *name, char *opts, char *usage);
	int			print_version(char *name, char *version);
	char		*format_for_shell(const char *value, char quote_type);

	//	========== ALIAS ===========
	int			alias(t_arg *args);

	//	========= UNALIAS ==========
	int			unalias(t_arg *args);

	//	========= DECLARE ==========
	int			declare(t_arg *args);

	//	========= READONLY =========
	int			readonly(t_arg *args);

	//	========== EXPORT ==========
	int			export(t_arg *args);

	//	=========== UNSET ==========
	int			unset(t_arg *args);

	//	========== ENABLE ==========
	int			enable(t_arg *args);

	//	============ FC ============
	const char	*default_editor();

	int			builtin_exec(t_arg *args);

#pragma endregion
