/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 20:09:18 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/08 15:18:09 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"

#pragma endregion

#pragma region "STR_STRING"


	int	ft_isspace_s(const char *str) { while (str && *str) { if (!isspace(*str++)) return (0); } return (1); }
	int	ft_isalnum_s(const char *str) { while (str && *str) { if (!isalnum(*str++)) return (0); } return (1); }

	int	ft_isnum_s(const char *str) {
		if (!str) return (0);

		while (*str) {
			if (!isdigit(*str++)) return (0);
		}

		return (1);
	}

	int	ft_isdigit_s(const char *str) {
		if (str && (*str == '+' || *str == '-')) str++;
		if (!str || *str == '\0') return (0);
		while (*str) {
			if (!isdigit(*str++)) return (0);
		}
		
		return (1);
	}

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

#pragma endregion

#pragma region "STR_TRIM"

	#pragma region "StrTrim"

		char	*ft_strtrim(const char *s1, const char *set) {
			int i = 0, j = ft_strlen(s1) - 1;

			if (!s1 || !*s1) return (NULL);
			while (s1[i] && strchr(set, s1[i])) ++i;
			while (j > i && strchr(set, s1[j])) --j;

			return (ft_substr(s1, i, j - i + 1));
		}

	#pragma endregion

	#pragma region "SubStrTrim"

		char	*ft_substr(const char *str, size_t start, int len) {
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

	#pragma endregion

#pragma endregion

#pragma region "StrLCat"

	int	ft_strlcat(char *dst, const char *src, int dstsize) {
		int src_len = 0, dest_len = 0, i;

		while (dst[dest_len] && dest_len < dstsize) dest_len++;
		while (src[src_len]) src_len++;
		if (dstsize == 0 || dest_len >= dstsize) return (dest_len + src_len);
		i = dest_len - 1;
		while (++i < dstsize - 1 && src[i - dest_len]) dst[i] = src[i - dest_len];
		dst[i] = '\0';
		return (dest_len + src_len);
	}

#pragma endregion

#pragma region "StrLCpy"

	int	ft_strlcpy(char *dst, const char *src, int dstsize) {
		int		i = 0, srclen = 0;

		if (!src) return (0);
		while (src[srclen]) ++srclen;
		if (dstsize > 0) {
			while (dstsize > 0 && src[i] && i < dstsize - 1) { dst[i] = src[i]; ++i; }
			dst[i] = '\0';
		}

		return (srclen);
	}

#pragma endregion

#pragma region "STR_JOIN"

	#pragma region "StrJoin"

		char	*ft_strjoin(char *str1, char *str2, int frees) {
			if (!str1 && !str2) return (NULL);

			int len = ft_strlen(str1) + ft_strlen(str2) + 1;
			char *new_str = malloc(len);
			if (!new_str) return (NULL);
			if (str1) {
				ft_strlcpy(new_str, str1, len);
				if (str2) ft_strlcat(new_str, str2, len);
			} else if (str2) strcpy(new_str, str2);

			if (frees == 1 || frees == 3) free(str1);
			if (frees == 2 || frees == 3) free(str2);

			return (new_str);
		}

	#pragma endregion

	#pragma region "StrJoinSep"

		char	*ft_strjoin_sep(char *str1, char *sep, char *str2, int frees) {
			int	len = ft_strlen(str1) + ft_strlen(sep) + ft_strlen(str2);

			if (!str1 && !sep && !str2) return (NULL);

			char *new_str = malloc(len + 1);
			if (str1) {
				ft_strlcpy(new_str, str1, len + 1);
				if (sep)  ft_strlcat(new_str, sep, len + 1);
				if (str2) ft_strlcat(new_str, str2, len + 1);
			} else if (sep) {
				ft_strlcpy(new_str, sep, len + 1);
				if (str2) ft_strlcat(new_str, str2, len + 1);
			} else if (str2) ft_strlcpy(new_str, str2, len + 1);

			if (str1 && (frees == 1 || frees == 4 || frees == 6 || frees == 7)) free(str1);
			if (sep  && (frees == 2 || frees == 4 || frees == 5 || frees == 7)) free(sep);
			if (str2 && (frees == 3 || frees == 5 || frees == 6 || frees == 7)) free(str2);

			return (new_str);
		}

	#pragma endregion

#pragma endregion

#pragma region "STR_DUP"

	#pragma region "StrDup"

		char	*ft_strdup(const char *s1) {
			if (!s1) return (NULL);

			char *copy = malloc(ft_strlen(s1) + 1);
			if (!copy) return (NULL);

			ft_strlcpy(copy, s1, ft_strlen(s1) + 1);
			return (copy);
		}

	#pragma endregion

	#pragma region "StrNDup"

		char	*ft_strndup(const char *s1, size_t n) {
			if (!s1 || n == 0) return (NULL);

			size_t len = 0;
    		while (len < n && s1[len] != '\0') len++;

			char *copy = malloc(len + 1);
			if (!copy) return (NULL);

			ft_strlcpy(copy, s1, len + 1);
			return (copy);
		}

	#pragma endregion

#pragma endregion

#pragma region "STR_LEN"

	size_t ft_strlen(const char *str) {
		size_t i = 0;

		while (str && str[i]) ++i;
		return (i);
	}

#pragma endregion

#pragma region "STR_SPLIT"

	#pragma region "Free All"

		static char	**free_all(char **result, int i) {
			if (result) {
				while (i-- > 0) free(result[i]);
				free(result);
			}

			return (NULL);
		}

	#pragma endregion

	#pragma region "Count Words"

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

	#pragma endregion

	#pragma region "Word Dup"

		static char	*word_dup(char *str, int start, int finish, char c) {
			char	*word = malloc((finish - start + 1));
			int		i = 0;

			if (!word) return (NULL);
			if (finish - start == 1 && str[start] == c) return (free(word), ft_strdup(""));
			while (start < finish) word[i++] = str[start++];
			word[i] = '\0';

			return (word);
		}

	#pragma endregion

	#pragma region "Split"

		char	**ft_split(char *s, char c) {
			int		i = -1, j = 0, index = -1;
			char	**split = malloc((count_words(s, c) + 1) * sizeof(char *));
			if (!s || !split) return (free_all(split, 0));

			while (i + 1 <= (int) ft_strlen(s)) {
				if ((s[++i] != c || (s[i] == c && i > 0 && s[i - 1] == c)) && index < 0) index = i;
				else if ((s[i] == c || i == (int) ft_strlen(s)) && index >= 0) {
					split[j++] = word_dup(s, index, i, c);
					if (split[j - 1] == NULL) return (free_all(split, j));
					index = -1;
				}
			} split[j] = 0;

			return (split);
		}

	#pragma endregion

#pragma endregion

#pragma region "STR_TOK"

	char *ft_strtok(char *str, const char *delim, int id) {
		static char *static_str[100];
		char *token = NULL;

		if (id < 0) id = 0;
		if (id > 99) id = 99;

		if (str) static_str[id] = str;
		if (!static_str[id]) return (NULL);

		while (*static_str[id] && strchr(delim, *static_str[id])) static_str[id]++;
		if (!*static_str[id]) { static_str[id] = NULL; return (NULL); }

		token = static_str[id];
		while (*static_str[id] && !strchr(delim, *static_str[id])) static_str[id]++;

		if (*static_str[id]) { *static_str[id] = '\0'; static_str[id]++; }

		return (token);
	}

#pragma endregion

#pragma region "REPLACE"

	//	Replace a string inside a string with another string XD
	char *replace(char *str, size_t *start, size_t len, char *replace) {
		if (len <= 0) return (str);
		size_t i = (start) ? *start : 0;
		size_t str_len = ft_strlen(str);
		size_t replace_len = ft_strlen(replace);
		size_t new_len = str_len - len + replace_len;
		char *new_str = malloc(new_len + 1);
		
		memcpy(new_str, str, i);														//	Copy the part of the original string before the replacement
		memcpy(new_str + i, replace, replace_len);									//	Copy the replacement string
		memcpy(new_str + i + replace_len, str + i + len, str_len - (i + len));		//	Copy the part of the original string after the replacement
		new_str[new_len] = '\0';														//	Add the null terminator at the end of the new string

		if (start) *start = i + replace_len;											//	Update the start index if needed

		return (free(str), new_str);
	}

#pragma endregion

#pragma region "REPLACE SUBSTRING"

	char *replace_substring(char *original, size_t start, size_t len, const char *replacement) {
		size_t orig_len = ft_strlen(original);
		size_t repl_len = ft_strlen(replacement);
		
		if (start > orig_len)		return (NULL);
		if (start + len > orig_len)	len = orig_len - start;
		
		char *new_str = malloc(orig_len - len + repl_len + 1);
		
		strncpy(new_str, original, start);
		new_str[start] = '\0';
		strcat(new_str, replacement);
		if (start + len < orig_len) strcat(new_str, original + start + len);
		
		return (new_str);
	}

#pragma endregion

#pragma region "BEEP"

	void beep() { write(STDOUT_FILENO, "\a", 1); }

#pragma endregion
