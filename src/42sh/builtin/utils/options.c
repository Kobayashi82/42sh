/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:28:26 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/17 18:38:42 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Parse

	t_opt *parse_options(t_arg *args, const char *valid_opts, bool no_invalid) {
		t_opt *result = safe_calloc(1, sizeof(t_opt));
		result->options = valid_opts; result->args = args;
		if (!args || !valid_opts || !*valid_opts) return (result);

		int valid_index = 0, invalid_index = 0, done = 0;
		while (args && args->value && args->value[0] == '-' && !done) {
			if (valid_index >= MAX_OPTIONS - 1 || invalid_index >= MAX_OPTIONS - 1) {
				result->too_many = true;
				args = args->next; break;
			}

			char *arg = args->value;
			if (!ft_strcmp(arg, "--")) { args = args->next; break; }
			else if (!ft_strcmp(arg, "--help"))		{ if (!ft_strchr(result->valid, 'H')) result->valid[valid_index++] = 'H'; }
			else if (!ft_strcmp(arg, "--version"))	{ if (!ft_strchr(result->valid, 'V')) result->valid[valid_index++] = 'V'; }
			else {
				for (int i = 1; arg[i]; ++i) {
					if (valid_index >= MAX_OPTIONS - 1 || invalid_index >= MAX_OPTIONS - 1)	{ result->too_many = true; break; }
					if (ft_strchr(valid_opts, arg[i])) {
						if (!ft_strchr(result->valid, arg[i])) result->valid[valid_index++] = arg[i];
					} else if (no_invalid) {
						done = true; break;
					} else {
						if (!ft_strchr(result->invalid, arg[i])) result->invalid[invalid_index++] = arg[i];
					} 
				}
			} if (!done && !result->too_many) args = args->next;
		}

		result->valid[valid_index] = '\0';
		result->invalid[invalid_index] = '\0';
		result->args = args;

		return (result);
	}

#pragma endregion
