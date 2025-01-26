/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 00:45:44 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/26 12:26:24 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include "colors.h"

	#include <fcntl.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <limits.h>
	#include <stdbool.h>
	#include <errno.h>

#pragma endregion

#pragma region "Methods"

	#pragma region "MEM"

		int		ft_memcmp(const void *s1, const void *s2, int n);
		void	*ft_memcpy(void *dst, const void *src, int n);
		void	*ft_memmove(void *dst, const void *src, int len);
		void	*ft_memchr(const void *s, int c, int n);
		void	*ft_memset(void *b, int c, int len);
		void	*ft_calloc(int count, int size);
		void	*ft_realloc(void *ptr, int old_size, int size);
		void	*smalloc(long bytes);
		void	sfree(void *ptr);
		void	sfree_all();
		int		sopen(const char *file, int oflag, int perm);
		int		sclose(int fd);
		void	sclose_all();
		int		sdup(int fd);
		int		sdup2(int *fd1, int fd2, int closeFD);
		int		spipe(int *fd);
		void	sexit(int code);

	#pragma endregion

	#pragma region "STR"

		#pragma region "IS"

			int		ft_isprint(int c);
			int		ft_isascii(int c);
			int		ft_isalpha(int c);
			int		ft_isalnum(int c);
			int		ft_isdigit(int c);
			int		ft_isxdigit(int c);
			int		ft_isspace(int c);
			int		ft_isalnum_s(char *str);
			int		ft_isdigit_s(char *str);
			int		ft_isspace_s(char *str);
			int		ft_ispunct(const char c);

		#pragma endregion

		#pragma region "TO"

			int		ft_toupper(int c);
			int		ft_tolower(int c);
			char	*ft_toupper_s(char *str);
			char	*ft_tolower_s(char *str);

		#pragma endregion

		#pragma region "CPY"

			char	*ft_strcpy(char *dst, const char *src);
			char	*ft_strncpy(char *dest, const char *src, int n);
			int		ft_strlcpy(char *dst, const char *src, int dstsize);

		#pragma endregion

		#pragma region "CAT"

			char	*ft_strcat(char *dest, const char *src);
			char	*ft_strncat(char *dest, const char *src, int n);
			int		ft_strlcat(char *dst, const char *src, int dstsize);


		#pragma endregion

		#pragma region "CMP"

			int		ft_strcmp(const char *s1, const char *s2);
			int		ft_strncmp(const char *s1, const char *s2, int n);

		#pragma endregion

		#pragma region "CHR"

			char	*ft_strchr(const char *s, int c);
			char	*ft_strrchr(const char *s, int c);

		#pragma endregion

		#pragma region "STR"

			char	*ft_strstr(const char *haystack, const char *needle);
			char	*ft_strnstr(const char *str, const char *substr, int len);

		#pragma endregion

		#pragma region "DUP"

			char	*ft_strdup(const char *s1);
			char	*ft_strndup(const char *s1, int n);

		#pragma endregion

		#pragma region "TRIM"

			char	*ft_strtrim(char const *s1, char const *set);
			char	*ft_substr(const char *str, int start, int len);

		#pragma endregion

		#pragma region "JOIN"

			char	*ft_strjoin(char *str1, char *str2, int frees);
			char	*ft_strjoin_sep(char *s1, char *sep, char *s2, int frees);

		#pragma endregion

		#pragma region "LEN"

			int		ft_strlen(const char *str);
			int		ft_strlenl(const char *str);

		#pragma endregion

		#pragma region "SPLIT"

			char	**ft_split(char *s, char c);

		#pragma endregion

		#pragma region "PUT"

			void	ft_putchar_fd(int fd, char c);
			void	ft_putstr_fd(int fd, char *str, int add_nl);
			void	ft_putnbr_fd(int fd, int n);

		#pragma endregion

		#pragma region "TOK"

			char	*ft_strtok(char *str, const char *delim, int id);

		#pragma endregion

		#pragma region "REPLACE"

			char	*replace(char *str, int *start, int len, char *replace);

		#pragma endregion

	#pragma endregion

	#pragma region "NUM"

		int		ft_max(int n1, int n2);
		void	*ft_max_ptr(void *s1, void *s2);
		int		ft_min(int n1, int n2);
		int		ft_abs(int n);
		int		ft_atoi(const char *str);
		long	ft_atol(char *str);
		char	*ft_itoa(int n);

	#pragma endregion

	#pragma region "ARRAY"

		void	array_nsort(char **array, int skip);
		void	array_sort(char **array);
		void	array_int_sort(int *array);
		void	array_print(const char **array, int numbered);
		void	array_int_print(int *array);
		void	array_free(char **array);
		void	array_free_ptr(char ***array);

	#pragma endregion

	#pragma region "FT_PRINTF"

		int		ft_printf(int fd, char const *format, ...);

	#pragma endregion

	#pragma region "GET_NEXT_LINE"

		char	*get_next_line(int fd);

	#pragma endregion

	#pragma region "USERS"

		typedef struct {
			char	*username;
			char	*home;
			char	*shell;
			int		uid;
			int		gid;
		}	t_userinfo;

		t_userinfo	*get_userinfo();
		t_userinfo	*get_userinfo_by_id(int uid);
		t_userinfo	*get_userinfo_by_name(const char *name);
		char		*get_home();
		char		*get_home_by_id(int uid);
		char		*get_home_by_name(const char *name);
		void		free_user(t_userinfo *user);

	#pragma endregion

	#pragma region "TEMP"

		char	*tmp_find_path(char *path);
		char	*tmp_find_path_fd(int fd);
		int		tmp_find_fd_path(char *path);
		int		tmp_find_fd(int fd);
		void	tmp_add(char *path, int fd);
		void	tmp_close_path(char *path);
		void	tmp_close_fd(int fd);
		void	tmp_delete_path(char *path);
		void	tmp_delete_fd(int fd);
		void	tmp_clear();
		char	*ft_mkdtemp(char *path, char *name);

	#pragma endregion

#pragma endregion
