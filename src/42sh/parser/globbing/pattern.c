/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 15:44:59 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/24 15:57:39 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "globbing.h"
#include "args.h"

#include <dirent.h>
#include <sys/stat.h>

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

//	Add the matched input to a list of filenames
static void	files_add(char *filename, char *dir) {
	t_arg	*new_node;

	new_node = smalloc(sizeof(t_arg));
	if (dir)	new_node->value = ft_strjoin_sep(dir, filename, "/", 0);
	else		new_node->value = ft_strdup(filename);

	new_node->next = files;
	files = new_node;
}

//	Check if is a valid file or directory and add it to the list if matched
static int	files_get(struct dirent *de, char *pattern, char *dir) {
	struct stat		stbuf;
	char			path[1024];

	ft_strncpy(path, dir, 1024);
	ft_strncat(path, "/", 1024);
	ft_strncat(path, de->d_name, 1024);
	if (stat(path, &stbuf) == -1) return (1);
	if ((S_ISREG(stbuf.st_mode) || S_ISDIR(stbuf.st_mode)) && match_pattern(de->d_name, pattern)) {
		if (!extra)	files_add(de->d_name, dir);
		else		files_add(de->d_name, NULL);
	}

	return (0);
}

//	Open the directory and check every file and directory for a match
void	dir_get(char *cpattern, char *dir) {
	DIR				*dr;
	struct dirent	*de;
	char			*pattern;

	pattern = ft_strdup(cpattern);
	dr = opendir(dir);
	if (dr && pattern) {
		de = readdir(dr);
		while (de) {
			if (((de->d_name[0] == '.' && *cpattern == '.') || de->d_name[0] != '.') && files_get(de, pattern, dir)) break ;
			de = readdir(dr);
		}
	}
	if (dr) closedir(dr);
	sfree(pattern);
}
