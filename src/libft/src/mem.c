/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mem.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:22:26 by vzurera-          #+#    #+#             */
/*   Updated: 2024/01/18 13:22:26 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#pragma region HASH

	#pragma region Variables
		
		#define MEM_HASH_SIZE 1031

		enum e_type {
			NO_MEMORY = 90,
			DUP_FAIL = 46,
			DUP2_FAIL = 47,
			PIPE_FAIL = 48
		};

		typedef struct s_mem {
			void			*ptr;
			struct s_mem	*next;
		}	t_mem;

		t_mem	*mem_table[MEM_HASH_SIZE];
		int		fd_table[1024];

	#pragma endregion

	#pragma region Utils

		void sexit(int code) {
			sclose(STDIN_FILENO);
			sclose(STDOUT_FILENO);
			sclose(STDERR_FILENO);
			// sclose_all();
			// sfree_all();
			// tmp_clear();
			exit(code);
		}

		#pragma region Alloc Fail

			static void alloc_fail(int type) {
				if (type == NO_MEMORY)	write(2, "CATASTROPHIC - No memory left on the device\n", 45);
				if (type == DUP_FAIL)	write(2, "CATASTROPHIC - Error duplicating file descriptor\n", 50);
				if (type == DUP2_FAIL)	write(2, "CATASTROPHIC - Error duplicating file descriptor\n", 50);
				if (type == PIPE_FAIL)	write(2, "CATASTROPHIC - Pipe failed\n", 28);
				sexit(1);
			}

		#pragma endregion

		#pragma region Index

			static unsigned int hash_index_ptr(void *ptr) {
				unsigned long addr = (unsigned long)ptr;
				addr = ~addr + (addr << 15);
				addr = addr ^ (addr >> 12);
				addr = addr + (addr << 2);
				addr = addr ^ (addr >> 4);
				addr = addr * 2057;
				addr = addr ^ (addr >> 16);

				return (addr % MEM_HASH_SIZE);
			}

		#pragma endregion

		#pragma region Find

			static t_mem *mem_find(void *ptr) {
				if (!ptr) return (NULL);

				t_mem *mem = mem_table[hash_index_ptr(ptr)];

				while (mem) {
					if (mem->ptr == ptr) return (mem);
					mem = mem->next;
				}

				return (NULL);
			}

		#pragma endregion

	#pragma endregion

	#pragma region Actions

		#pragma region Add

			static void mem_add(void *ptr) {
				if (!ptr) return;

				t_mem *new_mem = mem_find(ptr);
				if (new_mem) {
					free(new_mem->ptr);
					new_mem->ptr = ptr;
				} else {
					unsigned int index = hash_index_ptr(ptr) % MEM_HASH_SIZE;
					t_mem *new_node = malloc(sizeof(t_mem));
					if (!new_node) alloc_fail(NO_MEMORY);

					new_node->ptr = ptr;
					new_node->next = mem_table[index];
					mem_table[index] = new_node;
				}
			}

		#pragma endregion

		#pragma region Delete

			static void mem_delete(void *ptr) {
				if (!ptr) return;

				unsigned int index = hash_index_ptr(ptr);
				t_mem *mem = mem_table[index];
				t_mem *prev = NULL;

				while (mem) {
					if (mem->ptr == ptr) {
						if (prev)	prev->next = mem->next;
						else		mem_table[index] = mem->next;
						free(mem->ptr); free(mem);
						return;
					}
					prev = mem;
					mem = mem->next;
				}
				free(ptr);
			}

		#pragma endregion

		#pragma region Clear

			static void mem_clear() {
				for (unsigned int index = 0; index < MEM_HASH_SIZE; index++) {
					if (mem_table[index]) {
						t_mem *mem = mem_table[index];
						while (mem) {
							t_mem *next = mem->next;
							free(mem->ptr);
							free(mem);
							mem = next;
						}
						mem_table[index] = NULL;
					}
				}
			}

		#pragma endregion

	#pragma endregion

#pragma endregion

#pragma region MEM

	#pragma region MEMCMP

		int	ft_memcmp(const void *s1, const void *s2, int n) {
			unsigned char	*str1 = (unsigned char *)s1;
			unsigned char	*str2 = (unsigned char *)s2;
			int				i = 0;

			while ((*str1 == *str2) && i < n - 1 && n > 0) {
				str1++; str2++; i++;
			}

			if ((!*str1 && !*str2) || i == n) return (0);

			return (*str1 - *str2);
		}

	#pragma endregion

	#pragma region MEMCPY

		void	*ft_memcpy(void *dst, const void *src, int n) {
			unsigned char		*d = (unsigned char *)dst;
			const unsigned char	*s = (const unsigned char *)src;

			if (!n || (!d && !s && n)) return (dst);
			while (n-- > 0) *d++ = *s++;

			return (dst);
		}

	#pragma endregion

	#pragma region MEMMOVE

		void	*ft_memmove(void *dst, const void *src, int len) {
			unsigned char		*d = (unsigned char *)dst;
			const unsigned char	*s = (const unsigned char *)src;

			if (!d && !s && len) return (dst);
			if (d < s)
				while (len-- > 0) *d++ = *s++;
			else {
				d += len - 1;
				s += len - 1;
				while (len-- > 0) *d-- = *s--;
			}

			return (dst);
		}

	#pragma endregion

	#pragma region MEMCHR

		void	*ft_memchr(const void *s, int c, int n) {
			const unsigned char	*str = (const unsigned char *)s;
			int					i = -1;

			while (++i < n)
				if (str[i] == (unsigned char)c) return ((void *)&str[i]);

			return (NULL);
		}

	#pragma endregion

	#pragma region MEMSET

		void	*ft_memset(void *b, int c, int len) {
			unsigned char	*p = b;

			while (len--) *p++ = (unsigned char)c;

			return (b);
		}

	#pragma endregion

#pragma endregion

#pragma region CALLOC

	void	*ft_calloc(int count, int size) {
		int n = count * size - 1;

		char *nuller = smalloc(count * size);
		if (!nuller) return (NULL);
		while (n >= 0) nuller[n--] = '\0';

		return ((void *)nuller);
	}

#pragma endregion

#pragma region REALLOC

	void	*ft_realloc(void *ptr, int old_size, int size) {
		int min_size;

		if (size == 0) return (sfree(ptr), NULL);

		void *new_ptr = smalloc(size);
		if (!new_ptr) return (sfree(ptr), NULL);

		if (ptr) {
			if (old_size < size)	min_size = old_size;
			else					min_size = size;

			ft_memcpy(new_ptr, ptr, min_size);
			sfree(ptr);
		}

		return (new_ptr);
	}

#pragma endregion

#pragma region SMALLOC

	void *smalloc(long bytes) {
		void *ptr;

		if (!(ptr = malloc(bytes))) alloc_fail(NO_MEMORY);

		mem_add(ptr);
		return (ptr);
	}

#pragma endregion

#pragma region SFREE

	void sfree(void *ptr)	{ mem_delete(ptr);	}
	void sfree_all()		{ mem_clear();		}

#pragma endregion


#pragma region SOPEN

	int sopen(const char *file, int oflag, int perm) {
		int fd = -1;

		if (perm == -1)	fd = open(file, oflag);
		else			fd = open(file, oflag, perm);

		if (fd >= 0 && fd < 1024) fd_table[fd] = 1;

		return (fd);
	}

#pragma endregion

#pragma region SCLOSE

	int sclose(int fd) {
		if (fd < 0 || fd > 1023) return (1);
		fd_table[fd] = 0;

		return (close(fd));
	}

	void sclose_all() {
		for (int fd = 0; fd < 1024; ++fd) {
			if (fd_table[fd]) { close(fd); fd_table[fd] = 0; }
		}
	}

#pragma endregion

#pragma region SDUP

	int sdup(int fd) {
		int	new_fd = -1;

		if (fd >= 0 && (new_fd = dup(fd)) == -1) alloc_fail(DUP_FAIL);

		fd_table[new_fd] = 1;
		return (new_fd);
	}

#pragma endregion

#pragma region SDUP2

	int sdup2(int *fd1, int fd2, int closeFD) {
		int	new_fd = -1;

		if (*fd1 == -1)		return (new_fd);
		if (*fd1 == fd2)	return (0);

		new_fd = dup2(*fd1, fd2);
		if (closeFD) { close(*fd1); *fd1 = -1; }
		if (new_fd == -1) alloc_fail(DUP2_FAIL);

		fd_table[new_fd] = 1;
		return (new_fd);
	}

#pragma endregion

#pragma region SPIPE

	int spipe(int *fd) {
		if (pipe(fd) == -1) alloc_fail(PIPE_FAIL);

		fd_table[fd[0]] = 1;
		fd_table[fd[1]] = 1;
		return (0);
	}

#pragma endregion