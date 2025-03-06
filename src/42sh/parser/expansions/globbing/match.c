/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 15:44:59 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/06 13:40:53 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "parser/tokenizer/args.h"
	#include "parser/expansions/globbing.h"
	#include "main/options.h"
	#include "utils/paths.h"

	#include <dirent.h>
	#include <sys/stat.h>

	#include <stdio.h>

#pragma endregion

#pragma region "Match Pattern"

	//	Check if input match a pattern. Support ? * and []
	static bool	match_pattern(char * input, char *pattern) {
		int i = 0, j = 0, start = -1, match = 0;
		int input_len = ft_strlen(input);
		int pattern_len = ft_strlen(pattern);
		int quoted = 0;

		while (i < input_len) {

			if (!quoted && pattern[j] == '\\')			{ quoted = 1; j++; }
			else if (!quoted && pattern[j] == '\'')  	{ quoted = 2; j++; }
			else if (quoted == 2 && pattern[j] == '\'')	{ quoted = 0; j++; }
			else if (!quoted && pattern[j] == '"')  	{ quoted = 3; j++; }
			else if (quoted == 3 && pattern[j] == '\'')	{ quoted = 0; j++; }

			char input_char = input[i];
			char pattern_char = pattern[j];

			if (options.nocaseglob) {
				input_char = ft_tolower(input_char);
				pattern_char = ft_tolower(pattern_char);
			}

			if (quoted > 0 && j < pattern_len) { if (input_char == pattern_char) { i++; j++; } else return (false); }
			else if (j < pattern_len && pattern_char != '*' && (pattern_char == '?' || (pattern_char == '[' && brackets(input, pattern, i, &j)) || pattern_char == input_char)) { i++; j++; }
			else if (j < pattern_len && pattern_char == '*') { match = i; start = j++; }
			else if (start != -1) { j = start + 1; i = match++; }
			else return (false);
			if (quoted == 1) quoted = 0;
		}

		while (j < pattern_len && !quoted && pattern[j] == '*') j++;
		if (j < pattern_len && quoted) j++;

		return (j == pattern_len);
	}

#pragma endregion

#pragma region "File Add"

	//	Add the matched input to a list of filenames
	static void files_add(char *filename, t_pattern *pattern, char *dir, t_arg **files, bool is_dir) {
		t_arg *new_node;

		if ((!ft_strcmp(filename, ".") || !ft_strcmp(filename, "..")) && ft_strcmp(filename, pattern->value)) return;

		new_node = ft_calloc(1, sizeof(t_arg));
		if (is_dir)	new_node->value = ft_strjoin_sep(dir, filename, "/", 0);
		else		new_node->value = ft_strjoin(dir, filename, 0);

		if (*files) (*files)->prev = new_node;
		new_node->next = *files;
		*files = new_node;
	}

#pragma endregion

#pragma region "Match File"

	//	Check if is a valid file or directory and add it to the list if matched
	static int	match_files(struct dirent *de, t_pattern *pattern, char *fulldir, char *dir, t_arg **files) {
		struct stat		stbuf;

		char *tmp = ft_strjoin_sep(fulldir, "/", de->d_name, 0);
		fulldir = resolve_path(tmp); sfree(tmp);

		if (stat(fulldir, &stbuf) == -1) return (sfree(fulldir), 1);

		if (pattern->is_dir && S_ISDIR(stbuf.st_mode)) {
			if (match_pattern(de->d_name, pattern->value))
				files_add(de->d_name, pattern, dir, files, true);
		} else if (!pattern->is_dir) {
			if (match_pattern(de->d_name, pattern->value))
				files_add(de->d_name, pattern, dir, files, false);
		}

		return (sfree(fulldir), 0);
	}

#pragma endregion

#pragma region "Match Dir"

	//	Open the directory and check every file and directory for a match
	t_arg *match_dir(t_pattern *pattern, char *basedir, char *dir) {
		if (!pattern || !pattern->value) return (NULL);

		DIR				*dr;
		struct dirent	*de;
		t_arg			*files = NULL;

		char *tmp = ft_strjoin_sep(basedir, "/", dir, 0);
		char *fulldir = resolve_path(tmp); sfree(tmp);
		if (!fulldir) return (NULL);

		dr = opendir(fulldir);
		if (dr && pattern && pattern->value) {
			de = readdir(dr);
			while (de) {
				bool valid = (de->d_name[0] != '.' || options.dotglob || (de->d_name[0] == '.' && *pattern->value == '.'));
				if (valid && match_files(de, pattern, fulldir, dir, &files)) break ;
				de = readdir(dr);
			}
		}

		if (dr) closedir(dr);
		sfree(fulldir);

		return (files);
	}

#pragma endregion
