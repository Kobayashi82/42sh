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
		int			type;           // O_SET, O_SHOPT
		char		short_opt;      // Short option (0 if none)
	} t_option;

	static t_option options[] = {
		//	=== SHELL ===
		{	"allexport",				&shell.options.allexport,					O_SET,		'a'	},
		{	"errexit",					&shell.options.errexit,						O_SET,		'e'	},
		{	"errtrace",					&shell.options.errtrace,					O_SET,		'E'	},
		{	"functrace",				&shell.options.functrace,					O_SET,		'T'	},
		{	"nounset",					&shell.options.nounset,						O_SET,		'u'	},
		{	"noclobber",				&shell.options.noclobber,					O_SET,		'C'	},
		{	"noexec",					&shell.options.noexec,						O_SET,		'n'	},
		{	"notify",					&shell.options.notify,						O_SET,		'b'	},
		{	"monitor",					&shell.options.monitor,						O_SET,		'm'	},
		{	"privileged",				&shell.options.privileged,					O_SET,		'p'	},
		{	"pipefail",					&shell.options.pipefail,					O_SET,		0	},
		{	"hashall",					&shell.options.hashall,						O_SET,		'h'	},
		{	"ignoreeof",				&shell.options.ignoreeof,					O_SET,		0	},
		{	"execfail",					&shell.options.execfail,					O_SHOPT,	0	},
		{	"checkjobs",				&shell.options.checkjobs,					O_SHOPT,	0	},
		{	"huponexit",				&shell.options.huponexit,					O_SHOPT,	0	},
		{	"inherit_errexit",			&shell.options.inherit_errexit,				O_SHOPT,	0	},
		{	"login_shell",				&shell.options.login_shell,					O_SHOPT,	0	},
		{	"restricted_shell",			&shell.options.restricted_shell,			O_SHOPT,	0	},
		
		//	=== READINPUT ===
		{	"emacs",					&shell.options.emacs,						O_SET,		0	},
		{	"vi",						&shell.options.vi,							O_SET,		0	},
		{	"hide_ctrl_chars",			&shell.options.hide_ctrl_chars,				O_SET,		0	},
		{	"multiwidth_chars",			&shell.options.multiwidth_chars,			O_SET,		0	},
		
		//	=== HISTORY ===
		{	"history",					&shell.options.history,						O_SET,		0	},
		{	"histexpand",				&shell.options.histexpand,					O_SET,		'H'	},
		{	"histappend",				&shell.options.histappend,					O_SHOPT,	0	},
		{	"histreedit",				&shell.options.histreedit,					O_SHOPT,	0	},
		{	"histverify",				&shell.options.histverify,					O_SHOPT,	0	},
		
		//	=== EXPANSION ===
		{	"expand_aliases",			&shell.options.expand_aliases,				O_SHOPT,	0	},
		{	"braceexpand",				&shell.options.braceexpand,					O_SET,		'B'	},
		{	"noglob",					&shell.options.noglob,						O_SET,		'f'	},
		{	"dotglob",					&shell.options.dotglob,						O_SHOPT,	0	},
		{	"nullglob",					&shell.options.nullglob,					O_SHOPT,	0	},
		{	"failglob",					&shell.options.failglob,					O_SHOPT,	0	},
		{	"nocaseglob",				&shell.options.nocaseglob,					O_SHOPT,	0	},
		{	"nocasematch",				&shell.options.nocasematch,					O_SHOPT,	0	},
		{	"extglob",					&shell.options.extglob,						O_SHOPT,	0	},
		{	"globstar",					&shell.options.globstar,					O_SHOPT,	0	},
		{	"globasciiranges",			&shell.options.globasciiranges,				O_SHOPT,	0	},
		{	"globskipdots",				&shell.options.globskipdots,				O_SHOPT,	0	},
		{	"extquote",					&shell.options.extquote,					O_SHOPT,	0	},
		{	"patsub_replacement",		&shell.options.patsub_replacement,			O_SHOPT,	0	},
		
		//	=== DIRECTORY ===
		{	"physical",					&shell.options.physical,					O_SET,		'P'	},
		{	"cdable_vars",				&shell.options.cdable_vars,					O_SHOPT,	0	},
		{	"autocd",					&shell.options.autocd,						O_SHOPT,	0	},
		{	"cdspell",					&shell.options.cdspell,						O_SHOPT,	0	},
		{	"dirspell",					&shell.options.dirspell,					O_SHOPT,	0	},
		{	"direxpand",				&shell.options.direxpand,					O_SHOPT,	0	},
		{	"checkwinsize",				&shell.options.checkwinsize,				O_SHOPT,	0	},
		
		//	=== COMPLETION ===
		{	"progcomp",					&shell.options.progcomp,					O_SHOPT,	0	},
		{	"progcomp_alias",			&shell.options.progcomp_alias,				O_SHOPT,	0	},
		{	"hostcomplete",				&shell.options.hostcomplete,				O_SHOPT,	0	},
		{	"no_empty_cmd_completion",	&shell.options.no_empty_cmd_completion,		O_SHOPT,	0	},
		{	"complete_fullquote",		&shell.options.complete_fullquote,			O_SHOPT,	0	},
		{	"force_fignore",			&shell.options.force_fignore,				O_SHOPT,	0	},
		
		//	=== EXECUTION ===
		{	"checkhash",				&shell.options.checkhash,					O_SHOPT,	0	},
		{	"sourcepath",				&shell.options.sourcepath,					O_SHOPT,	0	},
		{	"lastpipe",					&shell.options.lastpipe,					O_SHOPT,	0	},
		
		//	=== VARIABLE ===
		{	"assoc_expand_once",		&shell.options.assoc_expand_once,			O_SHOPT,	0	},
		{	"localvar_inherit",			&shell.options.localvar_inherit,			O_SHOPT,	0	},
		{	"localvar_unset",			&shell.options.localvar_unset,				O_SHOPT,	0	},
		
		//	=== PROMPT ===
		{	"promptvars",				&shell.options.promptvars,					O_SHOPT,	0	},
		
		//	=== DEBUG ===
		{	"verbose",					&shell.options.verbose,						O_SET,		'v'	},
		{	"xtrace",					&shell.options.xtrace,						O_SET,		'x'	},
		{	"extdebug",					&shell.options.extdebug,					O_SHOPT,	0	},
		{	"shift_verbose",			&shell.options.shift_verbose,				O_SHOPT,	0	},
		{	"gnu_errfmt",				&shell.options.gnu_errfmt,					O_SHOPT,	0	},
		{	"xpg_echo",					&shell.options.xpg_echo,					O_SHOPT,	0	},
		{	"interactive_comments",		&shell.options.interactive_comments,		O_SET,		0	},
		
		{	NULL,						NULL,										0,			0	}
	};

#pragma endregion

#pragma region "Set"

	int option_set(const char *name, int value, int type) {
		if (!name) return (0);

		for (int i = 0; options[i].name; ++i) {
			if (!strcmp(options[i].name, name)) {
				int match = 0;
				if (type == O_SHOPT)	match = (options[i].type == O_SHOPT);
				if (type == O_SET)		match = (options[i].type == O_SET);
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

#pragma endregion

#pragma region "Get"

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

	#pragma region "Type Match"

		static int options_type_match(int requested, int option_type) {
			if (requested == O_SHOPT)	return (option_type == O_SHOPT);
			if (requested == O_SET)		return (option_type == O_SET);

			return (0);
		}

	#pragma endregion

	#pragma region "Append Reusable"

		static int options_append_reusable(char **opt_str, const t_option *opt, int type) {
			int			is_on = *opt->value;
			int			use_set = (type == O_SET);
			const char	*prefix = (use_set) ? "set " : "shopt ";
			const char	*flag = (use_set) ? ((is_on) ? "-o " : "+o ") : ((is_on) ? "-s " : "-u ");

			char *tmp = ft_strjoin_sep(*opt_str, prefix, flag, J_FREE_VAL_1);
			if (!tmp) return (1);
			*opt_str = ft_strjoin_sep(tmp, opt->name, "\n", J_FREE_VAL_1);
			if (!*opt_str) return (1);

			return (0);
		}

	#pragma endregion

	#pragma region "Append Normal"

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

	#pragma endregion

	#pragma region "Option"

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

	#pragma region "Options"

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

	#pragma endregion

#pragma endregion

#pragma region "Initialize"

	void options_initialize() {
		//	=== SHELL ===
		shell.options.allexport					= 0;
		shell.options.errexit					= 0;
		shell.options.errtrace					= 0;
		shell.options.functrace					= 0;
		shell.options.nounset					= 0;
		shell.options.noclobber					= 0;
		shell.options.noexec					= 0;
		shell.options.notify					= 0;
		shell.options.monitor					= 0;
		shell.options.privileged				= 0;
		shell.options.pipefail					= 0;
		shell.options.hashall					= 0;
		shell.options.ignoreeof					= 0;
		shell.options.execfail					= 0;
		shell.options.checkjobs					= 0;
		shell.options.huponexit					= 0;
		shell.options.inherit_errexit			= 0;
		shell.options.login_shell				= 0;
		shell.options.restricted_shell			= 0;

		//	=== READINPUT ===
		shell.options.emacs						= 1;
		shell.options.vi						= 0;
		shell.options.hide_ctrl_chars			= 0;
		shell.options.multiwidth_chars			= 0;

		//	=== HISTORY ===
		shell.options.history					= 0;
		shell.options.histexpand				= 0;
		shell.options.histappend				= 1;
		shell.options.histreedit				= 0;
		shell.options.histverify				= 0;

		//	=== EXPANSION ===
		shell.options.expand_aliases			= 0;
		shell.options.braceexpand				= 0;
		shell.options.noglob					= 0;
		shell.options.dotglob					= 0;
		shell.options.nullglob					= 0;
		shell.options.failglob					= 0;
		shell.options.nocaseglob				= 0;
		shell.options.nocasematch				= 0;
		shell.options.extglob					= 0;
		shell.options.globstar					= 0;
		shell.options.globasciiranges			= 0;
		shell.options.globskipdots				= 0;
		shell.options.extquote					= 0;
		shell.options.patsub_replacement		= 0;

		//	=== DIRECTORY ===
		shell.options.physical					= 0;
		shell.options.cdable_vars				= 0;
		shell.options.autocd					= 0;
		shell.options.cdspell					= 0;
		shell.options.dirspell					= 0;
		shell.options.direxpand					= 0;
		shell.options.checkwinsize				= 0;

		//	=== COMPLETION ===
		shell.options.progcomp					= 0;
		shell.options.progcomp_alias			= 0;
		shell.options.hostcomplete				= 0;
		shell.options.no_empty_cmd_completion	= 0;
		shell.options.complete_fullquote		= 0;
		shell.options.force_fignore				= 0;

		//	=== EXECUTION ===
		shell.options.checkhash					= 0;
		shell.options.sourcepath				= 0;
		shell.options.lastpipe					= 0;

		//	=== VARIABLE ===
		shell.options.assoc_expand_once			= 0;
		shell.options.localvar_inherit			= 0;
		shell.options.localvar_unset			= 0;

		//	=== PROMPT ===
		shell.options.promptvars				= 0;

		//	=== DEBUG ===
		shell.options.verbose					= 0;
		shell.options.xtrace					= 0;
		shell.options.extdebug					= 0;
		shell.options.shift_verbose				= 0;
		shell.options.gnu_errfmt				= 0;
		shell.options.xpg_echo					= 0;
		shell.options.interactive_comments		= 0;
	}

#pragma endregion
