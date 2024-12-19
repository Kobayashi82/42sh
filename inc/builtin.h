/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 20:59:56 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/19 19:24:56 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#define MAX_OPTIONS 128

#pragma endregion

#pragma region Variables

	#pragma region Structures

		typedef struct s_opt {
			const char	*options;
			char		valid[MAX_OPTIONS];
			char		invalid[MAX_OPTIONS];
			bool		too_many;
			t_arg		*args;
		}	t_opt;

	#pragma endregion

#pragma endregion

#pragma region Methods

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

	//	============ FC ============
	const char	*default_editor();

#pragma endregion
