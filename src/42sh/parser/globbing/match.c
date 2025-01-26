/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 15:44:59 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/26 23:54:05 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "parser/args.h"
	#include "parser/globbing.h"
	#include "utils/utils.h"

	#include <dirent.h>
	#include <sys/stat.h>

#pragma endregion

#pragma region "Match"

	//	Check if input match a pattern. Support ? * and []
	static bool	match_pattern(char * input, char *pattern) {
		int i = 0, j = 0, start = -1, match = 0;
		int input_len = ft_strlen(input);
		int pattern_len = ft_strlen(pattern);

		while (i < input_len) {
			if (j < pattern_len && (pattern[j] == '?' || (pattern[j] == '[' && brackets(input, pattern, i, &j)) || pattern[j] == input[i])) { i++; j++; }
			else if (j < pattern_len && pattern[j] == '*') { match = i; start = j++; }
			else if (start != -1) { j = start + 1; i = match++; }
			else return (false);
		}

		while (j < pattern_len && pattern[j] == '*') j++;

		return (j == pattern_len);
	}

#pragma endregion

#pragma region "File Add"

	//	Add the matched input to a list of filenames
	static void files_add(char *filename, char *dir, t_arg **files, bool is_dir) {
		t_arg *new_node;

		new_node = ft_calloc(1, sizeof(t_arg));
		if (is_dir)	new_node->value = ft_strjoin_sep(dir, filename, "/", 0);
		else		new_node->value = ft_strjoin(dir, filename, 0);

		if (*files) (*files)->prev = new_node;
		new_node->next = *files;
		*files = new_node;
	}

#pragma endregion

#pragma region "File Get"

	//	Check if is a valid file or directory and add it to the list if matched
	static int	files_get(struct dirent *de, t_pattern *pattern, char *fulldir, char *dir, t_arg **files) {
		struct stat		stbuf;

		char *tmp = ft_strjoin_sep(fulldir, "/", de->d_name, 0);
		fulldir = resolve_path(tmp); sfree(tmp);

		if (stat(fulldir, &stbuf) == -1) return (sfree(fulldir), 1);

		if (pattern->is_dir && S_ISDIR(stbuf.st_mode)) {
			if (match_pattern(de->d_name, pattern->value))
				files_add(de->d_name, dir, files, true);
		} else if (!pattern->is_dir) {
			if (match_pattern(de->d_name, pattern->value))
				files_add(de->d_name, dir, files, S_ISDIR(stbuf.st_mode) && pattern->is_dir);
		}

		return (sfree(fulldir), 0);
	}

#pragma endregion

#pragma region "Dir Get"

	//	Open the directory and check every file and directory for a match
	t_arg *dir_get(t_pattern *pattern, char *basedir, char *dir) {
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
				if (((de->d_name[0] == '.' && *pattern->value == '.') || de->d_name[0] != '.')
					&& files_get(de, pattern, fulldir, dir, &files)) break ;
				de = readdir(dr);
			}
		}

		if (dr) closedir(dr);
		sfree(fulldir);

		return (files);
	}

#pragma endregion
