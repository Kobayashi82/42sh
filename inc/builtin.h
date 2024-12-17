/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 20:59:56 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/17 17:30:44 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#define MAX_OPTIONS 128

#pragma endregion

typedef struct s_opt {
	const char	*options;
	char		valid[MAX_OPTIONS];
	char		invalid[MAX_OPTIONS];
	int			valid_index, invalid_index;
	bool		too_many;
    t_arg		*args;
}	t_opt;

#pragma region Methods

	//	---------- OPTIONS ---------
	t_opt		*parse_options(t_arg *args, const char *opts, bool no_invalid);

	//	------------ FC ------------
	const char *default_editor();

#pragma endregion
