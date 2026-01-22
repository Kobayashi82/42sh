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
	#include "internal/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Variables"

	typedef struct s_option {
		const char	*name;          // Long option name
		int			*value;         // Pointer to option value
		int			type;           // O_SET, O_SHOPT, O_BOTH
		char		short_opt;      // Short option (0 if none)
	} t_option;

	static t_option options[] = {
		{	"emacs",			&shell.options.emacs,				O_BOTH,		0	},
		{	"vi",				&shell.options.vi,					O_BOTH,		0	},
		{	"hide_ctrl_chars",	&shell.options.hide_ctrl_chars,		O_SHOPT,	0	},
		{	"multiwidth_chars",	&shell.options.multiwidth_chars,	O_SHOPT,	0	},

		{	"history",			&shell.options.history,				O_BOTH,		0	},
		{	"histexpand",		&shell.options.histexpand,			O_BOTH,		'H'	},
		{	"histappend",		&shell.options.histappend,			O_SHOPT,	0	},
		{	"histreedit",		&shell.options.histreedit,			O_SHOPT,	0	},
		{	"histverify",		&shell.options.histverify,			O_SHOPT,	0	},

		{	"expand_aliases",	&shell.options.expand_aliases,		O_SHOPT,	0	},

		{	"noglob",			&shell.options.noglob,				O_BOTH,		'f'	},
		{	"dotglob",			&shell.options.dotglob,				O_SHOPT,	0	},
		{	"nullglob",			&shell.options.nullglob,			O_SHOPT,	0	},
		{	"failglob",			&shell.options.failglob,			O_SHOPT,	0	},
		{	"nocaseglob",		&shell.options.nocaseglob,			O_SHOPT,	0	},

		{	"physical",			&shell.options.physical,			O_BOTH,		'P'	},
		{	"cdable_vars",		&shell.options.cdable_vars,			O_SHOPT,	0	},
		{	"autocd",			&shell.options.autocd,				O_SHOPT,	0	},
		{	"cdspell",			&shell.options.cdspell,				O_SHOPT,	0	},
		{	"dirspell",			&shell.options.dirspell,			O_SHOPT,	0	},

		{	"verbose",			&shell.options.verbose,				O_SET,		'v'	},
		{	"xtrace",			&shell.options.xtrace,				O_SET,		'x'	},

		{	NULL,				NULL,								0,			0	}
	};

#pragma endregion

#pragma region "Set"

	int option_set(const char *name, int value, int type) {
		if (!name) return (0);

		for (int i = 0; options[i].name; ++i) {
			if (!strcmp(options[i].name, name)) {
				int match = 0;
				if (type == O_SHOPT)	match = (options[i].type == O_SHOPT);
				if (type == O_SET)		match = (options[i].type == O_SET   || options[i].type == O_BOTH);
				if (type == O_BOTH)		match = (options[i].type == O_BOTH);
				if (!match) return (shell.error = E_SOPT_INVALID, 1);

				if (!strcmp(options[i].name, "emacs")) {
					for (int j = 0; options[j].name; ++j) {
						if (!strcmp(options[j].name, "vi")) {
							*options[j].value = !value;
							break;
						}
					}
				}
				if (!strcmp(options[i].name, "vi")) {
					for (int j = 0; options[j].name; ++j) {
						if (!strcmp(options[j].name, "emacs")) {
							*options[j].value = !value;
							break;
						}
					}
				}

				*options[i].value = value;
				return (0);
			}
		}

		return (shell.error = E_SOPT_INVALID, 1);
	}

	int option_set_char(char c, int value) {
		for (int i = 0; options[i].name; ++i) {
			if (options[i].short_opt == c) {
				*options[i].value = value;
				return (0);
			}
		}

		return (shell.error = E_SOPT_INVALID, 1);
	}

	int option_get(const char *name) {
		for (int i = 0; options[i].name; ++i) {
			if (!strcmp(options[i].name, name)) {
				return (*options[i].value);
			}
		}
		return (-1);
	}

	int option_get_char(char c) {
		for (int i = 0; options[i].name; ++i) {
			if (options[i].short_opt == c) {
				return (*options[i].value);
			}
		}
		return (-1);
	}

#pragma endregion

#pragma region "Print"

	static int options_type_match(int requested, int option_type) {
		if (requested == O_SHOPT)	return (option_type == O_SHOPT);
		if (requested == O_SET)		return (option_type == O_SET || option_type == O_BOTH);
		if (requested == O_BOTH)	return (option_type == O_BOTH);

		return (0);
	}

	static int options_append_reusable(char **opt_str, const t_option *opt, int type) {
		int			is_on = *opt->value;
		int			use_set = (type == O_BOTH);
		const char	*prefix = (use_set) ? "set " : "shopt ";
		const char	*flag = (use_set) ? ((is_on) ? "-o " : "+o ") : ((is_on) ? "-s " : "-u ");

		char *tmp = ft_strjoin_sep(*opt_str, prefix, flag, J_FREE_VAL_1);
		if (!tmp) return (1);
		*opt_str = ft_strjoin_sep(tmp, opt->name, "\n", J_FREE_VAL_1);
		if (!*opt_str) return (1);

		return (0);
	}

	static int options_append_normal(char **opt_str, const t_option *opt, char *padding, int max_len) {
		int len = ft_strlen(opt->name);
		int spaces = max_len - len + 1;

		char *tmp = ft_strjoin_sep(*opt_str, CYAN300, opt->name, J_FREE_VAL_1);
		if (!tmp) return (1);

		if (padding) {
			if (spaces < max_len) padding[spaces] = '\0';
			*opt_str = ft_strjoin_sep(tmp, padding, (*opt->value) ? GREEN500"on\n"NC : RED500"off\n"NC, J_FREE_VAL_1);
			if (!*opt_str) return (1);
			if (spaces < max_len) padding[spaces] = ' ';
		} else {
			*opt_str = ft_strjoin_sep(tmp, " ", (*opt->value) ? GREEN500"on\n"NC : RED500"off\n"NC, J_FREE_VAL_1);
			if (!*opt_str) return (1);
		}

		return (0);
	}

	int options_print(int type, int reusable, int suppress) {
		int		num_options = 0;
		char	*opt_str = NULL;

		while (options[num_options].name) num_options++;

		// Sort options
		int *sorted_indices = malloc(sizeof(int) * num_options);
		if (!sorted_indices) return (shell.error = E_NO_MEMORY, 1);

		for (int i = 0; i < num_options; ++i) sorted_indices[i] = i;
		for (int i = 1; i < num_options; ++i) {
			int key = sorted_indices[i];
			int j = i - 1;
			while (j >= 0 && strcmp(options[sorted_indices[j]].name, options[key].name) > 0) {
				sorted_indices[j + 1] = sorted_indices[j];
				j--;
			}
			sorted_indices[j + 1] = key;
		}

		// Max line length
		int max_len = 0;
		for (int i = 0; i < num_options; ++i) {
			if (!options_type_match(type, options[i].type)) continue;
			int len = ft_strlen(options[i].name);
			if (len > max_len) max_len = len;
		}

		// Create options string
		char *padding = NULL;
		if (!reusable && max_len > 0) {
			padding = malloc(max_len + 1);
			if (!padding) {
				free(sorted_indices);
				return (shell.error = E_NO_MEMORY, 1);
			}
			memset(padding, ' ', max_len);
			padding[max_len] = '\0';
		}

		for (int i = 0; i < num_options; ++i) {
			int idx = sorted_indices[i];
			if (!options_type_match(type, options[idx].type)) continue;

			int ret = 0;
			if (reusable)	ret = options_append_reusable(&opt_str, &options[idx], type);
			else			ret = options_append_normal(&opt_str, &options[idx], padding, max_len);
			if (ret) {
				free(padding);
				free(sorted_indices);
				return (shell.error = E_NO_MEMORY, 1);
			}
		}

		free(padding);
		free(sorted_indices);

		if (!opt_str)	return (1);
		if (!suppress)	print(STDOUT_FILENO, opt_str, P_RESET_PRINT);
		free(opt_str);

		return (0);
	}

	int option_print(const char *name, int type, int reusable, int suppress) {
		int		num_options = 0;
		char	*opt_str = NULL;

		while (options[num_options].name) num_options++;

		for (int i = 0; i < num_options; ++i) {
			if (!strcmp(name, options[i].name)) {
				if (!options_type_match(type, options[i].type)) continue;

				int ret = 0;
				if (reusable)	ret = options_append_reusable(&opt_str, &options[i], type);
				else			ret = options_append_normal(&opt_str, &options[i], NULL, 0);
				if (ret) {
					free(opt_str);
					return (shell.error = E_NO_MEMORY, 1);
				}
				break;
			}
		}

		if (!opt_str)	return (shell.error = E_SOPT_INVALID, 1);
		if (!suppress)	print(STDOUT_FILENO, opt_str, P_RESET_PRINT);
		free(opt_str);

		return (0);
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
		shell.options.physical			= 0;
		shell.options.cdable_vars		= 1;
		shell.options.autocd			= 1;
		shell.options.cdspell			= 0;
		shell.options.dirspell			= 0;

		return (0);
	}

#pragma endregion
