/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 15:57:35 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/05 21:47:16 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Initialize

	void initialize_signals() {
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		data.terminal.signal = 0;
	}

#pragma endregion

#pragma region SIG_INT

	//	Handle SIGINT signal
	void sigint_handler(int sig) {
		data.terminal.signal = sig;
		write(STDIN_FILENO, "\n", 1);
	}

#pragma endregion

#pragma region SIG_QUIT

	//	Handle SIGQUIT signal
	void sigquit_handler(int sig) {
		data.terminal.signal = sig;
		write(1, "Quit\n", 5);
	}

#pragma endregion
