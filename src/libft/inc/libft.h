/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 00:45:44 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/11 13:23:18 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdlib.h>
#include <unistd.h>

//	FT_MEM
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

//	STR_IS
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
//	STR_TO
int		ft_toupper(int c);
int		ft_tolower(int c);
char	*ft_toupper_s(char *str);
char	*ft_tolower_s(char *str);
//	STR_CPY
char	*ft_strcpy(char *dst, const char *src);
char	*ft_strncpy(char *dest, const char *src, int n);
int		ft_strlcpy(char *dst, const char *src, int dstsize);
//	STR_CAT
char	*ft_strcat(char *dest, const char *src);
char	*ft_strncat(char *dest, const char *src, int n);
int		ft_strlcat(char *dst, const char *src, int dstsize);
//	STR_CMP
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, int n);
//	STR_CHR
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
//	STR_STR
char	*ft_strstr(const char *haystack, const char *needle);
char	*ft_strnstr(const char *str, const char *substr, int len);
//	STR_DUP
char	*ft_strdup(const char *s1);
char	*ft_strndup(const char *s1, int n);
//	STR_TRIM
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_substr(const char *str, int start, int len);
//	STR_JOIN
char	*ft_strjoin(char *str1, char *str2, int frees);
char	*ft_strjoin_sep(char *s1, char *sep, char *s2, int frees);
//	STR_LEN
int		ft_strlen(const char *str);
int		ft_strlenl(const char *str);
//	STR_SPLIT
char	**ft_split(char *s, char c);
//	STR_PUT
void	ft_putchar_fd(int fd, char c);
void	ft_putstr_fd(int fd, char *str, int add_nl);
void	ft_putnbr_fd(int fd, int n);
//	STR_TOK
char	*ft_strtok(char *str, const char *delim);

//	FT_STR_NUM
int		ft_max(int n1, int n2);
void	*ft_max_ptr(void *s1, void *s2);
int		ft_min(int n1, int n2);
int		ft_abs(int n);
int		ft_atoi(const char *str);
long	ft_atol(char *str);
char	*ft_itoa(int n);

//	FT_ARRAY
void	array_nsort(char **array, int skip);
void	array_sort(char **array);
void	array_int_sort(int *array);
void	array_print(const char **array, int numbered);
void	array_int_print(int *array);
void	array_free(char **array);
void	array_free_ptr(char ***array);

//	FT_PRINTF
int		ft_printf(int fd, char const *format, ...);
//	FT_GET_NEXT_LINE
char	*ft_get_next_line(int fd);
