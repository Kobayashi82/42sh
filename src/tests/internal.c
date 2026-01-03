/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:15:27 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/03 23:35:18 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "terminal/colors.h"
	#include "terminal/terminal.h"
	#include "terminal/readinput/history.h"
	#include "tests/args.h"
	#include "expansion/globbing.h"
	#include "hashes/alias.h"
	#include "hashes/variable.h"
	#include "main/options.h"
	#include "tests/tests.h"
	
	#include <stdio.h>

#pragma endregion

#pragma region "Alias"

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
			char **alias = alias_to_array(1);
			if (alias && alias[0])					{ result = 1; printf(RD"X"RED500" clear\n"NC); }
		}

		if (!result) printf(G"✓"GREEN500" passed\n"NC);

		return (result);
	}

#pragma endregion

#pragma region "Variables"

	int test_variables(const char **envp) {
		printf(W"\t────────────────────────\n"NC);
		printf(C"\tVariables   ");

		int result = 0, envp_count = 0, vars_count = 0;
		variables_clear(vars_table);
		variables_from_array(vars_table, envp);
		char **vars = variables_to_array(vars_table, EXPORTED, 1);
		for (int i = 0; envp[i]; ++i) ++envp_count;
		for (int i = 0; vars[i]; ++i) ++vars_count;
		array_free(vars);

		if (envp_count != vars_count)																			{ result = 1; printf(RD"X"RED500" clone\n"NC); }
		if (!result && !variables_find(vars_table, "PATH"))														{ result = 1; printf(RD"X"RED500" find\n"NC); }
		if (!result) { variables_add(vars_table, "testing", "test", 1, 0, 0, 0);
			if (!variables_find(vars_table, "testing"))															{ result = 1; printf(RD"X"RED500" create\n"NC); }
		}

		if (!result) { variables_delete(vars_table, "testing");
			if (variables_find(vars_table, "testing"))															{ result = 1; printf(RD"X"RED500" delete\n"NC); }
		}
		if (!result) { variables_clear(vars_table);
			vars = variables_to_array(vars_table, EXPORTED, 1);
			if (vars && vars[0])																				{ result = 1; printf(RD"X"RED500" clear\n"NC); }
		}

		variables_clear(vars_table);

		if (!result) printf(G"✓"GREEN500" passed\n"NC);

		return (result);
	}

#pragma endregion

#pragma region "Options"

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

#pragma region "History"

	int test_history() {
		printf(W"\t────────────────────────\n"NC);
		printf(C"\tHistory     ");

		int		result = 0;
		char	*add_line = "Testing";

		history_clear();
		if (history_read("history"))											{ result = 1; printf(RD"X"RED500" load\n"NC); }
		if (!result && !history_add(add_line, 0)) {
			if (!history_entry_current()->line)									{ result = 1; printf(RD"X"RED500" add\n"NC); }
			else if (strcmp(history_entry_current()->line, add_line))			{ result = 1; printf(RD"X"RED500" add\n"NC); }
		}
		if (!result) { size_t length = history_length(); history_remove_current(0);
			if (history_length() == length)										{ result = 1; printf(RD"X"RED500" delete\n"NC); }
		}
		if (!result) { history_clear();
			if (history_entry_current() || history_length())					{ result = 1; printf(RD"X"RED500" clear\n"NC); }
		}

		if (!result) printf(G"✓"GREEN500" passed\n"NC);

		return (result);
	}

#pragma endregion

#pragma region "Globbing"

	int test_globbing() {
		printf(W"\t────────────────────────\n"NC);
		printf(C"\tGlobbing    ");

		int result = 0;

		char *pattern = "./\\M?*************'k'[aaaa,,,,,b,c-z]?***********?l[!]\\]a\\-df-z]***************";

		t_arg *args = calloc(1, sizeof(t_arg));
		args->value = ft_strdup(pattern);
		globbing(args);

		if (!strcmp(args->value, pattern)) { result = 1; printf(RD"X"RED500" pattern\n"NC); }
		
		if (!result) printf(G"✓"GREEN500" passed\n"NC);

		args_clear(&args);

		return (result);
	}

#pragma endregion
