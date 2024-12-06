/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:29 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/06 12:01:43 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#include <signal.h>

#pragma endregion

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

#pragma region Methods

	//	---------- BANNER ----------
	void	banner();
	void	welcome();

	//	---------- PRINT -----------
	int		print(int fd, char *str, int mode);

	//	---------- SIGNAL ----------
	void	initialize_signals();
	void	sigint_handler(int sig);
	void	sigquit_handler(int sig);

#pragma endregion
