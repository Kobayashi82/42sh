/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 12:49:17 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/07 16:05:11 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/builtin.h"
	#include "main/shell.h"
	#include "utils/libft.h"
	#include "utils/print.h"

	#define BUILTIN_HASH_SIZE	101

#pragma endregion

#pragma region "Variables"

	t_builtin *builtin_table[BUILTIN_HASH_SIZE];

#pragma endregion

#pragma region "Index"

	static unsigned int hash_index(const char *key) {
		unsigned int hash = 0;

		while (*key) hash = (hash * 31) + *key++;
		return (hash % BUILTIN_HASH_SIZE);
	}

#pragma endregion

#pragma region "Import"

	#pragma region "Builtin"

		static int builtin_add(const char *name, int disabled, int special, int (*execute)(int argc, char **argv), int (*help)(int format, int no_print)) {
			if (!name) return (0);

			t_builtin *new_builtin = builtin_find(name);
			if (new_builtin) {
				if (disabled != -1)	new_builtin->disabled = disabled;
				if (special  != -1)	new_builtin->special  = special;
				if (execute)		new_builtin->execute  = execute;
				if (help)			new_builtin->help     = help;

				return (0);
			}

			unsigned int index = hash_index(name);
			new_builtin = calloc(1, sizeof(t_builtin));
			new_builtin->name = ft_strdup(name);

			if (disabled != -1)	new_builtin->disabled = disabled;
			if (special  != -1)	new_builtin->special  = special;
			if (execute)		new_builtin->execute  = execute;
			if (help)			new_builtin->help     = help;
			new_builtin->next = builtin_table[index];
			builtin_table[index] = new_builtin;

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Export"

	#pragma region "Builtin"

		t_builtin *builtin_find(const char *name) {
			if (!name) return (NULL);

			unsigned int index = hash_index(name);
			t_builtin *builtin = builtin_table[index];

			while (builtin) {
				if (!strcmp(builtin->name, name)) return (builtin);
				builtin = builtin->next;
			}

			return (NULL);
		}

		int builtin_isactive(const char *name) {
			if (!name) return (0);

			unsigned int index = hash_index(name);
			t_builtin *builtin = builtin_table[index];

			while (builtin) {
				if (!strcmp(builtin->name, name)) return (!builtin->disabled);
				builtin = builtin->next;
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Array"

		char **builtin_to_array(int disabled, int special, int sort) {
			size_t i = 0;

			for (unsigned int index = 0; index < BUILTIN_HASH_SIZE; index++) {
				t_builtin *builtin = builtin_table[index];
				while (builtin) {
					if (builtin->name && (builtin->disabled == disabled || disabled == 2) && (!special || builtin->special == special)) i++;
					builtin = builtin->next;
				}
			}

			if (i == 0) return (NULL);
			char **array = malloc((i + 1) * sizeof(char *));

			i = 0;
			for (unsigned int index = 0; index < BUILTIN_HASH_SIZE; index++) {
				t_builtin *builtin = builtin_table[index];
				while (builtin) {
					if (builtin->name && (builtin->disabled == disabled || disabled == 2) && (!special || builtin->special == special)) {
						array[i] = ft_strdup(builtin->name);
						i++;
					}
					builtin = builtin->next;
				}
			} array[i] = NULL;

			if (sort) array_sort(array);
			return (array);
		}

	#pragma endregion

	#pragma region "Print"

		int builtin_print(int disabled, int special, int sort) {
			size_t i = 0;

			for (unsigned int index = 0; index < BUILTIN_HASH_SIZE; index++) {
				t_builtin *builtin = builtin_table[index];
				while (builtin) {
					if (builtin->name && (builtin->disabled == disabled || disabled == 2) && (!special || builtin->special == special)) i++;
					builtin = builtin->next;
				}
			}

			if (i == 0) return (1);
			char **array = malloc((i + 1) * sizeof(char *));

			i = 0;
			for (unsigned int index = 0; index < BUILTIN_HASH_SIZE; index++) {
				t_builtin *builtin = builtin_table[index];
				while (builtin) {
				
					if (builtin->name && (builtin->disabled == disabled || disabled == 2) && (!special || builtin->special == special)) {
						if ( builtin->disabled && (disabled == 1 || disabled == 2))
							array[i] = ft_strjoin("disable ", builtin->name, 0);
						if (!builtin->disabled && (disabled == 0 || disabled == 2))
							array[i] = ft_strjoin("enable  ", builtin->name, 0);
						i++;
					}
					builtin = builtin->next;
				}
			} array[i] = NULL;

			if (sort) array_nsort(array, 8);

			if (array && array[0]) {
				print(STDOUT_FILENO, NULL, RESET);
				for (size_t i = 0; array[i]; ++i) {
					print(STDOUT_FILENO, array[i], JOIN);
					print(STDOUT_FILENO, "\n", JOIN);
				}
				print(STDOUT_FILENO, NULL, PRINT);
			}
			if (array) array_free(array);

			return (0);
		}

	#pragma endregion

	#pragma region "Length"

		size_t builtin_length(int disabled, int special) {
			size_t i = 0;

			for (unsigned int index = 0; index < BUILTIN_HASH_SIZE; index++) {
				t_builtin *builtin = builtin_table[index];
				while (builtin) {
					if (builtin->name && (builtin->disabled == disabled || disabled == 2) && (!special || (!special || builtin->special == special))) i++;
					builtin = builtin->next;
				}
			}

			return (i);
		}

	#pragma endregion

#pragma endregion

#pragma region "Delete"

	#pragma region "Builtin"

		int builtin_delete(const char *name) {
			if (!name) return (1);

			unsigned int index = hash_index(name);
			t_builtin *builtin = builtin_table[index];
			t_builtin *prev = NULL;

			while (builtin) {
				if (!strcmp(builtin->name, name)) {
					if (prev)	prev->next = builtin->next;
					else		builtin_table[index] = builtin->next;
					free(builtin->name); free(builtin);
					return (0);
				}
				prev = builtin;
				builtin = builtin->next;
			}
			
			return (1);
		}

	#pragma endregion

	#pragma region "Clear"

		void builtin_clear() {
			for (unsigned int index = 0; index < BUILTIN_HASH_SIZE; index++) {
				if (builtin_table[index]) {
					t_builtin *builtin = builtin_table[index];
					while (builtin) {
						t_builtin *next = builtin->next;
						free(builtin->name);
						free(builtin);
						builtin = next;
					}
					builtin_table[index] = NULL;
				}
			}
		}

	#pragma endregion

#pragma endregion

#pragma region "Initialize"

	// When Bash is executing in POSIX mode, the special builtins differ from other builtin commands in three respects:
	//
	// Special builtins are found before shell functions during command lookup.
	// If a special builtin returns an error status, a non-interactive shell exits.
	// Assignment statements preceding the command stay in effect in the shell environment after the command completes.
	// When Bash is not executing in POSIX mode, these builtins behave no differently than the rest of the Bash builtin commands.
	// The Bash POSIX mode is described in Bash POSIX Mode.

	int builtin_initialize() {
		// 42sh
		builtin_add("banner",		0,	0,	&bt_banner,		&bt_banner_help);
		builtin_add("builtin",		0,	0,	&bt_builtin,	&bt_builtin_help);
		builtin_add("declare",		0,	0,	&bt_declare,	&bt_declare_help);
		builtin_add("typeset",		0,	0,	&bt_declare,	&bt_typeset_help);
		builtin_add("dirs",			0,	0,	&bt_dirs,		&bt_dirs_help);
		builtin_add("disown",		0,	0,	&bt_disown,		&bt_disown_help);
		builtin_add("enable",		0,	0,	&bt_enable,		&bt_enable_help);
		builtin_add("help",			0,	0,	&bt_help,		&bt_help_help);
		builtin_add("history",		0,	0,	&bt_history,	&bt_history_help);
		builtin_add("let",			0,	0,	&bt_let,		&bt_let_help);
		builtin_add("local",		0,	0,	&bt_local,		&bt_local_help);
		builtin_add("logout",		0,	0,	&bt_logout,		&bt_logout_help);
		builtin_add("mapfile",		0,	0,	&bt_mapfile,	&bt_mapfile_help);
		builtin_add("readarray",	0,	0,	&bt_mapfile,	&bt_readarray_help);
		builtin_add("popd",			0,	0,	&bt_popd,		&bt_popd_help);
		builtin_add("pushd",		0,	0,	&bt_pushd,		&bt_pushd_help);
		builtin_add("shopt",		0,	0,	&bt_shopt,		&bt_shopt_help);
		builtin_add("suspend",		0,	0,	&bt_suspend,	&bt_suspend_help);

		// Regular
		builtin_add("alias",		0,	0,	&bt_alias,		&bt_alias_help);
		builtin_add("bg",			0,	0,	&bt_bg,			&bt_bg_help);
		builtin_add("cd",			0,	0,	&bt_cd,			&bt_cd_help);
		builtin_add("command",		0,	0,	&bt_command,	&bt_command_help);
		builtin_add("echo",			0,	0,	&bt_echo,		&bt_echo_help);
		builtin_add("false",		0,	0,	&bt_false,		&bt_false_help);
		builtin_add("fc",			0,	0,	&bt_fc,			&bt_fc_help);
		builtin_add("fg",			0,	0,	&bt_fg,			&bt_fg_help);
		builtin_add("getopts",		0,	0,	&bt_getopts,	&bt_builtin_help);
		builtin_add("hash",			0,	0,	&bt_hash,		&bt_hash_help);
		builtin_add("jobs",			0,	0,	&bt_jobs,		&bt_jobs_help);
		builtin_add("kill",			0,	0,	&bt_kill,		&bt_kill_help);
		builtin_add("printf",		0,	0,	&bt_printf,		&bt_printf_help);
		builtin_add("pwd",			0,	0,	&bt_pwd,		&bt_pwd_help);
		builtin_add("read",			0,	0,	&bt_read,		&bt_read_help);
		builtin_add("test",			0,	0,	&bt_test,		&bt_test_help);
		builtin_add("[",			0,	0,	&bt_test,		&bt_bracket_help);
		builtin_add("true",			0,	0,	&bt_true,		&bt_true_help);
		builtin_add("type",			0,	0,	&bt_type,		&bt_type_help);
		builtin_add("ulimit",		0,	0,	&bt_ulimit,		&bt_ulimit_help);
		builtin_add("umask",		0,	0,	&bt_umask,		&bt_umask_help);
		builtin_add("unalias",		0,	0,	&bt_unalias,	&bt_unalias_help);
		builtin_add("wait",			0,	0,	&bt_wait,		&bt_wait_help);

		// Special
		builtin_add("break",		0,	1,	&bt_break,		&bt_break_help);
		builtin_add("continue",		0,	1,	&bt_continue,	&bt_continue_help);
		builtin_add(".",			0,	1,	&bt_dot,		&bt_dot_help);
		builtin_add("source",		0,	1,	&bt_dot,		&bt_source_help);
		builtin_add(":",			0,	1,	&bt_dots,		&bt_dots_help);
		builtin_add("eval",			0,	1,	&bt_eval,		&bt_eval_help);
		builtin_add("exec",			0,	1,	&bt_exec,		&bt_exec_help);
		builtin_add("exit",			0,	1,	&bt_exit,		&bt_exit_help);
		builtin_add("export",		0,	1,	&bt_export,		&bt_export_help);
		builtin_add("readonly",		0,	1,	&bt_readonly,	&bt_readonly_help);
		builtin_add("return",		0,	1,	&bt_return,		&bt_return_help);
		builtin_add("set",			0,	1,	&bt_set,		&bt_set_help);
		builtin_add("shift",		0,	1,	&bt_shift,		&bt_shift_help);
		builtin_add("times",		0,	1,	&bt_times,		&bt_times_help);
		builtin_add("trap",			0,	1,	&bt_trap,		&bt_trap_help);
		builtin_add("unset",		0,	1,	&bt_unset,		&bt_unset_help);

		return (0);
	}

#pragma endregion

#pragma region "Execute"

	int builtin_exec(int argc, char **argv) {
		if (!argc) return (0);

		t_builtin *builtin = builtin_find(argv[0]);
		if (builtin && !builtin->disabled) {
			int exit_code = builtin->execute(argc, argv);
			if (exit_code != -1) shell.exit_code = exit_code;
			return (1);
		}

		return (0);
	}

#pragma endregion
