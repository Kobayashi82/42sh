/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 20:59:56 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/07 16:14:12 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		enum e_builtin_help {
			HELP_SYNTAX,
			HELP_DESCRIPTION,
			HELP_NORMAL,
			HELP_MANPAGE
		};

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_arg t_arg;
		typedef struct s_builtin{
			char				*name;
			int					disabled;
			int					special;
			int         		(*execute)(int argc, char **argv);
			int         		(*help)(int format, int no_print);
			struct s_builtin	*next;
		} t_builtin;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	//	======= BUILTIN_HASH =======
	t_builtin	*builtin_find(const char *name);
	int			builtin_isactive(const char *name);
	char		**builtin_to_array(int disabled, int special, int sort);
	int			builtin_print(int disabled, int special, int sort);
	size_t		builtin_length(int disabled, int special);
	void		builtin_clear();
	int			builtin_delete(const char *name);
	int			builtin_initialize();
	int			builtin_exec(int argc, char **argv);

	//	====== BUILTINS (42SH) =====
	int			bt_banner(int argc, char **argv);
	int			bt_banner_help(int format, int no_print);
	int			bt_builtin(int argc, char **argv);
	int			bt_builtin_help(int format, int no_print);
	int			bt_declare(int argc, char **argv);
	int			bt_declare_help(int format, int no_print);
	int			bt_typeset_help(int format, int no_print);
	int			bt_dirs(int argc, char **argv);
	int			bt_dirs_help(int format, int no_print);
	int			bt_disown(int argc, char **argv);
	int			bt_disown_help(int format, int no_print);
	int			bt_enable(int argc, char **argv);
	int			bt_enable_help(int format, int no_print);
	int			bt_help(int argc, char **argv);
	int			bt_help_help(int format, int no_print);
	int			bt_history(int argc, char **argv);
	int			bt_history_help(int format, int no_print);
	int			bt_let(int argc, char **argv);
	int			bt_let_help(int format, int no_print);
	int			bt_local(int argc, char **argv);
	int			bt_local_help(int format, int no_print);
	int			bt_logout(int argc, char **argv);
	int			bt_logout_help(int format, int no_print);
	int			bt_mapfile(int argc, char **argv);
	int			bt_mapfile_help(int format, int no_print);
	int			bt_readarray_help(int format, int no_print);
	int			bt_popd(int argc, char **argv);
	int			bt_popd_help(int format, int no_print);
	int			bt_pushd(int argc, char **argv);
	int			bt_pushd_help(int format, int no_print);
	int			bt_shopt(int argc, char **argv);
	int			bt_shopt_help(int format, int no_print);
	int			bt_suspend(int argc, char **argv);
	int			bt_suspend_help(int format, int no_print);

	//	==== BUILTINS (Regular) ====
	int			bt_alias(int argc, char **argv);
	int			bt_alias_help(int format, int no_print);
	int			bt_bg(int argc, char **argv);
	int			bt_bg_help(int format, int no_print);
	int			bt_cd(int argc, char **argv);
	int			bt_cd_help(int format, int no_print);
	int			bt_command(int argc, char **argv);
	int			bt_command_help(int format, int no_print);
	int			bt_echo(int argc, char **argv);
	int			bt_echo_help(int format, int no_print);
	int			bt_false(int argc, char **argv);
	int			bt_false_help(int format, int no_print);
	int			bt_fc(int argc, char **argv);
	int			bt_fc_help(int format, int no_print);
	int			bt_fg(int argc, char **argv);
	int			bt_fg_help(int format, int no_print);
	int			bt_getopts(int argc, char **argv);
	int			bt_hash(int argc, char **argv);
	int			bt_hash_help(int format, int no_print);
	int			bt_jobs(int argc, char **argv);
	int			bt_jobs_help(int format, int no_print);
	int			bt_kill(int argc, char **argv);
	int			bt_kill_help(int format, int no_print);
	int			bt_printf(int argc, char **argv);
	int			bt_printf_help(int format, int no_print);
	int			bt_pwd(int argc, char **argv);
	int			bt_pwd_help(int format, int no_print);
	int			bt_read(int argc, char **argv);
	int			bt_read_help(int format, int no_print);
	int			bt_test(int argc, char **argv);
	int			bt_test_help(int format, int no_print);
	int			bt_bracket_help(int format, int no_print);
	int			bt_true(int argc, char **argv);
	int			bt_true_help(int format, int no_print);
	int			bt_type(int argc, char **argv);
	int			bt_type_help(int format, int no_print);
	int			bt_ulimit(int argc, char **argv);
	int			bt_ulimit_help(int format, int no_print);
	int			bt_umask(int argc, char **argv);
	int			bt_umask_help(int format, int no_print);
	int			bt_unalias(int argc, char **argv);
	int			bt_unalias_help(int format, int no_print);
	int			bt_wait(int argc, char **argv);
	int			bt_wait_help(int format, int no_print);

	//	==== BUILTINS (Special) ====
	int			bt_break(int argc, char **argv);
	int			bt_break_help(int format, int no_print);
	int			bt_continue(int argc, char **argv);
	int			bt_continue_help(int format, int no_print);
	int			bt_dot(int argc, char **argv);
	int			bt_dot_help(int format, int no_print);
	int			bt_source_help(int format, int no_print);
	int			bt_dots(int argc, char **argv);
	int			bt_dots_help(int format, int no_print);
	int			bt_eval(int argc, char **argv);
	int			bt_eval_help(int format, int no_print);
	int			bt_exec(int argc, char **argv);
	int			bt_exec_help(int format, int no_print);
	int			bt_exit(int argc, char **argv);
	int			bt_exit_help(int format, int no_print);
	int			bt_export(int argc, char **argv);
	int			bt_export_help(int format, int no_print);
	int			bt_readonly(int argc, char **argv);
	int			bt_readonly_help(int format, int no_print);
	int			bt_return(int argc, char **argv);
	int			bt_return_help(int format, int no_print);
	int			bt_set(int argc, char **argv);
	int			bt_set_help(int format, int no_print);
	int			bt_shift(int argc, char **argv);
	int			bt_shift_help(int format, int no_print);
	int			bt_times(int argc, char **argv);
	int			bt_times_help(int format, int no_print);
	int			bt_trap(int argc, char **argv);
	int			bt_trap_help(int format, int no_print);
	int			bt_unset(int argc, char **argv);
	int			bt_unset_help(int format, int no_print);

#pragma endregion
