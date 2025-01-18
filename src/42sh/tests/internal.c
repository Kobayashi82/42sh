/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:15:27 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/18 18:55:42 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Alias

	int test_aliases() {
		printf(W"\t────────────────────────\n"NC);
		printf(C"\tAlias       ");

		int result = 0;

		if (!result && alias_find("testing"))		{ result = 1; printf(RD"X"RED500" find\n"NC); }
		if (!result) { alias_add("testing", "ls");
			if (!alias_find("testing"))				{ result = 1; printf(RD"X"RED500" create\n"NC); }
		}

		if (!result) { alias_delete("testing");
			if (alias_find("testing"))				{ result = 1; printf(RD"X"RED500" delete\n"NC); }
		}

		if (!result) { alias_clear();
			char **alias = alias_to_array(true);
			if (alias && alias[0])					{ result = 1; printf(RD"X"RED500" clear\n"NC); }
		}

		if (!result) printf(G"✓"GREEN500" passed\n"NC);

		return (result);
	}

#pragma endregion

#pragma region Variables

	int test_variables(const char **envp) {
		printf(W"\t────────────────────────\n"NC);
		printf(C"\tVariables   ");

		int result = 0, envp_count = 0, vars_count = 0;
		variables_clear(vars_table);
		variables_from_array(vars_table, envp);
		char **vars = variables_to_array(vars_table, EXPORTED, true);
		for (int i = 0; envp[i]; ++i) ++envp_count;
		for (int i = 0; vars[i]; ++i) ++vars_count;
		array_free(vars);

		if (envp_count != vars_count)																			{ result = 1; printf(RD"X"RED500" clone\n"NC); }
		if (!result && !variables_find(vars_table, "PATH"))														{ result = 1; printf(RD"X"RED500" find\n"NC); }
		if (!result) { variables_add(vars_table, "testing", "test", true, false, false, false);
			if (!variables_find(vars_table, "testing"))															{ result = 1; printf(RD"X"RED500" create\n"NC); }
		}

		if (!result) { variables_delete(vars_table, "testing");
			if (variables_find(vars_table, "testing"))															{ result = 1; printf(RD"X"RED500" delete\n"NC); }
		}
		if (!result) { variables_clear(vars_table);
			vars = variables_to_array(vars_table, EXPORTED, true);
			if (vars && vars[0])																				{ result = 1; printf(RD"X"RED500" clear\n"NC); }
		}

		variables_clear(vars_table);

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
