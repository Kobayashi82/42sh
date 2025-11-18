/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 20:09:18 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/18 23:03:59 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include <string.h>

#pragma endregion

#pragma region "STR_IS"

	#pragma region "Char"

		#pragma region "Is_Print"

			int	ft_isprint(int c) { return (c >= 32 && c <= 126); }

		#pragma endregion

		#pragma region "Is_ASCII"

			int	ft_isascii(int c) { return (c >= 0 && c <= 127); }

		#pragma endregion

		#pragma region "Is_Alpha"

			int	ft_isalpha(int c) { return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); }

		#pragma endregion

		#pragma region "Is_AlphaNum"

			int	ft_isalnum(int c) { return (ft_isdigit(c) || ft_isalpha(c)); }

		#pragma endregion

		#pragma region "Is_Digit"

			int	ft_isdigit(int c) { return (c >= '0' && c <= '9'); }

		#pragma endregion

		#pragma region "Is_Hex"

			int	ft_isxdigit(int c) { return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')); }

		#pragma endregion

		#pragma region "Is_Space"

			int	ft_isspace(int c) { return ((c >= 9 && c <= 13) || c == 32); }

		#pragma endregion

		#pragma region "Is_Punct"

			int	ft_ispunct(const char c) {
				int			i = 0;
				const char	*punct = "!\"#$%&'()*+,-./:;<=>?@€[\\]^_`{|}~";
				while (punct[i] != '\0') { if (c == punct[i++]) return (1); } return (0);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "String"

		#pragma region "Is_AlphaNum STR"

			int	ft_isalnum_s(char *str) { while (str && *str) { if (!ft_isalnum(*str)) return (0); } return (1); }

		#pragma endregion

		#pragma region "Is_Digit STR"

			int	ft_isdigit_s(char *str) {
				if (str && (*str == '+' || *str == '-')) str++;
				if (!str || *str == '\0') return (0);
				while (*str) {
					if (!ft_isdigit(*str++)) return (0);
				}
				
				return (1);
			}

		#pragma endregion

		#pragma region "Is_Space STR"

			int	ft_isspace_s(char *str) { while (str && *str) { if (!ft_isspace(*str++)) return (0); } return (1); }

		#pragma endregion

	#pragma endregion

#pragma endregion

#pragma region "STR_TO"

	#pragma region "Char"

		#pragma region "To_Upper"

			int		ft_toupper(int c) { if (c >= 'a' && c <= 'z') { c -= 32; } return (c); }

		#pragma endregion

		#pragma region "To_Lower"

			int		ft_tolower(int c) { if (c >= 'A' && c <= 'Z') { c += 32; } return (c); }

		#pragma endregion

	#pragma endregion

	#pragma region "String"

		#pragma region "To_Upper STR"

			char	*ft_toupper_s(char *str) {
				int i = -1;

				while (str[++i]) if (str[i] >= 'a' && str[i] <= 'z') str[i] -= 32;
				return (str);
			}

		#pragma endregion

		#pragma region "To_Lower STR"

			char	*ft_tolower_s(char *str) {
				int	i = -1;

				while (str[++i]) { if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 32; }
				return (str);
			}

		#pragma endregion

	#pragma endregion

#pragma endregion

#pragma region "STR_STR"

	#pragma region "StrStr"

		char	*ft_strstr(const char *haystack, const char *needle) {
			int needle_len = ft_strlen(needle);

			if (*needle == '\0') return ((char *)haystack);
			while (*haystack) {
				if (*haystack == *needle && !strncmp(haystack, needle, needle_len)) return ((char *)haystack);
				haystack++;
			}

			return (NULL);
		}

	#pragma endregion

	#pragma region "StrNStr"

		char	*ft_strnstr(const char *haystack, const char *needle, int len) {
			int needle_len = ft_strlen(needle);;

			if (*needle == '\0') return ((char *)haystack);
			while (*haystack && len >= needle_len) {
				if (*haystack == *needle && strncmp(haystack, needle, needle_len) == 0) return ((char *)haystack);
				haystack++; len--;
			}

			return (NULL);
		}

	#pragma endregion

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
			if (len > 1) { if (!strlcpy(new_str, str + start, len)) new_str[0] = '\0'; }
			else new_str[0] = '\0';

			return (new_str);
		}

	#pragma endregion

#pragma endregion

#pragma region "STR_JOIN"

	#pragma region "StrJoin"

		char	*ft_strjoin(char *str1, char *str2, int frees) {
			if (!str1 && !str2) return (NULL);

			int len = ft_strlen(str1) + ft_strlen(str2) + 1;
			char *new_str = malloc(len);
			if (!new_str) return (NULL);
			if (str1) {
				strlcpy(new_str, str1, len);
				if (str2) strlcat(new_str, str2, len);
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
				strlcpy(new_str, str1, len + 1);
				if (sep)  strlcat(new_str, sep, len + 1);
				if (str2) strlcat(new_str, str2, len + 1);
			} else if (sep) {
				strlcpy(new_str, sep, len + 1);
				if (str2) strlcat(new_str, str2, len + 1);
			} else if (str2) strlcpy(new_str, str2, len + 1);

			if (str1 && (frees == 1 || frees == 4 || frees == 6 || frees == 7)) free(str1);
			if (sep  && (frees == 2 || frees == 4 || frees == 5 || frees == 7)) free(sep);
			if (str2 && (frees == 3 || frees == 5 || frees == 6 || frees == 7)) free(str2);

			return (new_str);
		}

	#pragma endregion

#pragma endregion

#pragma region "STR_LEN"

	#pragma region "StrLen"

		size_t ft_strlen(const char *str) {
			size_t i = 0;

			while (str && str[i]) ++i;
			return (i);
		}

	#pragma endregion

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
			if (finish - start == 1 && str[start] == c) return (free(word), strdup(""));
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

#pragma region "STR_PUT"

	#pragma region "Put Char"

		void	ft_putchar_fd(int fd, char c) {
			if (fd > 0) write (fd, &c, 1);
		}

	#pragma endregion

	#pragma region "Put String"

		void	ft_putstr_fd(int fd, char *str, int add_nl) {
			if (fd > 0 && str) {
				write(fd, str, ft_strlen(str));
				if (add_nl) write(fd, "nl", 1);
			}
		}

	#pragma endregion

	#pragma region "Put Number"

		void	ft_putnbr_fd(int fd, int n) {
			if (fd > 0) { long nb = n;
				if (nb < 0) {	write(fd, "-", 1); nb = -nb; }
				if (nb >= 10)	ft_putnbr_fd(nb / 10, fd);
				char c = (nb % 10) + '0';
				write(fd, &c, 1);
			}
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
