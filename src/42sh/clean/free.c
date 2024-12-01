/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:07:44 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/01 13:12:11 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

// void	clear_tmp(t_data *data)
// {
// 	t_args	*current;
// 	char	*path;

// 	path = "/tmp/hd_*";
// 	if (data->main)
// 	{
// 		wildcards(data, &path);
// 		current = data->files;
// 		while (current)
// 		{
// 			unlink(current->arg);
// 			current = current->next;
// 		}
// 	}
// }

// //	Free the data structure
// void	data_free() {
// 	clear_tmp(data);
// 	environment_clear(&data->envp);
// 	shell_clear(&data->vars);
// 	args_list_clear(&data->files);
// 	tokens_clear(data);
// 	if (data->input)
// 		free(data->input);
// 	if (data->msg)
// 		free(data->msg);
// 	close(data->bk_in);
// 	close(data->bk_out);
// 	close(data->bk_err);
// }

// //	Free a char array
// void	free_array(char **array)
// {
// 	char	**original_array;

// 	original_array = array;
// 	while (array && *array)
// 		free(*(array++));
// 	if (original_array)
// 		free(original_array);
// }


//	Free the data structure
void data_free() {
	// clear_tmp(data);
	// environment_clear(&data->envp);
	// shell_clear(&data->vars);
	// args_list_clear(&data->files);
	// tokens_clear(data);
	free(data.terminal.input);
	free(data.terminal.msg);
	close(data.shell.bk_std.bk_stdin);
	close(data.shell.bk_std.bk_stdout);
	close(data.shell.bk_std.bk_stderr);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}
