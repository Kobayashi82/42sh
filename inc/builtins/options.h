/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:28:32 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/21 22:17:05 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include "args.h"

	#define MAX_OPTIONS	128

#pragma endregion

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_opt {
			const char	*options;
			char		valid[MAX_OPTIONS];
			char		invalid[MAX_OPTIONS];
			bool		too_many;
			t_arg		*args;
		}	t_opt;

	#pragma endregion

#pragma endregion

#pragma region "Methods"


	//	========== UTILS ===========
	t_opt		*parse_options(t_arg *args, const char *valid_opts, char opt_char, bool no_invalid);
	int			invalid_option(char *name, char *opts, char *usage);
	int			print_version(char *name, char *version);

#pragma endregion
