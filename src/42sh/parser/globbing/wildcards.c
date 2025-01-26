/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 15:42:59 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/26 23:46:30 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "parser/args.h"
	#include "parser/globbing.h"
	#include "main/options.h"
	#include "utils/utils.h"

	#include <sys/stat.h>

	#include <stdio.h>

#pragma endregion

static t_arg *search(t_pattern *patterns, char *basedir, char *dir) {
	if (!patterns) return (NULL);
	t_arg *files = NULL, *final = NULL;

	if (patterns->next && patterns->is_dir) {
		files = dir_get(patterns, basedir, dir);
		t_arg *tmp = files;
		while (tmp) {
			t_arg *tmp_files = search(patterns->next, basedir, tmp->value);
			if (tmp_files) args_merge(&final, &tmp_files);
			tmp = tmp->next;
		} args_clear(&files);
	} else files = dir_get(patterns, basedir, dir);

	args_merge(&files, &final);
	return (files);
}

//	Check if there is a match for every file and directory using cpattern
static t_arg *wildcards(char *value) {
	if (!value || !*value) return (NULL);

	t_pattern *patterns = pattern_create(value);
	if (!patterns) return (NULL);

	char *basedir = NULL;
	char *dir = NULL;
	if (value[0] != '/') {
		char cwd[PATH_MAX];
		if (!getcwd(cwd, sizeof(cwd))) return (pattern_clear(&patterns), NULL);
		if (!ft_strcmp(cwd, "/"))	basedir = ft_strdup(cwd);
		else						basedir = ft_strjoin(cwd, "/", 0);
	} else							dir = ft_strdup("/");

	t_arg *files = search(patterns, basedir, dir);
	sfree(basedir); sfree(dir);
	pattern_clear(&patterns);

	return (args_sort(files));
}

//	Search for wildcards matches in every arg and join them in a single list
void expand_wildcards(t_arg *args) {
	if (args == NULL) return ;
	expand_wildcards(args->next);
	if (ft_memchr(args->value, '?', ft_strlen(args->value))
		|| ft_memchr(args->value, '*', ft_strlen(args->value))
		|| ft_memchr(args->value, '[', ft_strlen(args->value)))
	{
		t_arg *files = wildcards(args->value);
		args_insert(args, &files);
	}
}
