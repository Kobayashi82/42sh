/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brackets.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:28:19 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/27 19:44:44 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "parser/globbing.h"
	#include "main/options.h"

	#define MAX_NEW_PATTERN_SIZE 512

#pragma endregion

#pragma region "Pattern"

	#pragma region "Add"

		//	Adds a character to the new pattern
		static void bracket_pattern_add(char *new_pattern, char c) {
			if (ft_strlen(new_pattern) >= MAX_NEW_PATTERN_SIZE) return;

			int	i;
			for (i = 0; new_pattern[i]; ++i)
				if (new_pattern[i] == c) return;

			new_pattern[i++] = c;
			new_pattern[i] = '\0';
		}

	#pragma endregion

	#pragma region "Create"

		//	Generates a string of characters to form a new pattern
		static char *bracket_pattern_create(const char *pattern, int end, int i) {
			char c, *new_pattern = ft_calloc(MAX_NEW_PATTERN_SIZE + 1, sizeof(char));
			bool escaped = false;

			for (++i; i < end; ++i) {
				if (ft_strlen(new_pattern) >= MAX_NEW_PATTERN_SIZE) break;
				if (escaped) { bracket_pattern_add(new_pattern, pattern[i]);	escaped = false; }
				else if (pattern[i] == '\\') escaped = true;
				else if (i + 2 < end && pattern[i + 1] == '-') {
					if (pattern[i] > pattern[i + 2]) return (sfree(new_pattern), NULL);
					c = pattern[i];
					while (c <= pattern[i + 2]) bracket_pattern_add(new_pattern, c++);
					i += 2;
				} else bracket_pattern_add(new_pattern, pattern[i]);
			}

			if (!*new_pattern) { sfree(new_pattern), new_pattern = NULL; }
			return (new_pattern);
		}

	#pragma endregion

	#pragma region "Match"

		//	Checks if input is present in the pattern
		//	If ! or ^ then return true if input is not present in the pattern
		static bool bracket_pattern_match(const char input, char *pattern, bool inv) {
			char input_char = input;
			char *pattern_chars = pattern;
			if (options.nocaseglob) {
				input_char = ft_tolower(input_char);
				pattern_chars = ft_tolower_s(pattern);
			}

			bool match = ft_memchr(pattern_chars, input_char, ft_strlen(pattern_chars));

			if ((match && !inv) || (!match && inv))
				return (sfree(pattern), true);

			return (sfree(pattern), false);
		}

	#pragma endregion

#pragma endregion

#pragma region "Brackets"

	//	Create a pattern based on [] and check if input matches it
	bool brackets(char *input, char *pattern, int i, int *j) {
		int end = *j;

		if (pattern[end] == '[') { end++;
			if(pattern[end] == ']') end++;

			// Check for inversion (! or ^)
			bool inv = (pattern[end] == '!' || pattern[end] == '^');
			if (inv && pattern[++end] && pattern[end] != '\\') end++;

			// Find the closing bracket
			while (pattern[end] && pattern[end] != ']') {
				if (pattern[end] == '\\' && pattern[end + 1]) end++;
				end++;
			}
			// If no closing bracket, return false
			if (pattern[end] != ']' || end == *j + 1 + inv) return (false);

			// Generate the pattern characters
			char *new_pattern = bracket_pattern_create(pattern, end, *j + (inv ? 1 : 0));
			if (!new_pattern) return (false);
			*j = end;

			// Check if input matches the generated pattern
			return (bracket_pattern_match(input[i], new_pattern, inv));
		}

		return (false);
	}

#pragma endregion
