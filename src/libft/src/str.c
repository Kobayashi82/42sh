/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 20:09:18 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/27 15:07:12 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"

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
				while (*str) { if (!ft_isdigit(*str++)) return (0); } return (1);
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

#pragma region "STR_CPY"

	#pragma region "StrCpy"

		char	*ft_strcpy(char *dst, const char *src) {
			int	i = -1;

			if (!dst || !src) return (NULL);
			while (src[++i]) dst[i] = src[i];
			dst[i] = '\0';
			return (dst);
		}

	#pragma endregion

	#pragma region "StrNCpy"

		char	*ft_strncpy(char *dest, const char *src, int n) {
			int	i = -1;

			while (++i < n && src[i]) dest[i] = src[i];
			while (i < n) dest[i++] = '\0';
			return (dest);
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

#pragma endregion

#pragma region "STR_CAT"

	#pragma region "StrCat"

		char	*ft_strcat(char *dest, const char *src) {
			int dest_len = 0, i = -1;

			while (dest[dest_len]) dest_len++;
			while (src[++i]) dest[dest_len + i] = src[i];
			dest[dest_len + i] = '\0';
			return (dest);
		}

	#pragma endregion

	#pragma region "StrNCat"

		char	*ft_strncat(char *dest, const char *src, int n) {
			int dest_len = 0, i = -1;

			while (dest[dest_len]) dest_len++;
			while (++i < n && src[i]) dest[dest_len + i] = src[i];
			dest[dest_len + i] = '\0';
			return (dest);
		}

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

#pragma endregion

#pragma region "STR_CMP"

	#pragma region "StrCmp"

		int	ft_strcmp(const char *s1, const char *s2) {
			if (!s1 || !s2) return (-1);
			while (*s1 && (*s1 == *s2)) { s1++; s2++; }
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		}

	#pragma endregion

	#pragma region "StrNCmp"

		int	ft_strncmp(const char *s1, const char *s2, int n) {
			unsigned char	*str1, *str2;
			int				i = 0;

			if (!s1 || !s2) return (-1);
			str1 = (unsigned char *)s1;
			str2 = (unsigned char *)s2;
			while (*str1 && (*str1 == *str2) && i < n && n > 0) { ++str1; ++str2; ++i; }
			if ((!*str1 && !*str2) || i == n) return (0);
			return (*str1 - *str2);
		}

	#pragma endregion

#pragma endregion

#pragma region "STR_CHR"

	#pragma region "StrChr"

		char	*ft_strchr(const char *str, int c) {
			if (!str) return (NULL);
			while (*str) { if (*str == (char)c) { break; } str++; }
			if (*str == (char)c) return ((char *)str);

			return (NULL);
		}

	#pragma endregion

	#pragma region "StrRChr"

		char	*ft_strrchr(const char *str, int c) {
			int len = ft_strlen(str);
			if (!str) return (NULL);
			str += len;
			while (len-- > 0) { if (*str == (char)c) { break; } str--; }
			if (*str == (char)c) return ((char *)str);

			return (NULL);
		}

	#pragma endregion

#pragma endregion

#pragma region "STR_STR"

	#pragma region "StrStr"

		char	*ft_strstr(const char *haystack, const char *needle) {
			int needle_len = ft_strlen(needle);

			if (*needle == '\0') return ((char *)haystack);
			while (*haystack) {
				if (*haystack == *needle && !ft_strncmp(haystack, needle, needle_len)) return ((char *)haystack);
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
				if (*haystack == *needle && ft_strncmp(haystack, needle, needle_len) == 0) return ((char *)haystack);
				haystack++; len--;
			}

			return (NULL);
		}

	#pragma endregion

#pragma endregion

#pragma region "STR_DUP"

	#pragma region "StrDup"

		char	*ft_strdup(const char *s1) {
			if (!s1) return (NULL);

			char *copy = smalloc(ft_strlen(s1) + 1);
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

			char *copy = smalloc(len + 1);
			if (!copy) return (NULL);

			ft_strlcpy(copy, s1, len + 1);
			return (copy);
		}

	#pragma endregion

#pragma endregion

#pragma region "STR_TRIM"

	#pragma region "StrTrim"

		char	*ft_strtrim(const char *s1, const char *set) {
			int i = 0, j = ft_strlen(s1) - 1;

			if (!s1 || !*s1) return (NULL);
			while (s1[i] && ft_strchr(set, s1[i])) ++i;
			while (j > i && ft_strchr(set, s1[j])) --j;

			return (ft_substr(s1, i, j - i + 1));
		}

	#pragma endregion

	#pragma region "SubStrTrim"

		char	*ft_substr(const char *str, size_t start, int len) {
			int m_len = 0;

			if (start <= ft_strlen(str)) m_len = ft_strlen(str) - start;
			if (len > m_len) len = m_len;
			len += 1;

			char *new_str = smalloc(len);
			if (!new_str) return (NULL);
			if (len > 1) { if (!ft_strlcpy(new_str, str + start, len)) new_str[0] = '\0'; }
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
			char *new_str = smalloc(len);
			if (!new_str) return (NULL);
			if (str1) {
				ft_strlcpy (new_str, str1, len);
				if (str2) ft_strlcat(new_str, str2, len);
			} else if (str2) ft_strcpy (new_str, str2);

			if (frees == 1 || frees == 3) sfree(str1);
			if (frees == 2 || frees == 3) sfree(str2);

			return (new_str);
		}

	#pragma endregion

	#pragma region "StrJoinSep"

		char	*ft_strjoin_sep(char *str1, char *sep, char *str2, int frees) {
			int	len = ft_strlen(str1) + ft_strlen(sep) + ft_strlen(str2);

			if (!str1 && !sep && !str2) return (NULL);

			char *new_str = smalloc(len + 1);
			if (str1) {
				ft_strlcpy(new_str, str1, len + 1);
				if (sep)  ft_strlcat(new_str, sep, len + 1);
				if (str2) ft_strlcat(new_str, str2, len + 1);
			} else if (sep) {
				ft_strlcpy(new_str, sep, len + 1);
				if (str2) ft_strlcat(new_str, str2, len + 1);
			} else if (str2) ft_strlcpy(new_str, str2, len + 1);

			if (str1 && (frees == 1 || frees == 4 || frees == 6 || frees == 7)) sfree(str1);
			if (sep  && (frees == 2 || frees == 4 || frees == 5 || frees == 7)) sfree(sep);
			if (str2 && (frees == 3 || frees == 5 || frees == 6 || frees == 7)) sfree(str2);

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

	#pragma region "StrLen (Lines)"

		size_t ft_strlenl(const char *str) {
			size_t lines = 0;

			if (str) lines++;
			while (str && *str) { if (*str++ == '\n') lines++; }
			return (lines);
		}

	#pragma endregion

#pragma endregion

#pragma region "STR_SPLIT"

	#pragma region "Free All"

		static char	**free_all(char **result, int i) {
			if (result) {
				while (i-- > 0) sfree(result[i]);
				sfree(result);
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
			char	*word = smalloc((finish - start + 1));
			int		i = 0;

			if (!word) return (NULL);
			if (finish - start == 1 && str[start] == c) return (sfree(word), ft_strdup(""));
			while (start < finish) word[i++] = str[start++];
			word[i] = '\0';

			return (word);
		}

	#pragma endregion

	#pragma region "Split"

		char	**ft_split(char *s, char c) {
			int		i = -1, j = 0, index = -1;
			char	**split = smalloc((count_words(s, c) + 1) * sizeof(char *));
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

		while (*static_str[id] && ft_strchr(delim, *static_str[id])) static_str[id]++;
		if (!*static_str[id]) { static_str[id] = NULL; return (NULL); }

		token = static_str[id];
		while (*static_str[id] && !ft_strchr(delim, *static_str[id])) static_str[id]++;

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
		char *new_str = smalloc(new_len + 1);
		
		ft_memcpy(new_str, str, i);														//	Copy the part of the original string before the replacement
		ft_memcpy(new_str + i, replace, replace_len);									//	Copy the replacement string
		ft_memcpy(new_str + i + replace_len, str + i + len, str_len - (i + len));		//	Copy the part of the original string after the replacement
		new_str[new_len] = '\0';														//	Add the null terminator at the end of the new string

		if (start) *start = i + replace_len;											//	Update the start index if needed

		return (sfree(str), new_str);
	}

#pragma endregion

#pragma region "BEEP"

	void beep() { write(STDOUT_FILENO, "\a", 1); }

#pragma endregion
