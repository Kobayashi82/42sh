/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 15:57:35 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/11 14:02:54 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

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

#pragma region Set

	void signals_set() {
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		terminal.signal = 0;
	}

#pragma endregion
