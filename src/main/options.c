/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:51:23 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/07 23:49:36 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/colors.h"
	#include "main/options.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Variables"

	t_options options;

	static char *option_names[] = {
		"emacs", "vi", "hide_ctrl_chars", "multiwidth_chars",
		"history", "histexpand", "histappend", "histreedit", "histverify",
		"expand_aliases",
		"noglob", "dotglob", "nullglob", "failglob", "nocaseglob",
		"cd_resolve", "cdable_vars", "autocd", "cdspell", "dirspell"
	};

	static int *option_values[] = {
		&options.emacs, &options.vi, &options.hide_ctrl_chars, &options.multiwidth_chars,
		&options.history, &options.histexpand, &options.histappend, &options.histreedit, &options.histverify,
		&options.expand_aliases,
		&options.noglob, &options.dotglob, &options.nullglob, &options.failglob, &options.nocaseglob,
		&options.cd_resolve, &options.cdable_vars, &options.autocd, &options.cdspell, &options.dirspell
	};

	static int option_types[] = {
		SET, SET, SET, SET,
		SET, SET, SHOPT, SHOPT, SHOPT,
		SHOPT,
		SET, SHOPT, SHOPT, SHOPT, SHOPT,
		SET, SHOPT, SHOPT, SHOPT, SHOPT
	};

#pragma endregion

#pragma region "Set"

	int options_set(const char *option, int value) {
		if (!option) return (1);

		//	READINPUT
		if (!strcmp("emacs", option)) {
			if (value) options.vi = !value;
			options.emacs = value;	return (0);
		}
		if (!strcmp("vi", option)) {
			if (value) options.emacs = !value;
			options.vi = value;		return (0);
		}
		if (!strcmp("hide_ctrl_chars", option)) 	{ options.hide_ctrl_chars = value;	return (0); }
		if (!strcmp("multiwidth_chars", option)) 	{ options.multiwidth_chars = value;	return (0); }

		//	HISTORY
		if (!strcmp("history", option)) 			{ options.history = value;			return (0); }
		if (!strcmp("histexpand", option)) 			{ options.histexpand = value;		return (0); }
		if (!strcmp("histappend", option)) 			{ options.histappend = value;		return (0); }
		if (!strcmp("histreedit", option)) 			{ options.histreedit = value;		return (0); }
		if (!strcmp("histverify", option)) 			{ options.histverify = value;		return (0); }

		//	ALIAS
		if (!strcmp("expand_aliases", option)) 		{ options.expand_aliases = value;	return (0); }

		//	GLOBBING
		if (!strcmp("noglob", option)) 				{ options.noglob = value;			return (0); }
		if (!strcmp("dotglob", option)) 			{ options.dotglob = value;			return (0); }
		if (!strcmp("nullglob", option)) 			{ options.nullglob = value;			return (0); }
		if (!strcmp("failglob", option)) 			{ options.failglob = value;			return (0); }
		if (!strcmp("nocaseglob", option)) 			{ options.nocaseglob = value;		return (0); }

		//	CD
		if (!strcmp("cd_resolve", option)) 			{ options.cd_resolve = value;		return (0); }
		if (!strcmp("cdable_vars", option)) 		{ options.cdable_vars = value;		return (0); }
		if (!strcmp("autocd", option)) 				{ options.autocd = value;			return (0); }
		if (!strcmp("cdspell", option)) 			{ options.cdspell = value;			return (0); }
		if (!strcmp("dirspell", option)) 			{ options.dirspell = value;			return (0); }

		return (1);
	}

#pragma endregion

#pragma region "Print"

	char *options_print(char *value, int type) {
		int num_options = sizeof(option_names) / sizeof(option_names[0]);
		char *opt_str = NULL;

		if (!value) {
			// Sort options
			int *sorted_indices = malloc(sizeof(int) * num_options);
			if (!sorted_indices) return (NULL);

			for (int i = 0; i < num_options; ++i) sorted_indices[i] = i;
			for (int i = 0; i < num_options - 1; ++i) {
				for (int j = 0; j < num_options - i - 1; ++j) {
					if (strcmp(option_names[sorted_indices[j]], option_names[sorted_indices[j + 1]]) > 0) {
						int temp = sorted_indices[j];
						sorted_indices[j] = sorted_indices[j + 1];
						sorted_indices[j + 1] = temp;
					}
				}
			}

			// Max line length
			int max_len = 0;
			for (int i = 0; i < num_options; ++i) {
				if (option_types[i] != type) continue;
				int len = ft_strlen(option_names[i]);
				if (len > max_len) max_len = len;
			}

			// Create options string
			for (int i = 0; i < num_options; ++i) {
				int idx = sorted_indices[i];
				if (option_types[idx] != type) continue;

				int len = ft_strlen(option_names[idx]);
				int spaces = max_len - len + 1;

				opt_str = ft_strjoin_sep(opt_str, CYAN300, option_names[idx], 1);

				// Add spaces
				char *padding = malloc(spaces + 1);
				if (padding) {
					memset(padding, ' ', spaces);
					padding[spaces] = '\0';
					opt_str = ft_strjoin_sep(opt_str, padding, (*option_values[idx]) ? GREEN500"on\n" : RED500"off\n", 4);
				}
			}
			free(sorted_indices);
		} else {
			// Single option string
			for (int i = 0; i < num_options; ++i) {
				if (strcmp(value, option_names[i]) == 0 && option_types[i] == type) {
					opt_str = ft_strjoin_sep(CYAN300, option_names[i], " ", 0);
					opt_str = ft_strjoin(opt_str, *option_values[i] ? GREEN500"on\n" : RED500"off\n", 1);
					break;
				}
			}
		}

		return (opt_str);
	}

#pragma endregion

#pragma region "Initialize"

	int options_initialize() {
		//	READINPUT
		options.emacs				= 0;
		options.vi					= 1;
		options.hide_ctrl_chars		= 0;
		options.multiwidth_chars	= 1;

		//	HISTORY
		options.history				= 0;
		options.histexpand			= 0;
		options.histappend			= 1;
		options.histreedit			= 0;
		options.histverify			= 0;

		//	ALIAS
		options.expand_aliases		= 0;

		//	GLOBBING
		options.noglob				= 0;
		options.dotglob				= 0;
		options.nullglob			= 0;
		options.failglob			= 0;
		options.nocaseglob			= 0;

		//	CD
		options.cd_resolve			= 0;
		options.cdable_vars			= 0;
		options.autocd				= 0;
		options.cdspell				= 0;
		options.dirspell			= 0;

		return (0);
	}

#pragma endregion
