/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 12:18:06 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/26 23:37:48 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/terminal.h"

#pragma endregion

#pragma region "Print"

	//	Print text in the specified FD
	int print(int fd, char *str, int mode) {
		int	result = 0;

		if ((mode >= RESET && mode <= FREE_RESET_PRINT) && terminal.msg[fd]) {
			sfree(terminal.msg[fd]);
			terminal.msg[fd] = NULL;
		}

		if (str && !terminal.msg[fd])		terminal.msg[fd] = ft_strdup(str);
		else if (str && terminal.msg[fd])	terminal.msg[fd] = ft_strjoin(terminal.msg[fd], str, 1);

		if (mode % 2 && terminal.msg[fd] && fd > 0) {
			result = write(fd, terminal.msg[fd], ft_strlen(terminal.msg[fd]));
			sfree(terminal.msg[fd]);
			terminal.msg[fd] = NULL;
		}
		
		if ((mode >= FREE_RESET && mode <= FREE_PRINT) && str) sfree(str);
		return (result == -1);
	}

	void print_reset() {
		for (int i = 0; i < 1024; ++i)
			sfree(terminal.msg[i]);
	}

#pragma endregion
