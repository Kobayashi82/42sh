/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:59:25 by vzurera-          #+#    #+#             */
/*   Updated: 2024/01/18 13:59:25 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c) { return (c >= 32 && c <= 126); }
int	ft_isascii(int c) { return (c >= 0 && c <= 127); }
int	ft_isalpha(int c) { return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); }
int	ft_isalnum(int c) { return (ft_isdigit(c) || ft_isalpha(c)); }
int	ft_isdigit(int c) { return (c >= '0' && c <= '9'); }
int	ft_isxdigit(int c) { return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')); }
int	ft_isspace(int c) { return ((c >= 9 && c <= 13) || c == 32); }
int	ft_isspace_s(char *str) { while (str && *str) { if (!ft_isspace(*str++)) return (0); } return (1); }
int	ft_isalnum_s(char *str) { while (str && *str) { if (!ft_isalnum(*str)) return (0); } return (1); }

int	ft_isdigit_s(char *str) {
	if (str && (*str == '+' || *str == '-')) str++;
	if (!str || *str == '\0') return (0);
	while (*str) { if (!ft_isdigit(*str++)) return (0); } return (1);
}

int	ft_ispunct(const char c) {
	int			i = 0;
	const char	*punct = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
	while (punct[i] != '\0') { if (c == punct[i++]) return (1); } return (0);
}

int		ft_toupper(int c) { if (c >= 'a' && c <= 'z') { c -= 32; } return (c); }
int		ft_tolower(int c) { if (c >= 'A' && c <= 'Z') { c += 32; } return (c); }

char	*ft_toupper_s(char *str) {
	int i = -1;

	while (str[++i]) if (str[i] >= 'a' && str[i] <= 'z') str[i] -= 32;
	return (str);
}

char	*ft_tolower_s(char *str) {
	int	i = -1;

	while (str[++i]) { if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 32; }
	return (str);
}
