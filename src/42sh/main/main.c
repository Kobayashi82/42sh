/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:36 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/02 19:48:10 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

t_data	data;

int shell_time() { return (time(NULL) - data.shell.started); }

int read_input() {
	initialize_signals();

	if (!(data.terminal.input = readline(BLUE600"["GREEN600"kobayashi"BLUE600"]"GREEN600"-"RED600"42"Y"sh"BLUE600"> "NC))) return (1);
	if (data.terminal.input) printf("%s\n", data.terminal.input);
	free(data.terminal.input);

	return (0);
}

int initialize(int argc, char **argv, char **envp) {
	(void) argc;
	(void) argv;
	(void) envp;
	data.shell.pid = getpid();
	data.shell.started = time(NULL);
	srand(data.shell.started);
	data.shell.bk_std.bk_stdin = safe_dup(STDIN_FILENO);
	data.shell.bk_std.bk_stdout = safe_dup(STDOUT_FILENO);
	data.shell.bk_std.bk_stderr = safe_dup(STDERR_FILENO);
	return (0);
}

int main(int argc, char **argv, char **envp) {
	if (initialize(argc, argv, envp)) return (1);
	
	if (argc == 2 && !ft_strcmp(argv[1], "-c"))
		exit_error(START_ARGS, 2, NULL, true);
	else if (argc > 1 && ft_strcmp(argv[1], "-c")) ;
		//argument_error(&data, argv[1]);
	else if (argc > 2 && !ft_strcmp(argv[1], "-c")) {
		initialize_signals();
		data.shell._inline = true;
		data.terminal.input = safe_strdup(argv[2]);
	 	if (ft_strlen(data.terminal.input)) printf("inline\n");
	} else {
		print_banner();
		while (!data.shell.exit && !read_input()) ;
	}

	if (data.terminal.signal) data.shell.exit_code = 128 + data.terminal.signal;
	return (data_free(), data.shell.exit_code % 256);
}
