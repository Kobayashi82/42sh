#include <stdint.h>
#include <stdarg.h>
#include <fcntl.h>

int		__real_open(const char *file, int oflag, ...);
int		__real_close(int fd);
int		__real_dup(int fd);
int		__real_dup2(int fd, int fd2);
int		__real_pipe(int pipedes[2]);

static int fd_table[1024];

int	__wrap_open(const char *file, int oflag, ...)
{
	va_list		args;
	int			fd;
	mode_t		mode;

	if (oflag == -42)
		return ((int)(uintptr_t)fd_table);
	if (oflag & O_CREAT)
	{
		va_start(args, oflag);
		mode = va_arg(args, mode_t);
		va_end(args);
		fd = __real_open(file, oflag, mode);
	}
	else
		fd = __real_open(file, oflag);
	if (fd >= 0 && fd < 1024)
		fd_table[fd] = 1;
	return (fd);
}

int	__wrap_close(int fd)
{
	if (fd == -42)
	{
		fd = -1;
		while (++fd < 1024)
		{
			if (fd_table[fd])
			{
				__real_close(fd);
				fd_table[fd] = 0;
			}
		}
	}
	if (fd < 0 || fd > 1023)
		return (1);
	fd_table[fd] = 0;
	return (__real_close(fd));
}
#include <stdio.h>
int	__wrap_dup(int fd)
{
	int	new_fd;

	if (fd < 0)
		return (-1);
	new_fd = __real_dup(fd);
	if (new_fd != -1)
		fd_table[new_fd] = 1;
	return (new_fd);
}

int	__wrap_dup2(int fd, int fd2)
{
	int	new_fd;

	new_fd = -1;
	if (fd == -1)
		return (new_fd);
	if (fd == fd2)
		return (0);
	new_fd = __real_dup2(fd, fd2);
	if (new_fd != -1)
		fd_table[new_fd] = 1;
	return (new_fd);
}

int	__wrap_pipe(int pipedes[2])
{
	if (__real_pipe(pipedes) == -1)
		return (-1);
	fd_table[pipedes[0]] = 1;
	fd_table[pipedes[1]] = 1;
	return (0);
}
