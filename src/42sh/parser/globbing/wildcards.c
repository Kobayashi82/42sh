/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 15:42:59 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/26 14:58:13 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "parser/args.h"
	#include "parser/globbing.h"
	#include "main/options.h"

	#include <sys/stat.h>

	#include <stdio.h>

#pragma endregion

//	Check if there is a match for every file and directory using cpattern
void wildcards(char *pattern) {
	//if (!pattern) return (NULL);

	t_pattern *patterns = pattern_create(pattern);
	t_pattern *tmp = patterns;
	while (tmp) {
		printf("%s\n", tmp->value);
		tmp = tmp->next;
	}

	pattern_clear(&patterns);

	// int		dir_len;
	// char	dir[1024];
	// char	*last_slash;

	// args_list_clear(&files);

	// //tilde(cpattern);
	// last_slash = ft_strrchr(*cpattern, '/');
	// if (last_slash) {
	// 	pattern = last_slash + 1;
	// 	dir_len = last_slash - *cpattern;
	// 	ft_strncpy(dir, *cpattern, dir_len);
	// 	dir[dir_len] = '\0';
	// } else {
	// 	pattern = *cpattern;
	// 	if (!getcwd(dir, sizeof(dir))) return ;
	// }

	// extra = !ft_memchr(orig_pattern, '/', ft_strlen(orig_pattern));

	// pattern = optimize_pattern(pattern);

	// dir_get(pattern, dir);
	// extra = 0;
	// sfree(pattern);
	// files_sort(&files);
}

//	Search for wildcards matches in every arg and join them in a single list
void expand_wildcards(t_arg *args) {
	if (args == NULL) return ;
	expand_wildcards(args->next);
	if (ft_memchr(args->value, '?', ft_strlen(args->value))
		|| ft_memchr(args->value, '*', ft_strlen(args->value))
		|| ft_memchr(args->value, '[', ft_strlen(args->value))
		|| ft_memchr(args->value, '~', ft_strlen(args->value)))
	{
		wildcards(args->value);
		//args_list_merge(args, &files);
	}
}
