/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:38:00 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/01 13:07:32 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

char *safe_strdup(char *str) {
	char	*new_str = NULL;

	if (str && !(new_str = malloc((ft_strlen(str) + 1) * sizeof(char))))
		exit_error(NO_MEMORY, 0, NULL, true);
	if (str && new_str) ft_strcpy(new_str, str);
	return (new_str);
}

int safe_dup(int fd) {
	int	new_fd = -1;

	if (fd >= 0) new_fd = dup(fd);
	if (new_fd == -1)
		exit_error(DUP_FAIL, 1, NULL, true);
	return (new_fd);
}
