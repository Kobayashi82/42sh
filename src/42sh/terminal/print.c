/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 12:18:06 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/06 22:13:59 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Print

	//	Print text in the specified FD
	int print(int fd, char *str, int mode) {
		int	result = 0;

		if ((mode >= RESET && mode <= FREE_RESET_PRINT) && data.terminal.msg) {
			free(data.terminal.msg);
			data.terminal.msg = NULL;
		}

		if (str && !data.terminal.msg)		data.terminal.msg = safe_strdup(str);
		else if (str && data.terminal.msg)	data.terminal.msg = ft_strjoin(data.terminal.msg, str, 1);

		if (mode % 2 && data.terminal.msg && fd > 0) {
			result = write(fd, data.terminal.msg, ft_strlen(data.terminal.msg));
			free(data.terminal.msg);
			data.terminal.msg = NULL;
		}
		
		if ((mode >= FREE_RESET && mode <= FREE_PRINT) && str) free(str);
		return (result == -1);
	}

#pragma endregion
