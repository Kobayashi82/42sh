/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 12:18:06 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/23 23:27:35 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Print

	//	Print text in the specified FD
	int print(int fd, char *str, int mode) {
		int	result = 0;

		if ((mode >= RESET && mode <= FREE_RESET_PRINT) && terminal.msg[fd]) {
			free(terminal.msg[fd]);
			terminal.msg[fd] = NULL;
		}

		if (str && !terminal.msg[fd])		terminal.msg[fd] = safe_strdup(str);
		else if (str && terminal.msg[fd])	terminal.msg[fd] = ft_strjoin(terminal.msg[fd], str, 1);

		if (mode % 2 && terminal.msg[fd] && fd > 0) {
			result = write(fd, terminal.msg[fd], ft_strlen(terminal.msg[fd]));
			free(terminal.msg[fd]);
			terminal.msg[fd] = NULL;
		}
		
		if ((mode >= FREE_RESET && mode <= FREE_PRINT) && str) free(str);
		return (result == -1);
	}

#pragma endregion
