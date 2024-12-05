/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 20:09:18 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/06 00:21:27 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strchr(const char *str, int c) {
	if (!str) return (NULL);
	while (*str) { if (*str == (char)c) { break; } str++; }
	if (*str == (char)c) return ((char *)str);

	return (NULL);
}

char	*ft_strrchr(const char *str, int c) {
	int len = ft_strlen(str);
	if (!str) return (NULL);
	str += len;
	while (len-- > 0) { if (*str == (char)c) { break; } str--; }
	if (*str == (char)c) return ((char *)str);

	return (NULL);
}

char	*ft_strstr(const char *haystack, const char *needle) {
	int needle_len = ft_strlen(needle);

	if (*needle == '\0') return ((char *)haystack);
	while (*haystack) {
		if (*haystack == *needle && !ft_strncmp(haystack, needle, needle_len)) return ((char *)haystack);
		haystack++;
	}

	return (NULL);
}

char	*ft_strnstr(const char *haystack, const char *needle, int len) {
	int needle_len = ft_strlen(needle);;

	if (*needle == '\0') return ((char *)haystack);
	while (*haystack && len >= needle_len) {
		if (*haystack == *needle && ft_strncmp(haystack, needle, needle_len) == 0) return ((char *)haystack);
		haystack++; len--;
	}

	return (NULL);
}

char	*ft_substr(const char *str, int start, int len) {
	int m_len = 0;

	if (start <= ft_strlen(str)) m_len = ft_strlen(str) - start;
	if (len > m_len) len = m_len;
	len += 1;

	char *new_str = malloc(len);
	if (!new_str) return (NULL);
	if (len > 1) { if (!ft_strlcpy(new_str, str + start, len)) new_str[0] = '\0'; }
	else new_str[0] = '\0';

	return (new_str);
}

char	*ft_strtrim(const char *s1, const char *set) {
	int i = 0, j = ft_strlen(s1) - 1;

	if (!s1 || !*s1) return (NULL);
	while (s1[i] && ft_strchr(set, s1[i])) ++i;
	while (j > i && ft_strchr(set, s1[j])) --j;

	return (ft_substr(s1, i, j - i + 1));
}

char	*ft_strjoin(char *str1, char *str2, int frees) {
	int len = ft_strlen(str1) + ft_strlen(str2) + 1;

	if (!str1 && !str2) return (NULL);

	char *new_str = malloc(len);
	ft_strlcpy (new_str, str1, len);
	ft_strlcat(new_str, str2, len);

	if (frees == 1 || frees == 3) free(str1);
	if (frees == 2 || frees == 3) free(str2);

	return (new_str);
}

char	*ft_strjoin_sep(char *str1, char *sep, char *str2, int frees) {
	int	len = ft_strlen(str1) + ft_strlen(sep) + ft_strlen(str2);;

	if (!str1 && !sep && !str2) return (NULL);

	char *new_str = malloc(len + 1);
	if (new_str && str1) {
		ft_strlcpy(new_str, str1, len + 1);
		if (sep)  ft_strlcat(new_str, sep, len + 1);
		if (str2) ft_strlcat(new_str, str2, len + 1);
	}
	else if (new_str && sep) {
		ft_strlcpy(new_str, sep, len + 1);
		if (str2) ft_strlcat(new_str, str2, len + 1);
	}
	else if (new_str && str2) ft_strlcpy(new_str, str2, len + 1);

	if (str1 && (frees == 1 || frees == 4 || frees == 6 || frees == 7)) free(str1);
	if (sep  && (frees == 2 || frees == 4 || frees == 5 || frees == 7)) free(sep);
	if (str2 && (frees == 3 || frees == 5 || frees == 6 || frees == 7)) free(str2);

	return (new_str);
}

char	*ft_strdup(const char *s1) {
	char	*copy = malloc(ft_strlen(s1) + 1);
	if (!copy) return (NULL);
	copy[0] = '\0';
	ft_strlcpy(copy, s1, ft_strlen(s1) + 1);
	return (copy);
}

static char	**free_all(char **result, int i) {
	if (result) {
		while (i-- > 0) free(result[i]);
		free(result);
	}

	return (NULL);
}

static int	count_words(char *str, char c) {
	int		i = 0, trigger = 0;
	char	*tmp = str;

	while (str && *str) {
		if ((*str != c || (*str == c && str != tmp && *(str - 1) == c)) && trigger == 0) { trigger = 1; i++; }
		else if (*str == c) trigger = 0;
		str++;
	}

	return (i);
}

static char	*word_dup(char *str, int start, int finish, char c) {
	char	*word = malloc((finish - start + 1));
	int		i = 0;

	if (!word) return (NULL);
	if (finish - start == 1 && str[start] == c) return (free(word), ft_strdup(""));
	while (start < finish) word[i++] = str[start++];
	word[i] = '\0';

	return (word);
}

char	**ft_split(char *s, char c) {
	int		i = -1, j = 0, index = -1;
	char	**split = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!s || !split) return (free_all(split, 0));

	while (i + 1 <= ft_strlen(s)) {
		if ((s[++i] != c || (s[i] == c && i > 0 && s[i - 1] == c)) && index < 0) index = i;
		else if ((s[i] == c || i == ft_strlen(s)) && index >= 0) {
			split[j++] = word_dup(s, index, i, c);
			if (split[j - 1] == NULL) return (free_all(split, j));
			index = -1;
		}
	} split[j] = 0;

	return (split);
}
