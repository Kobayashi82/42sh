/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brackets.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:28:19 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/23 17:38:11 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "globbing.h"

//	Adds a character to the pattern
static int	add_char(char *str, char c) {
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == c) return (0);
	str[i] = c;
	str[i + 1] = '\0';

	return (0);
}

//	Generates a string of characters to form a pattern
static char	*brackets_chars(char *pattern, int end, int i, int escaped) {
	char	*chars;
	char	c;

	chars = smalloc(512 * sizeof(char));
	if (!chars) return (NULL);
	chars[0] = '\0';
	while (++i < end) {
		if (escaped)					escaped = add_char(chars, pattern[i]);
		else if (pattern[i] == '\\')	escaped = 1;
		else if (i + 1 < end && pattern[i + 1] == '-') {
			c = pattern[i];
			while (c <= pattern[i + 2]) add_char(chars, c++);
			i += 2;
		} else if (pattern[i] != ',')	add_char(chars, pattern[i]);
	}

	return (chars);
}

//	Checks if input is present in the pattern
//	If ! or ^ then return true if input is not present in the pattern
static bool	brackets_check(char *input, char *pattern, bool inv) {
	if (ft_memchr(pattern, *input, ft_strlen(pattern))) {
		if (!inv) return (sfree(pattern), true);
	} else {
		if (inv) return (sfree(pattern), true);
	}

	return (sfree(pattern), false);
}

//	Create a pattern based on [] and check if input match it
bool	brackets(t_wc *wc) {
	char	*new_pattern;
	int		end;
	bool	inv;

	end = wc->j;
	inv = false;
	if (wc->pattern[end] == '[') {
		end++;
		while (wc->pattern[end] && wc->pattern[end] != ']') {
			if (wc->pattern[end] == '\\' && wc->pattern[end + 1] == ']') end++;
			end++;
		}

		if (wc->pattern[end] != ']') return (false);

		inv = (wc->pattern[wc->j + 1] == '!' || wc->pattern[wc->j + 1] == '^');
		new_pattern = brackets_chars(wc->pattern, end, wc->j, 0);
		wc->j += (end - wc->j);

		if (new_pattern) return (brackets_check(&wc->input[wc->i], new_pattern, inv));
	}

	return (false);
}
