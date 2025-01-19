/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 15:57:35 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/19 20:24:53 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "terminal/terminal.h"

#pragma region SIG_INT

	//	Handle SIGINT signal
	void sigint_handler(int sig) {
		terminal.signal = sig;
		write(STDIN_FILENO, "\n", 1);
	}

#pragma endregion

#pragma region SIG_QUIT

	//	Handle SIGQUIT signal
	void sigquit_handler(int sig) {
		terminal.signal = sig;
		write(1, "Quit\n", 5);
	}

#pragma endregion

#pragma region SIG_WINCH

	//	Handle SIGWINCH signal
	void sigwinch_handler(int sig) {
		terminal.signal = sig;
		terminal_initialize();
		cursor_get();			// Esto solo cuando se esté en modo interactivo
	}

#pragma endregion

#pragma region Set

	void signals_set() {
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGWINCH, sigwinch_handler);
		terminal.signal = 0;
	}

#pragma endregion
