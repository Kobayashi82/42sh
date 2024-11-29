/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 00:45:44 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/22 14:43:06 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdarg.h>
# include <limits.h>
# include <errno.h>
# include <string.h>

# ifndef GNL_BUFFER
#  define GNL_BUFFER 256
# endif

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

//	FT_STR_CAT.C
char	*ft_strcat(char *dest, const char *src);
char	*ft_strncat(char *dest, const char *src, int n);
int		ft_strlcat(char *dst, const char *src, int dstsize);
//	FT_STR_CHR.C
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
//	FT_STR_CMP.C
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, int n);
//	FT_STR_CPY.C
char	*ft_strcpy(char *dst, const char *src);
char	*ft_strncpy(char *dest, const char *src, int n);
int		ft_strlcpy(char *dst, const char *src, int dstsize);
//	FT_STR_DUB.C
char	*ft_strdup(const char *s1);
//	FT_STR_JOIN.C
char	*ft_strjoin(char *s1, char *s2, int frees);
char	*ft_strjoin_c(char *s1, char *s2, char *c, int frees);
//	FT_STR_LEN.C
int		ft_strlen(const char *s);
int		ft_strlenl(char *str);
//	FT_STR_NUM_1.C
int		ft_atoi(const char *str);
char	*ft_itoa(int n);
//	FT_STR_NUM_2.C
long	ft_atol_base(const char *str, char **endptr, int base);
long	ft_atol(char *str);
//	FT_STR_PUT.C
void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putstr_fd(char *s, int fd);
int		ft_putstr_fdx(int fd, int count, ...);
void	ft_putnbr_fd(int n, int fd);
//	FT_STR_TO.C
int		ft_tolower(int c);
char	*ft_tolower_s(char *str);
int		ft_toupper(int c);
char	*ft_toupper_s(char *str);
//	FT_STR_SPLIT.C
char	**ft_split(char const *s, char c);
//	FT_STR_STR.C
char	*ft_strstr(const char *haystack, const char *needle);
char	*ft_strnstr(const char *str, const char *substr, int len);
//	FT_STR_SUB.C
char	*ft_substr(char const *s, unsigned int start, int len);
//	FT_STR_TRIM.C
char	*ft_strtrim(char const *s1, char const *set);
//	FT_STR_IS_1.C
int		ft_isprint(int c);
int		ft_isascii(int c);
int		ft_isalpha(int c);
int		ft_isalnum(int c);
int		ft_isalnum_s(char *str);
//	FT_STR_IS_2.C
int		ft_isdigit(int c);
int		ft_isdigit_s(char *str);
int		ft_isxdigit(int c);
int		ft_isspace(int c);
int		ft_isspace_s(char *str);
//	FT_MEM.C
void	*ft_memcpy(void *dst, const void *src, int n);
void	*ft_memmove(void *dst, const void *src, int len);
int		ft_memcmp(const void *s1, const void *s2, int n);
void	*ft_memchr(const void *s, int c, int n);
void	*ft_memset(void *b, int c, int len);
//	FT_REALLOC.C
void	*ft_realloc_2(void *ptr, int size);
void	*ft_realloc(void *ptr, int old_size, int size);
void	*ft_calloc(int count, int size);
//	FT_LIST_1.C
t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
//	FT_LIST_2.C
void	ft_lstdel_last(t_list **lst, void (*del)(void*));
int		ft_lstsize(t_list *lst);
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
//	FT_MATHS.C
int		ft_max(int n1, int n2);
int		ft_min(int n1, int n2);
int		ft_abs(int n);
char	*ft_max_ptr(char *s1, char *s2);
//	FT_GET_NEXT_LINE.C
char	*ft_get_next_line(int fd);
//	FT_PRINTF.C
int		ft_printf(char const *format, ...);
int		ft_dprintf(int fd, char const *format, ...);

#endif