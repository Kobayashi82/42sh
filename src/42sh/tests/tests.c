/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 20:28:05 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/16 23:16:07 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region No Testing

	#pragma region UserInfo

		void test_userinfo() {
			t_user *user = get_user_by_name("root");
			if (user) {
				printf("Usuario: %s\nHome: %s\nShell: %s\nUID: %d\nGID: %d\n", user->username, user->home, user->shell, user->uid, user->gid);
				free_user(user);
			};
			user = get_user_by_id(getuid());
			if (user) {
				printf("Usuario: %s\nHome: %s\nShell: %s\nUID: %d\nGID: %d\n", user->username, user->home, user->shell, user->uid, user->gid);
				free_user(user);
			};
			char *path = get_home_by_name("root");
			if (path) { printf("%s\n", path); free(path); }
			path = get_home_by_id(getuid());
			if (path) { printf("%s\n", path); free(path); }
		}

	#pragma endregion

#pragma endregion

#pragma region Testing

	#pragma region Variables

		int test_variables(const char **envp, int print_array) {
			printf(W"\t────────────────────────\n"NC);
			printf(C"\tVariables   ");

			int result = 0, envp_count = 0, vars_count = 0;
			variables_from_array(main_table,envp);
			char **vars = variables_to_array(main_table, EXPORTED);
			for (int i = 0; envp[i]; ++i) ++envp_count;
			for (int i = 0; vars[i]; ++i) ++vars_count;
			if (print_array) {
				array_print(envp, true);
				printf("\n=========================\n\n");
				array_print((const char **)vars, true);
				printf("\n\n");
			}
			array_free(vars);

			if (envp_count != vars_count)																			{ result = 1; printf(RD"X"RED500" clone\n"NC); }
			if (!result && !variables_find(main_table, "PATH"))														{ result = 1; printf(RD"X"RED500" find\n"NC); }
			if (!result) { variables_add(main_table, "testing", "test", true, false, false, false);
				if (!variables_find(main_table, "testing"))															{ result = 1; printf(RD"X"RED500" create\n"NC); }
			}
			if (!result) { variables_delete(main_table, "testing");
				if (variables_find(main_table, "testing"))															{ result = 1; printf(RD"X"RED500" delete\n"NC); }
			}
			if (!result) { variables_clear(main_table);
				vars = variables_to_array(main_table, EXPORTED);
				if (vars && vars[0])																				{ result = 1; printf(RD"X"RED500" clear\n"NC); }
			}

			if (!result) printf(G"✓"GREEN500" passed\n"NC);

			return (result);
		}

	#pragma endregion

	#pragma region Options

		int test_options() {
			printf(W"\t────────────────────────\n"NC);
			printf(C"\tOptions     ");

			int result = 0;

			if (!options_set(NULL, 0) || !options_set("none", 0))													{ result = 1; printf(RD"X"RED500" invalid\n"NC); }
			if (!result && ((options_set("emacs", 0) || options_set("vi", 0)) || (options.emacs || options.vi)))	{ result = 1; printf(RD"X"RED500" set option\n"NC); }

			if (!result) printf(G"✓"GREEN500" passed\n"NC);

			return (result);
		}

	#pragma endregion

	#pragma region History

		int test_history() {
			printf(W"\t────────────────────────\n"NC);
			printf(C"\tHistory     ");

			int		result = 0;
			char	*add_line = "Testing";
			char	*replace_line = "Testing Replaced";

			history_clear();
			if (history_read("history"))									{ result = 1; printf(RD"X"RED500" load\n"NC); }
			if (!result && !history_add(add_line, false)) {
				if (!history_current()->line)								{ result = 1; printf(RD"X"RED500" add\n"NC); }
				else if (ft_strcmp(history_current()->line, add_line))		{ result = 1; printf(RD"X"RED500" add\n"NC); }
			}
			if (!result) { history_replace(history_get_pos(), replace_line, NULL);
				if (!history_current()->line)								{ result = 1; printf(RD"X"RED500" replace\n"NC); }
				else if (ft_strcmp(history_current()->line, replace_line))	{ result = 1; printf(RD"X"RED500" replace\n"NC); }
			}
			if (!result) { size_t length = history_length(); history_remove_current();
				if (history_length() == length)								{ result = 1; printf(RD"X"RED500" delete\n"NC); }
			}
			if (!result) { history_clear();
				if (history_current() || history_length())					{ result = 1; printf(RD"X"RED500" clear\n"NC); }
			}

			if (!result) printf(G"✓"GREEN500" passed\n"NC);

			return (result);
		}

	#pragma endregion

#pragma endregion

#pragma region Test

	int tests(int argc, const char **argv, const char **envp) {
		if (argc == 1 || (ft_strcmp(argv[1], "test") && ft_strcmp(argv[1], "tests"))) return (0);
		int failed = 0;
		printf(W"\t────────────────────────\n"NC);
		printf ("\t\033[7;36m\033[40m          TESTS         \033[7;36m\033[40m\n"NC);

		if (test_options()) failed = 1;
		if (test_variables(envp, false)) failed = 1;
		if (test_history()) failed = 1;

		printf(W"\t────────────────────────\n"NC); fflush(stdout);

		shell.exit_code = failed;
		data_free();
		return (1);
	}

#pragma endregion
