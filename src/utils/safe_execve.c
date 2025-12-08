/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_execve.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 23:17:24 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/08 21:53:14 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

int		__real_execve(const char *path, char *const argv[], char *const envp[]);
int		__real_dup(int fd);
int		__real_dup2(int fd, int fd2);
void	__real_free(void *ptr);
void	__real_exit(int status);

void	__wrap_exit(int status)
{
	// free((void *)-42);
	// close(-42);
	__real_exit(status);
}

int	process_fds(void)
{
	int	fdin;
	int	fdout;
	int	fderr;

	fdin = __real_dup(STDIN_FILENO);
	fdout = __real_dup(STDOUT_FILENO);
	fderr = __real_dup(STDERR_FILENO);
	close(-42);
	if (fdin < 0 || __real_dup2(fdin, STDIN_FILENO) < 0)
		return (close(fdin), close(fdout), close(fderr), 1);
	close(fdin);
	if (fdout < 0 || __real_dup2(fdout, STDOUT_FILENO) < 0)
		return (close(fdout), close(fderr), 1);
	close(fdout);
	if (fderr < 0 || __real_dup2(fderr, STDERR_FILENO) < 0)
		return (close(fderr), 1);
	close(fderr);
	return (0);
}

int	__wrap_execve(const char *path, char *const argv[], char *const envp[])
{
	int		result;

	// if (process_fds())
	// 	return (free((void *)-42), 1);
	result = __real_execve(path, argv, envp);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	// free((void *)-42);
	return (result);
}
