/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 20:59:56 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/04 11:42:32 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>

	#define BUILTIN_HASH_SIZE	101

#pragma endregion

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_arg t_arg;
		typedef struct s_builtin{
			char				*name;
			int					disabled;
			int					special;
			int         		(*execute)(int argc, char **argv);
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
	int			bt_banner();
	int			bt_welcome();
	int			bt_builtin(int argc, char **argv);
	int			bt_declare(int argc, char **argv);
	// int			bt_dirs(t_arg *args);
	// int			bt_disown(t_arg *args);
	int			bt_enable(int argc, char **argv);
	// int			bt_help(t_arg *args);
	int			bt_history(int argc, char **argv);
	// int			bt_let(t_arg *args);
	// int			bt_local(t_arg *args);
	int			bt_logout(t_arg *args);
	// int			bt_mapfile(t_arg *args);
	// int			bt_popd(t_arg *args);
	// int			bt_pushd(t_arg *args);
	int			bt_shopt(t_arg *args);
	// int			bt_suspend(t_arg *args);

	//	==== BUILTINS (Regular) ====
	int			bt_alias(t_arg *args);
	// int			bt_bg(t_arg *args);
	int			bt_cd(t_arg *args);
	int			bt_command(t_arg *args);
	int			bt_echo(t_arg *args);
	int			bt_false(t_arg *args);
	int			bt_fc(int argc, char **argv);
	// int			bt_fg(t_arg *args);
	int			bt_getopts(int argc, char **argv);
	int			bt_hash(t_arg *args);
	// int			bt_jobs(t_arg *args);
	// int			bt_kill(t_arg *args);
	// int			bt_printf(t_arg *args);
	int			bt_pwd(int argc, char **argv);
	// int			bt_read(t_arg *args);
	// int			bt_test(t_arg *args);
	int			bt_true(t_arg *args);
	int			bt_type(t_arg *args);
	int			bt_ulimit(int argc, char **argv);
	// int			bt_umask(t_arg *args);
	int			bt_unalias(t_arg *args);
	// int			bt_wait(t_arg *args);

	//	==== BUILTINS (Special) ====
	// int			bt_break(t_arg *args);
	// int			bt_continue(t_arg *args);
	// int			bt_dot(t_arg *args);
	// int			bt_dots(t_arg *args);
	// int			bt_eval(t_arg *args);
	int			bt_exec(t_arg *args);
	int			bt_exit(t_arg *args);
	int			bt_export(t_arg *args);
	int			bt_readonly(t_arg *args);
	// int			bt_return(t_arg *args);
	// int			bt_set(t_arg *args);
	// int			bt_shift(t_arg *args);
	// int			bt_times(t_arg *args);
	// int			bt_trap(t_arg *args);
	int			bt_unset(t_arg *args);

#pragma endregion
