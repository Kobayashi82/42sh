/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:51:23 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/11 13:56:23 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Variables

	t_options	options;

#pragma endregion

#pragma region Set

	//	Set an option
	int options_set(char *key, char *value) {
		if (!ft_strcmp("input_mode", key)) 	{ options.input_mode = ft_atoi(value);	return (0); }
		if (!ft_strcmp("hist_local", key)) 	{ options.hist_local = ft_atoi(value);	return (0); }
		if (!ft_strcmp("hist_on", key)) 	{ options.hist_on = ft_atoi(value);		return (0); }

		return (1);
	}

#pragma endregion

#pragma region Print

	//	Print the option value
	static void print_value(int value, int mode) {
		if (value)	print(STDOUT_FILENO, GREEN500"on\n", mode);
		else		print(STDOUT_FILENO, RED500"off\n", mode);
	}

	//	Print all options
	void options_print() {
		print(STDOUT_FILENO, CYAN300"emacs \t\t", RESET);	print_value(!options.input_mode, JOIN);
		print(STDOUT_FILENO, CYAN300"hist_local \t", JOIN);	print_value(options.hist_local, JOIN);
		print(STDOUT_FILENO, CYAN300"hist_on \t", JOIN);	print_value(options.hist_on, JOIN);
		print(STDOUT_FILENO, CYAN300"vi \t\t", JOIN);		print_value(options.input_mode, PRINT);
	}

#pragma endregion

#pragma region Initialize

	//	Set all options to their defaults values
	int options_initialize() {
		options.input_mode = READLINE;
		options.hist_local = true;
		options.hist_on = true;

		return (0);
	}

#pragma endregion
