/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 00:45:44 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/18 23:30:03 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <fcntl.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <limits.h>
	#include <stdbool.h>
	#include <errno.h>
	#include <string.h>
	#include <ctype.h>
	#include <stdio.h>

#pragma endregion

#pragma region "Methods"

	#pragma region "STR"

		#pragma region "IS"

			int		ft_isspace_s(char *str);
			int		ft_isalnum_s(char *str);
			int		ft_isdigit_s(char *str);
			char	*ft_toupper_s(char *str);
			char	*ft_tolower_s(char *str);

		#pragma endregion

		#pragma region "TRIM"

			char	*ft_strtrim(char const *s1, char const *set);
			char	*ft_substr(const char *str, size_t start, int len);

		#pragma endregion

		#pragma region "JOIN"

			char	*ft_strjoin(char *str1, char *str2, int frees);
			char	*ft_strjoin_sep(char *s1, char *sep, char *s2, int frees);

		#pragma endregion

		#pragma region "LEN"

			size_t	ft_strlen(const char *str);

		#pragma endregion

		#pragma region "SPLIT"

			char	**ft_split(char *s, char c);

		#pragma endregion

		#pragma region "TOK"

			char	*ft_strtok(char *str, const char *delim, int id);

		#pragma endregion

		#pragma region "REPLACE"

			char	*replace(char *str, size_t *start, size_t len, char *replace);

		#pragma endregion

		#pragma region "REPLACE SUBSTRING"
		
			char *replace_substring(char *original, size_t start, size_t len, const char *replacement);
		
		#pragma endregion
		
		#pragma region "BEEP"

			void	beep();

		#pragma endregion


	#pragma endregion

	#pragma region "NUM"

		int		ft_min(int n1, int n2);
		int		ft_max(int n1, int n2);
		char	*ft_itoa(int n);

	#pragma endregion

	#pragma region "ARRAY"

		void	array_nsort(char **array, size_t skip);
		void	array_sort(char **array);
		void	array_int_sort(int *array);
		void	array_print(const char **array, int numbered);
		void	array_int_print(int *array);
		void	array_free(char **array);
		void	array_free_ptr(char ***array);

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
		char		*get_home_by_id(int uid);
		char		*get_home_by_name(const char *name);
		char		*get_home_by_var();
		char		*get_home();
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
