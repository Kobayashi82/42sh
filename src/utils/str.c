/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 20:09:18 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/12 12:45:24 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/utils.h"

#pragma endregion

#pragma region "IS"


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

	char *ft_toupper_s(char *str) {
		int i = -1;

		while (str[++i]) if (str[i] >= 'a' && str[i] <= 'z') str[i] -= 32;
		return (str);
	}

	char *ft_tolower_s(char *str) {
		int	i = -1;

		while (str[++i]) { if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 32; }
		return (str);
	}

#pragma endregion

#pragma region "TRIM"

	#pragma region "StrTrim"

		char *ft_strtrim(const char *s1, const char *set) {
			int i = 0, j = ft_strlen(s1) - 1;

			if (!s1 || !*s1) return (NULL);
			while (s1[i] && strchr(set, s1[i])) ++i;
			while (j > i && strchr(set, s1[j])) --j;

			return (ft_substr(s1, i, j - i + 1));
		}

	#pragma endregion

	#pragma region "SubStrTrim"

		char *ft_substr(const char *str, size_t start, int len) {
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

#pragma region "STRCAT"

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

#pragma endregion

#pragma region "STRJOIN"

	#pragma region "StrJoin"

		char *ft_strjoin(const char *value1, const char *value2, int free_mode) {
			if (!value1 && !value2) return (NULL);

			int len = ft_strlen(value1) + ft_strlen(value2) + 1;
			char *new_str = malloc(len);
			if (new_str) {
				if (value1) {
					ft_strlcpy(new_str, value1, len);
					if (value2) ft_strlcat(new_str, value2, len);
				} else if (value2) {
					strcpy(new_str, value2);
				}
			}

			if (free_mode == J_FREE_VAL1 || free_mode == J_FREE_VAL1_2) free((void *)value1);
			if (free_mode == J_FREE_VAL2 || free_mode == J_FREE_VAL1_2) free((void *)value2);

			return (new_str);
		}

	#pragma endregion

	#pragma region "StrJoinSep"

		char *ft_strjoin_sep(const char *value1, const char *value2, const char *value3, int free_mode) {
			int	len = ft_strlen(value1) + ft_strlen(value2) + ft_strlen(value3);

			if (!value1 && !value2 && !value3) return (NULL);

			char *new_str = malloc(len + 1);
			if (new_str) {
				if (value1) {
					ft_strlcpy(new_str, value1, len + 1);
					if (value2)  ft_strlcat(new_str, value2, len + 1);
					if (value3) ft_strlcat(new_str, value3, len + 1);
				} else if (value2) {
					ft_strlcpy(new_str, value2, len + 1);
					if (value3) ft_strlcat(new_str, value3, len + 1);
				} else if (value3) {
					ft_strlcpy(new_str, value3, len + 1);
				}
			}

			if (value1 && (free_mode == J_FREE_VAL1 || free_mode == J_FREE_VAL1_2 || free_mode == J_FREE_VAL1_3 || free_mode == J_FREE_VAL1_2_3)) free((void *)value1);
			if (value2 && (free_mode == J_FREE_VAL2 || free_mode == J_FREE_VAL1_2 || free_mode == J_FREE_VAL2_3 || free_mode == J_FREE_VAL1_2_3)) free((void *)value2);
			if (value3 && (free_mode == J_FREE_VAL3 || free_mode == J_FREE_VAL1_3 || free_mode == J_FREE_VAL2_3 || free_mode == J_FREE_VAL1_2_3)) free((void *)value3);

			return (new_str);
		}

	#pragma endregion

#pragma endregion

#pragma region "STRDUP"

	#pragma region "StrDup"

		char *ft_strdup(const char *s1) {
			if (!s1) return (NULL);

			size_t len = ft_strlen(s1);
			char *copy = malloc(len + 1);
			if (!copy) return (NULL);

			memcpy(copy, s1, len);
			copy[len] = '\0';
			return (copy);
		}

	#pragma endregion

	#pragma region "StrNDup"

		char *ft_strndup(const char *s1, size_t n) {
			if (!s1) return (NULL);

			size_t len = 0;
    		while (len < n && s1[len] != '\0') len++;

			char *copy = malloc(len + 1);
			if (!copy) return (NULL);

			memcpy(copy, s1, len);
			copy[len] = '\0';
			return (copy);
		}

	#pragma endregion

#pragma endregion

#pragma region "STRLEN"

	size_t ft_strlen(const char *str) {
		size_t i = 0;

		while (str && str[i]) ++i;
		return (i);
	}

#pragma endregion

#pragma region "SPLIT"

	char **ft_split(char *str, char c) {
		if (!str) return (NULL);

		int	count = 0, in_word = 0;
		for (char *tmp = str; *tmp; tmp++) {
			if (*tmp != c && !in_word && ++count)	in_word = 1;
			else if (*tmp == c)						in_word = 0;
		}

		char **result = calloc(count + 1, sizeof(char *));
		if (!result) return (NULL);

		for (int i = 0, j = 0, start; str[i];) {
			while (str[i] == c) ++i;
			if (str[i]) {
				start = i;
				while (str[i] && str[i] != c) ++i;
				result[j] = malloc(i - start + 1);
				if (!result[j]) {
					while (j--) free(result[j]);
					return (free(result), NULL);
				}
				for (int k = 0; k < i - start; ++k)
					result[j][k] = str[start + k];
				result[j++][i - start] = '\0';
			}
		}

		return (result);
	}

#pragma endregion

#pragma region "STRTOK"

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

	//	Replace a string inside a string with another string
	char *replace(const char *str, const char *old, const char *new) {
		char		*result, *pos;
		const char	*tmp;
		size_t		old_len = ft_strlen(old);
		size_t		new_len = ft_strlen(new);
		size_t		count = 0;
		size_t		result_len;

		if (!str || !old || !new || old_len == 0) return (NULL);

		tmp = str;
		while ((tmp = strstr(tmp, old)) != NULL) {
			tmp += old_len;
			count++;
		}

		if (count == 0) return ft_strdup(str);

		result_len = ft_strlen(str) + count * (new_len - old_len);
		result = malloc(result_len + 1);
		if (!result) return (NULL);

		pos = result;
		tmp = str;
		while ((tmp = strstr(tmp, old)) != NULL) {
			size_t len = tmp - str;
			memcpy(pos, str, len);
			pos += len;
			memcpy(pos, new, new_len);
			pos += new_len;
			tmp += old_len;
			str = tmp;
		}
		strcpy(pos, str);
		
		return (result);
	}

#pragma endregion

#pragma region "REPLACE SUBSTRING"

	char *replace_substring(char *str, size_t start, size_t len, const char *new) {
		size_t orig_len = ft_strlen(str);
		size_t repl_len = ft_strlen(new);

		if (start > orig_len) return (NULL);
		if (start + len > orig_len)	len = orig_len - start;

		char *new_str = malloc(orig_len - len + repl_len + 1);
		if (!new_str) return (NULL);

		strncpy(new_str, str, start);
		new_str[start] = '\0';
		strcat(new_str, new);
		if (start + len < orig_len) strcat(new_str, str + start + len);

		return (new_str);
	}

#pragma endregion

#pragma region "GET NEXT LINE"

	#define GNL_BUFFER	4096	//	Buffer size for GNL

	char *get_next_line(int fd) {
		static char	buffer[GNL_BUFFER + 1];
		char		*line = NULL, *newline, *tmp;
		int			bytes, len, blen;

		if (fd == -1) memset(buffer, 0, GNL_BUFFER + 1);
		if (fd < 0 || GNL_BUFFER <= 0) return (NULL);

		while (1) {
			if (!buffer[0] && (bytes = read(fd, buffer, GNL_BUFFER)) <= 0) {
				if (bytes == 0) return (line);
				memset(buffer, '\0', GNL_BUFFER + 1);
				return (free(line), NULL);
			}

			if ((newline = strchr(buffer, '\n'))) {
				len = (line) ? ft_strlen(line) : 0;
				blen = newline - buffer;
				tmp = malloc(len + blen + 2);
				if (!tmp) return (free(line), NULL);
				if (line) memcpy(tmp, line, len);
				memcpy(tmp + len, buffer, blen);
				tmp[len + blen] = '\n';
				tmp[len + blen + 1] = '\0';
				memmove(buffer, newline + 1, ft_strlen(newline + 1) + 1);
				return (free(line), tmp);
			}

			len = (line) ? ft_strlen(line) : 0;
			blen = ft_strlen(buffer);
			tmp = malloc(len + blen + 1);
			if (!tmp) return (free(line), NULL);
			if (line) memcpy(tmp, line, len);
			memcpy(tmp + len, buffer, blen + 1);
			memset(buffer, 0, GNL_BUFFER + 1);
			free(line);
			line = tmp;
		}
	}

#pragma endregion

#pragma region "BEEP"

	void beep() { write(STDOUT_FILENO, "\a", 1); }

#pragma endregion

#pragma region "MIN / MAX"

	int		ft_min(int n1, int n2) { if (n1 < n2) { return (n1); } return (n2); }
	int		ft_max(int n1, int n2) { if (n1 > n2) { return (n1); } return (n2); }

#pragma endregion

#pragma region "ITOA"

	char	*ft_itoa(int n) {
		char *str;
		int len = 0, sign = 1, tmp = n;

		if (n == 0) return (ft_strdup("0"));
		if (n < 0) { sign = -1; len++; }
		while (tmp != 0) { tmp /= 10; len++; }
		str = malloc((len + 1));
		if (!str) return (NULL);
		str[len] = '\0';
		while (n != 0) {
			str[--len] = '0' + (sign * (n % 10));
			n /= 10;
		}
		if (sign == -1) str[0] = '-';
		return (str);
	}

#pragma endregion

#pragma region "PRINT"

	//	Manages and writes content to a dynamic buffer associated with a file descriptor
	//
	//	Parameters:
	//		- fd: The file descriptor to write to (standars by default)
	//		- str: The string to append to the buffer (can be NULL)
	//		- mode: Controls the behavior based on the following enum values:
	//			- P_RESET:			Resets the buffer
	//			- P_RESET_PRINT:		Resets the buffer and writes its content
	//			- P_FREE_RESET:		Frees the input string and resets the buffer
	//			- P_FREE_RESET_PRINT:	Frees the input string, resets the buffer, and writes its content
	//			- P_FREE_JOIN:		Frees the input string and appends its content to the buffer
	//			-P_FREE_PRINT:		Frees the input string and writes the buffer content
	//			- P_JOIN:				Appends the input string to the buffer
	//			-P_PRINT:			Writes the buffer content
	//			- P_RESET_ALL:		Resets all buffers
	//
	//	Returns: 0 on success, 1 on failure

	#define PRINT_TOTAL_FD	3						//	Total of FDs to manage

	int print(int fd, const char *str, int mode) {
		static char		*msg[PRINT_TOTAL_FD];		//	Pointer to the buffer storing content for each file descriptor
		static size_t	len[1024];					//	Current size of the content stored for each file descriptor
		static size_t	cap[1024];					//	Total allocated capacity of the buffer for each file descriptor

		if (fd < 0 || fd >= PRINT_TOTAL_FD) return (1);

		if (mode == P_RESET_ALL) {
			//	Reset and free all buffers
			for (int i = 0; i < PRINT_TOTAL_FD; ++i) {
				free(msg[i]);
				len[i] = 0;
				cap[i] = 0;
			}
			return (0);
		}

		size_t str_len = (str) ? ft_strlen(str) : 0;

		//	Reset the buffer if necessary
		if ((mode >= P_RESET && mode <= P_FREE_RESET_PRINT) && msg[fd]) {
			free(msg[fd]);
			msg[fd] = NULL;
			len[fd] = 0;
			cap[fd] = 0;
		}

		//	If there is something to add
		if (str) {
			//	Initialize the buffer if it doesn't exist
			if (!msg[fd]) {
				cap[fd] = (str_len > 1024) ? str_len : 1024;
				msg[fd] = malloc(cap[fd]);
				if (!msg[fd]) {
					len[fd] = 0;
					cap[fd] = 0;
					errno = 290;
					return (-1);
				}
				len[fd] = 0;
			}

			//	Resize the buffer if there's not enough space
			if (len[fd] + str_len >= cap[fd]) {
				size_t new_cap = cap[fd] * 2;
				while (len[fd] + str_len >= new_cap) new_cap *= 2;
				char *new_msg = realloc(msg[fd], new_cap);
				if (!new_msg) {
					free(msg[fd]);
					len[fd] = 0;
					cap[fd] = 0;
					errno = 290;
					return (-1);
				}
				msg[fd] = new_msg;
				cap[fd] = new_cap;
			}

			//	Copy the new string into the buffer
			memcpy(msg[fd] + len[fd], str, str_len);
			len[fd] += str_len;
			msg[fd][len[fd]] = '\0';
		}

		//	Write the buffer if the mode indicates it
		int result = 0;
		if (mode % 2 && msg[fd] && fd > 0) {
			result = write(fd, msg[fd], len[fd]);
			free(msg[fd]);
			msg[fd] = NULL;
			len[fd] = 0;
			cap[fd] = 0;
		}

		//	Free the input string if necessary
		if ((mode >= P_FREE_RESET && mode <= P_FREE_PRINT) && str) free((void *)str);

		return (result == -1);
	}

#pragma endregion
