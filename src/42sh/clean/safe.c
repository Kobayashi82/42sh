/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:38:00 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/01 20:54:26 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

//	A malloc function with builtin exit_error
void *safe_malloc(long bytes) {
	void	*ptr;

	if (!(ptr = malloc(bytes)))
		exit_error(NO_MEMORY, 1, NULL ,true);

	return (ptr);
}

//	A calloc function with builtin exit_error
void *safe_calloc(int count, long bytes) {

	char	*ptr;
	int		n = count * bytes - 1;

	if (!(ptr = malloc(count * bytes)))
		exit_error(NO_MEMORY, 1, NULL, true);
	while (n >= 0) ptr[n--] = '\0';

	return (ptr);
}

void *safe_realloc(void *ptr, int old_size, int size) {
	void	*new_ptr;
	int		copy_size;

	if (size > 0) {
		new_ptr = malloc(size);
		if (!new_ptr) { free(ptr); exit_error(NO_MEMORY, 1, NULL, true); }
		if (ptr) {
			if (old_size < size)	copy_size = old_size;
			else					copy_size = size;
			ft_memcpy(new_ptr, ptr, copy_size);
			free(ptr);
		}
		return (new_ptr);
	}
	free(ptr);

	return (NULL);
}

//	A strdup function with builtin exit_error
char *safe_strdup(char *str) {
	char	*new_str = NULL;

	if (str && !(new_str = malloc((ft_strlen(str) + 1) * sizeof(char))))
		exit_error(NO_MEMORY, 0, NULL, true);
	if (str && new_str) ft_strcpy(new_str, str);

	return (new_str);
}

//	A dup function with builtin exit_error
int safe_dup(int fd) {
	int	new_fd = -1;

	if (fd >= 0 && (new_fd = dup(fd)) == -1)
		exit_error(DUP_FAIL, 1, NULL, true);

	return (new_fd);
}

//	A dup2 function with builtin exit error
int safe_dup2(int *fd1, int fd2, bool closeFD) {
	int	new_fd = -1;

	if (*fd1 == -1)		return (new_fd);
	if (*fd1 == fd2)	return (0);

	new_fd = dup2(*fd1, fd2);
	if (closeFD) { close(*fd1); *fd1 = -1; }
	if (new_fd == -1) exit_error(DUP2_FAIL, 1, NULL, false);

	return (new_fd);
}
