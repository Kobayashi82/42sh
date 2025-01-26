/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 15:44:59 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/26 14:49:59 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "args.h"
	#include "globbing.h"

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
	static void	files_add(char *filename, char *dir, t_arg *files) {
		t_arg	*new_node;

		new_node = smalloc(sizeof(t_arg));
		if (dir)	new_node->value = ft_strjoin_sep(dir, filename, "/", 0);
		else		new_node->value = ft_strdup(filename);

		new_node->next = files;
		files = new_node;
	}

#pragma endregion

#pragma region "File Get"

	//	Check if is a valid file or directory and add it to the list if matched
	static int	files_get(struct dirent *de, char *pattern, char *dir, t_arg *files) {
		struct stat		stbuf;
		char			path[1024];

		ft_strncpy(path, dir, 1024);
		ft_strncat(path, "/", 1024);
		ft_strncat(path, de->d_name, 1024);
		if (stat(path, &stbuf) == -1) return (1);
		if ((S_ISREG(stbuf.st_mode) || S_ISDIR(stbuf.st_mode)) && match_pattern(de->d_name, pattern)) {
			// if (!extra)	files_add(de->d_name, dir, files);
			// else		files_add(de->d_name, NULL, files);
			files_add(de->d_name, dir, files);
		}

		return (0);
	}

#pragma endregion

#pragma region "Dir Get"

	//	Open the directory and check every file and directory for a match
	t_arg *dir_get(char *pattern, char *dir) {
		DIR				*dr;
		struct dirent	*de;
		t_arg			*files = NULL;

		dr = opendir(dir);
		if (dr && pattern) {
			de = readdir(dr);
			while (de) {
				if (((de->d_name[0] == '.' && *pattern == '.') || de->d_name[0] != '.') && files_get(de, pattern, dir, files)) break ;
				de = readdir(dr);
			}
		}

		if (dr) closedir(dr);
		return (files);
	}

#pragma endregion
