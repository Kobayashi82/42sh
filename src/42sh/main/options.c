/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:51:23 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/25 11:57:02 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/print.h"
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
		if (!ft_strcmp("hist_on", option)) 		{ options.history = value;		return (0); }

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
		print(STDOUT_FILENO, CYAN300"hist_on \t", JOIN);	print_value(options.history, JOIN);
		print(STDOUT_FILENO, CYAN300"vi \t\t", JOIN);		print_value(options.vi, PRINT);
	}

#pragma endregion

#pragma region "Initialize"

	//	Set all options to their defaults values
	int options_initialize() {
		//	READINPUT
		options.emacs				= false;
		options.vi					= true;
		options.hide_ctrl_chars		= false;
		options.multiwidth_chars	= false;

		//	HISTORY
		options.history				= true;
		options.hist_local			= true;

		//	GLOBBING
		options.noglob				= false;
		options.dotglob				= false;
		options.nullglob			= false;
		options.failglob			= false;
		options.nocaseglob			= false;

		//	CD
		options.cdable_vars			= false;
		options.autocd				= false;
		options.dirspell			= false;

		return (0);
	}

#pragma endregion
