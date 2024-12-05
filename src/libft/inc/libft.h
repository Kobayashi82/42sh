/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 00:45:44 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/06 00:23:40 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

//	FT_MEM
void	*ft_memcpy(void *dst, const void *src, int n);
void	*ft_memmove(void *dst, const void *src, int len);
int		ft_memcmp(const void *s1, const void *s2, int n);
void	*ft_memchr(const void *s, int c, int n);
void	*ft_memset(void *b, int c, int len);
void	*ft_calloc(int count, int size);
void	*ft_realloc(void *ptr, int old_size, int size);
//	FT_STR_IS_TO
int		ft_isprint(int c);
int		ft_isascii(int c);
int		ft_isalpha(int c);
int		ft_isalnum(int c);
int		ft_isdigit(int c);
int		ft_isxdigit(int c);
int		ft_isspace(int c);
int		ft_isspace_s(char *str);
int		ft_isalnum_s(char *str);
int		ft_isdigit_s(char *str);
int		ft_ispunct(const char c);
int		ft_toupper(int c);
int		ft_tolower(int c);
char	*ft_toupper_s(char *str);
char	*ft_tolower_s(char *str);
//	FT_STR_PUT
void	ft_putchar_fd(int fd, char c);
void	ft_putstr_fd(int fd, char *str);
void	ft_putendl_fd(int fd, char *str);
void	ft_putnbr_fd(int fd, int n);
int		ft_putstr_fdx(int fd, int count, ...);
//	FT_STR_OP
char	*ft_strcpy(char *dst, const char *src);
char	*ft_strncpy(char *dest, const char *src, int n);
int		ft_strlcpy(char *dst, const char *src, int dstsize);
char	*ft_strcat(char *dest, const char *src);
char	*ft_strncat(char *dest, const char *src, int n);
int		ft_strlcat(char *dst, const char *src, int dstsize);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, int n);
int		ft_strlen(const char *str);
int		ft_strlenl(const char *str);
//	FT_STR_NUM
int		ft_max(int n1, int n2);
int		ft_min(int n1, int n2);
int		ft_abs(int n);
void	*ft_max_ptr(void *s1, void *s2);
int		ft_atoi(const char *str);
long	ft_atol(char *str);
char	*ft_itoa(int n);
//	FT_STRING
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
char	*ft_strstr(const char *haystack, const char *needle);
char	*ft_strnstr(const char *str, const char *substr, int len);
char	*ft_substr(const char *str, int start, int len);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strjoin(char *str1, char *str2, int frees);
char	*ft_strjoin_sep(char *s1, char *sep, char *s2, int frees);
char	*ft_strdup(const char *s1);
char	**ft_split(char *s, char c);
//	FT_PRINTF
int		ft_printf(int fd, char const *format, ...);
//	FT_GET_NEXT_LINE
char	*ft_get_next_line(int fd);
