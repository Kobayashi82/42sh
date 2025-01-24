/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brackets.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:28:19 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/24 13:19:39 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	] puede ir despues de ! o ^ o escapado

#include "libft.h"
#include "globbing.h"

#define MAX_NEW_PATTERN_SIZE 512

//	Adds a character to the new pattern
static void add_char(char *new_pattern, char c) {
	if (ft_strlen(new_pattern) >= MAX_NEW_PATTERN_SIZE) return;

	int	i;
	for (i = 0; new_pattern[i]; ++i)
		if (new_pattern[i] == c) return;

	new_pattern[i++] = c;
	new_pattern[i] = '\0';
}

//	Generates a string of characters to form a new pattern
static char *brackets_chars(const char *pattern, int end, int i) {
	char	c, *new_pattern = ft_calloc(MAX_NEW_PATTERN_SIZE + 1, sizeof(char));
	bool	escaped = false;

	for (++i; i < end; ++i) {
		if (ft_strlen(new_pattern) >= MAX_NEW_PATTERN_SIZE) break;
		if (escaped) { add_char(new_pattern, pattern[i]);	escaped = false; }
		else if (pattern[i] == '\\') escaped = true;
		else if (i + 2 < end && pattern[i + 1] == '-') {
			if (pattern[i] > pattern[i + 2]) return (sfree(new_pattern), NULL);
			c = pattern[i];
			while (c <= pattern[i + 2]) add_char(new_pattern, c++);
			i += 2;
		} else add_char(new_pattern, pattern[i]);
	}

	if (!*new_pattern) { sfree(new_pattern), new_pattern = NULL; }
	return (new_pattern);
}

//	Checks if input is present in the pattern
//	If ! or ^ then return true if input is not present in the pattern
static bool brackets_check(const char input, char *pattern, bool inv) {
	bool match = ft_memchr(pattern, input, ft_strlen(pattern));

	if ((match && !inv) || (!match && inv))
		return (sfree(pattern), true);

	return (sfree(pattern), false);
}

//	Create a pattern based on [] and check if input matches it
bool brackets(char *input, char *pattern, int i, int *j) {
	int		end = *j;

	if (pattern[end] == '[') { end++;
		// Check for inversion (! or ^)
		bool inv = (pattern[end] == '!' || pattern[end] == '^');
		if (inv && pattern[++end] && pattern[end] != '\\') end++;


		// Find the closing bracket
		while (pattern[end] && pattern[end] != ']') {
			if (pattern[end] == '\\' && pattern[end + 1] == ']') end++;
			end++;
		}
		// If no closing bracket, return false
		if (pattern[end] != ']' || end == *j + 1 + inv) return (false);

		// Generate the pattern characters
		char *new_pattern = brackets_chars(pattern, end, *j + (inv ? 1 : 0));
		if (!new_pattern) return (false);
		*j = end;

		// Check if input matches the generated pattern
		return (brackets_check(input[i], new_pattern, inv));
	}

	return (false);
}
