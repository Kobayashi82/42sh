/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 00:45:44 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/12 12:28:28 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <fcntl.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <string.h>
	#include <ctype.h>
	#include <limits.h>
	#include <errno.h>
	#include <stdio.h>

#pragma endregion

#pragma region "Enumerators"

	enum e_print {
		P_RESET,
		P_RESET_PRINT,
		P_FREE_RESET,
		P_FREE_RESET_PRINT,
		P_FREE_JOIN,
		P_FREE_PRINT,
		P_JOIN,
		P_PRINT,
		P_RESET_ALL
	};

	enum e_sort {
		SORT_NONE,
		SORT_NORMAL,
		SORT_REVERSE
	};

	enum e_join_free {
		J_FREE_NONE			= 294,		// Dont free any value
		J_FREE_VAL1			= 295,		// Free 'value1'
		J_FREE_VAL2			= 296,		// Free 'value2'
		J_FREE_BOTH			= 297,		// Free 'value1' and 'value2'
	};

#pragma endregion

#pragma region "Methods"

	#pragma region "STR"

		#pragma region "IS"

			int		ft_isspace_s(const char *str);
			int		ft_isalnum_s(const char *str);
			int		ft_isnum_s(const char *str);
			int		ft_isdigit_s(const char *str);
			char	*ft_toupper_s(char *str);
			char	*ft_tolower_s(char *str);

		#pragma endregion

		#pragma region "TRIM"

			char	*ft_strtrim(char const *s1, char const *set);
			char	*ft_substr(const char *str, size_t start, int len);

		#pragma endregion

		#pragma region "STRLCPY"

			int		ft_strlcat(char *dst, const char *src, int dstsize);

		#pragma endregion

		#pragma region "STRLCPY"

			int		ft_strlcpy(char *dst, const char *src, int dstsize);

		#pragma endregion

		#pragma region "P_JOIN"

			char	*ft_strjoin(const char *str1, const char *str2, int frees);
			char	*ft_strjoin_sep(const char *s1, const char *sep, const char *s2, int frees);

		#pragma endregion

		#pragma region "STRDUP"

			char	*ft_strdup(const char *s1);
			char	*ft_strndup(const char *s1, size_t n);

		#pragma endregion

		#pragma region "LEN"

			size_t	ft_strlen(const char *str);

		#pragma endregion

		#pragma region "SPLIT"

			char	**ft_split(char *str, char c);

		#pragma endregion

		#pragma region "TOK"

			char	*ft_strtok(char *str, const char *delim, int id);

		#pragma endregion

		#pragma region "REPLACE"

			char	*replace(const char *str, const char *old, const char *new);

		#pragma endregion

		#pragma region "REPLACE SUBSTRING"
		
			char	*replace_substring(char *str, size_t start, size_t len, const char *new);
		
		#pragma endregion

		#pragma region "GET NEXT LINE"

			char	*get_next_line(int fd);

		#pragma endregion

		#pragma region "BEEP"

			void	beep();

		#pragma endregion

		#pragma region "PRINT"

			int		print(int fd, const char *str, int mode);

		#pragma endregion

	#pragma endregion

	#pragma region "NUM"

		int		ft_min(int n1, int n2);
		int		ft_max(int n1, int n2);
		char	*ft_itoa(int n);

	#pragma endregion

	#pragma region "ARRAY"

		void	array_nsort(char **array, int sort, size_t skip);
		void	array_sort(char **array, int sort);
		void	array_int_sort(int *array, int sort);
		char	**array_clone(int length, const char **array);
		void	array_print(const char **array, int fd, int numbered);
		void	array_int_print(int *array, int fd);
		void	array_free(char **array);

	#pragma endregion

	#pragma region "KEY / VALUE"

		int		get_key_value(const char *line, char **key, char **value, char sep);
		char	*format_for_shell(const char *value, char quote_type);

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

	#pragma region "TIME"

		char	*format_timestamp(long timestamp);
		long	get_timestamp(const char *date);
		long	seconds_since(long start_time);

	#pragma endregion

	#pragma region "PATH"

		#define PATH	"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"

		char	*resolve_symlink(const char *path);
		char	*resolve_path(const char *path);
		char	*path_find_first(char *cmd, char *paths);
		char	**path_find_all(char *cmd, char *paths);
		char	*get_fullpath(char *path);
		char	*get_fullpath_command(const char *value, int fullpath);
		char	*get_cwd(char *sender);
		char	*correct_path(char *path);
		int		is_directory(const char *path);

	#pragma endregion

#pragma endregion
