/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:51:23 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/21 21:59:11 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "main/options.h"

#pragma endregion

#pragma region "Variables"

	t_options	options;

#pragma endregion

#pragma region "Set"

	//	Set an option
	int options_set(const char *option, bool value) {
		if (!ft_strcmp("emacs", option)) {
			if (value) options.vi = !value;
			options.emacs = value;	return (0);
		}
		if (!ft_strcmp("vi", option)) {
			if (value) options.emacs = !value;
			options.vi = value;		return (0);
		}
		if (!ft_strcmp("hist_local", option)) 	{ options.hist_local = value;	return (0); }
		if (!ft_strcmp("hist_on", option)) 		{ options.hist_on = value;		return (0); }

		return (1);
	}

#pragma endregion

#pragma region "Print"

	//	Print the option value
	static void print_value(int value, int mode) {
		if (value)	print(STDOUT_FILENO, GREEN500"on\n", mode);
		else		print(STDOUT_FILENO, RED500"off\n", mode);
	}

	//	Print all options
	void options_print() {
		print(STDOUT_FILENO, CYAN300"emacs \t\t", RESET);	print_value(!options.emacs, JOIN);
		print(STDOUT_FILENO, CYAN300"hist_local \t", JOIN);	print_value(options.hist_local, JOIN);
		print(STDOUT_FILENO, CYAN300"hist_on \t", JOIN);	print_value(options.hist_on, JOIN);
		print(STDOUT_FILENO, CYAN300"vi \t\t", JOIN);		print_value(options.vi, PRINT);
	}

#pragma endregion

#pragma region "Initialize"

	//	Set all options to their defaults values
	int options_initialize() {
		options.emacs = false;
		options.vi = true;
		options.hist_local = true;
		options.hist_on = true;

		return (0);
	}

#pragma endregion
