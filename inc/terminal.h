/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:29 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/01 21:06:23 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <termcap.h>
#include <stdbool.h>

#pragma region Enumerators

	enum e_print {
		RESET,
		RESET_PRINT,
		FREE_RESET,
		FREE_RESET_PRINT,
		FREE_JOIN,
		FREE_PRINT,
		JOIN,
		PRINT
	};

#pragma endregion

#pragma region Variables

	extern bool show_control_chars;

#pragma endregion

//	---------- SIGNAL ----------
void	sigquit_handler(int sig);
void	sigint_handler(int sig);
void	initialize_signals();

void	print_banner();
void	print_welcome();
int		print(int fd, char *str, int mode);

char	*readline(const char* prompt);
