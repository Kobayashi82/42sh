/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:51:23 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/09 12:31:39 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/colors.h"
	#include "main/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Variables"

	static char *option_names[] = {
		"emacs",				"vi",					"hide_ctrl_chars",		"multiwidth_chars",
		"history",				"histexpand",			"histappend",			"histreedit",			"histverify",
		"expand_aliases",
		"noglob",				"dotglob",				"nullglob",				"failglob",				"nocaseglob",
		"cd_resolve",			"cdable_vars",			"autocd",				"cdspell",				"dirspell"
	};

	static int *option_values[] = {
		&shell.options.emacs,				&shell.options.vi,					&shell.options.hide_ctrl_chars,		&shell.options.multiwidth_chars,
		&shell.options.history,				&shell.options.histexpand,			&shell.options.histappend,			&shell.options.histreedit,			&shell.options.histverify,
		&shell.options.expand_aliases,
		&shell.options.noglob,				&shell.options.dotglob,				&shell.options.nullglob,			&shell.options.failglob,			&shell.options.nocaseglob,
		&shell.options.cd_resolve,			&shell.options.cdable_vars,			&shell.options.autocd,				&shell.options.cdspell,				&shell.options.dirspell
	};

	static int option_types[] = {
		SET,	SET,	SET,	SET,
		SET,	SET,	SHOPT,	SHOPT,	SHOPT,
		SHOPT,
		SET,	SHOPT,	SHOPT,	SHOPT,	SHOPT,
		SET,	SHOPT,	SHOPT,	SHOPT,	SHOPT
	};

#pragma endregion

#pragma region "Set"

	int options_set(const char *option, int value) {
		if (!option) return (1);

		//	READINPUT
		if (!strcmp("emacs", option)) {
			if (value) shell.options.vi = !value;
			shell.options.emacs = value;	return (0);
		}
		if (!strcmp("vi", option)) {
			if (value) shell.options.emacs = !value;
			shell.options.vi = value;		return (0);
		}
		if (!strcmp("hide_ctrl_chars", option)) 	{ shell.options.hide_ctrl_chars = value;	return (0); }
		if (!strcmp("multiwidth_chars", option)) 	{ shell.options.multiwidth_chars = value;	return (0); }

		//	HISTORY
		if (!strcmp("history", option)) 			{ shell.options.history = value;			return (0); }
		if (!strcmp("histexpand", option)) 			{ shell.options.histexpand = value;		return (0); }
		if (!strcmp("histappend", option)) 			{ shell.options.histappend = value;		return (0); }
		if (!strcmp("histreedit", option)) 			{ shell.options.histreedit = value;		return (0); }
		if (!strcmp("histverify", option)) 			{ shell.options.histverify = value;		return (0); }

		//	ALIAS
		if (!strcmp("expand_aliases", option)) 		{ shell.options.expand_aliases = value;	return (0); }

		//	GLOBBING
		if (!strcmp("noglob", option)) 				{ shell.options.noglob = value;			return (0); }
		if (!strcmp("dotglob", option)) 			{ shell.options.dotglob = value;			return (0); }
		if (!strcmp("nullglob", option)) 			{ shell.options.nullglob = value;			return (0); }
		if (!strcmp("failglob", option)) 			{ shell.options.failglob = value;			return (0); }
		if (!strcmp("nocaseglob", option)) 			{ shell.options.nocaseglob = value;		return (0); }

		//	CD
		if (!strcmp("cd_resolve", option)) 			{ shell.options.cd_resolve = value;		return (0); }
		if (!strcmp("cdable_vars", option)) 		{ shell.options.cdable_vars = value;		return (0); }
		if (!strcmp("autocd", option)) 				{ shell.options.autocd = value;			return (0); }
		if (!strcmp("cdspell", option)) 			{ shell.options.cdspell = value;			return (0); }
		if (!strcmp("dirspell", option)) 			{ shell.options.dirspell = value;			return (0); }

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

				opt_str = ft_strjoin_sep(opt_str, CYAN300, option_names[idx], J_FREE_VAL_1);

				// Add spaces
				char *padding = malloc(spaces + 1);
				if (padding) {
					memset(padding, ' ', spaces);
					padding[spaces] = '\0';
					opt_str = ft_strjoin_sep(opt_str, padding, (*option_values[idx]) ? GREEN500"on\n" : RED500"off\n", J_FREE_VAL_1_2);
				}
			}
			free(sorted_indices);
		} else {
			// Single option string
			for (int i = 0; i < num_options; ++i) {
				if (strcmp(value, option_names[i]) == 0 && option_types[i] == type) {
					opt_str = ft_strjoin_sep(CYAN300, option_names[i], " ", J_FREE_NONE);
					opt_str = ft_strjoin(opt_str, *option_values[i] ? GREEN500"on\n" : RED500"off\n", J_FREE_VAL_1);
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
		shell.options.emacs				= 1;
		shell.options.vi				= 0;
		shell.options.hide_ctrl_chars	= 0;
		shell.options.multiwidth_chars	= 0;

		//	HISTORY
		shell.options.history			= 0;
		shell.options.histexpand		= 0;
		shell.options.histappend		= 1;
		shell.options.histreedit		= 0;
		shell.options.histverify		= 0;

		//	ALIAS
		shell.options.expand_aliases	= 0;

		//	GLOBBING
		shell.options.noglob			= 0;
		shell.options.dotglob			= 0;
		shell.options.nullglob			= 0;
		shell.options.failglob			= 0;
		shell.options.nocaseglob		= 0;

		//	CD
		shell.options.cd_resolve		= 0;
		shell.options.cdable_vars		= 1;
		shell.options.autocd			= 0;
		shell.options.cdspell			= 0;
		shell.options.dirspell			= 0;

		return (0);
	}

#pragma endregion
