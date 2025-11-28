/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:51:23 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/28 23:10:54 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/options.h"

	#include "utils/libft.h"
	#include "utils/print.h"

#pragma endregion

#pragma region "Variables"

	t_options options;

	static char *option_names[] = {
		"autocd", "cdable_vars", "dirspell", "dotglob",
		"emacs", "expand_aliases", "failglob", "hist_local",
		"hist_on", "nocaseglob", "noglob", "nullglob", "vi", "histexpand"
	};

	static int *option_values[] = {
		&options.autocd, &options.cdable_vars, &options.dirspell, &options.dotglob,
		&options.emacs, &options.expand_aliases, &options.failglob, &options.hist_local,
		&options.history, &options.nocaseglob, &options.noglob, &options.nullglob, &options.vi, &options.histexpand
	};

	static int option_types[] = {
		SHOPT, SHOPT, SHOPT, SHOPT,
		SET, SHOPT, SHOPT, SET,
		SET, SHOPT, SET, SHOPT, SET, SET
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
		if (!strcmp("hist_local", option)) 			{ options.hist_local = value;		return (0); }
		if (!strcmp("hist_on", option)) 			{ options.history = value;			return (0); }
		if (!strcmp("histexpand", option)) 			{ options.histexpand = value;		return (0); }

		//	ALIAS
		if (!strcmp("expand_aliases", option)) 		{ options.expand_aliases = value;	return (0); }

		//	GLOBBING
		if (!strcmp("noglob", option)) 				{ options.noglob = value;			return (0); }
		if (!strcmp("dotglob", option)) 			{ options.dotglob = value;			return (0); }
		if (!strcmp("nullglob", option)) 			{ options.nullglob = value;			return (0); }
		if (!strcmp("failglob", option)) 			{ options.failglob = value;			return (0); }
		if (!strcmp("nocaseglob", option)) 			{ options.nocaseglob = value;		return (0); }

		//	CD
		if (!strcmp("cdable_vars", option)) 		{ options.cdable_vars = value;		return (0); }
		if (!strcmp("autocd", option)) 				{ options.autocd = value;			return (0); }
		if (!strcmp("dirspell", option)) 			{ options.dirspell = value;			return (0); }

		return (1);
	}

#pragma endregion

#pragma region "Print"

	char *options_print(char *value, int type) {
		int num_options = sizeof(option_names) / sizeof(option_names[0]);
		char *opt_str = NULL;

		if (!value) {
			for (int i = 0; i < num_options; i++) {
				if (option_types[i] != type) continue;
				char *tab = " \t";
				if (ft_strlen(option_names[i]) < 7) tab = " \t\t";
				
				opt_str = ft_strjoin_sep(opt_str, CYAN300, option_names[i], 1);
				if (*option_values[i])	opt_str = ft_strjoin_sep(opt_str, tab, GREEN500"on\n", 1);
				else					opt_str = ft_strjoin_sep(opt_str, tab, RED500"off\n", 1);
			}

			return (opt_str);
		} else {
			for (int i = 0; i < num_options; i++) {
				if (strcmp(value, option_names[i]) == 0) {
					if (option_types[i] != type) continue;
					char *tab = " \t";
					if (ft_strlen(option_names[i]) < 7) tab = " \t\t";

					opt_str = ft_strjoin_sep(CYAN300, option_names[i], tab, 0);
					if (*option_values[i])	opt_str = ft_strjoin(opt_str, GREEN500"on\n", 1);
					else					opt_str = ft_strjoin(opt_str, RED500"off\n", 1);

					return (opt_str);
				}
			}

			return (NULL);
		}
	}

#pragma endregion

#pragma region "Initialize"

	int options_initialize() {
		//	READINPUT
		options.emacs				= 0;
		options.vi					= 1;
		options.hide_ctrl_chars		= 0;
		options.multiwidth_chars	= 0;

		//	HISTORY
		options.history				= 1;
		options.hist_local			= 1;
		options.histexpand			= 1;

		//	ALIAS
		options.expand_aliases		= 1;

		//	GLOBBING
		options.noglob				= 0;
		options.dotglob				= 0;
		options.nullglob			= 0;
		options.failglob			= 0;
		options.nocaseglob			= 0;

		//	CD
		options.cdable_vars			= 0;
		options.autocd				= 0;
		options.dirspell			= 0;

		return (0);
	}

#pragma endregion
