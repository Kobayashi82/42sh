/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 15:42:59 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/24 15:20:00 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "globbing.h"
#include "args.h"

#include <sys/stat.h>

int		extra;
t_arg	*files;






//	Replace a string inside a string with another string XD
char	*ft_replace(char *str, int *start, int len, char *replace)
{
	int		new_len;
	char	*new_str;
	int		i;

	if (!start)
		i = 0;
	else
		i = *start;
	new_len = ft_strlen(str) - len + ft_strlen(replace);
	new_str = smalloc(new_len + 1);
	if (!new_str)
	{
		sfree(str);
		return (NULL);
	}
	ft_memcpy(new_str, str, i);
	ft_memcpy(new_str + i, replace, ft_strlen(replace));
	ft_memcpy(new_str + i + ft_strlen(replace), \
		str + i + len, ft_strlen(str + i + len));
	new_str[new_len] = '\0';
	i += ft_strlen(replace);
	if (start)
		*start = i;
	sfree(str);
	return (new_str);
}

//	Clears all the args in an args list
void	args_list_clear(t_arg **args)
{
	t_arg	*current;
	t_arg	*next;

	current = *args;
	while (current)
	{
		next = current->next;
		if (current->value)
			sfree(current->value);
		sfree(current);
		current = next;
	}
	*args = NULL;
}

//	Merge two args lists (used for vars expansion and wildcards)
void	args_list_merge(t_arg *args, t_arg **new_args)
{
	t_arg *last;

	if (new_args && *new_args)
	{
		last = *new_args;
		while (last->next)
			last = last->next;
		if (args->next)
			last->next = args->next;
		if (args->value)
			sfree(args->value);
		args->value = (*new_args)->value;
		args->next = (*new_args)->next;
		sfree(*new_args);
		*new_args = NULL;
	}
}











//	Delete duplicates of *
static char	*optimize_pattern(char *pattern) {
	int i = 0;

	char *new_pattern = ft_strdup(pattern);
	while (new_pattern && new_pattern[i]) {
		if (new_pattern[i] == '*' && new_pattern[i + 1] == '*')
			ft_memmove(&new_pattern[i], &new_pattern[i + 1], ft_strlen(&new_pattern[i]));
		else i++;
	}

	return (new_pattern);
}

//	Sort the files lists in alphabetical order
static void	files_sort(t_arg **files) {
	t_arg	*current;
	t_arg	*tmp;
	char	*temp;

	if (!files || !*files) return ;
	current = *files;
	while (current) {
		tmp = current->next;
		while (tmp) {
			if (ft_strcmp(current->value, tmp->value) > 0) {
				temp = current->value;
				current->value = tmp->value;
				tmp->value = temp;
			} tmp = tmp->next;
		} current = current->next;
	}
}

// change ~ home only at the start of a file
// void	tilde(char **cpattern) {
// 	char *home;

// 	if (!ft_strncmp(*cpattern, "~", 1) && (home = get_home())) {
// 		int start = 0;
// 		*cpattern = ft_replace(*cpattern, &start, 1, home);
// 		sfree(home);
// 	}
// }

//	Check if there is a match for every file and directory using cpattern
void	wildcards(char **cpattern) {
	int		dir_len;
	char	dir[1024];
	char	*pattern;
	char	*last_slash;

	args_list_clear(&files);
	//tilde(cpattern);
	last_slash = ft_strrchr(*cpattern, '/');
	if (last_slash) {
		pattern = last_slash + 1;
		dir_len = last_slash - *cpattern;
		ft_strncpy(dir, *cpattern, dir_len);
		dir[dir_len] = '\0';
	} else {
		pattern = *cpattern;
		if (!getcwd(dir, sizeof(dir))) return ;
	}

	extra = !ft_memchr(*cpattern, '/', ft_strlen(*cpattern));

	pattern = optimize_pattern(pattern);

	dir_get(pattern, dir);
	extra = 0;
	sfree(pattern);
	files_sort(&files);
}

//	Search for wildcards matches in every arg and join them in a single list
void	expand_wildcards(t_arg *args) {
	if (args == NULL) return ;
	expand_wildcards(args->next);
	if (ft_memchr(args->value, '?', ft_strlen(args->value))
		|| ft_memchr(args->value, '*', ft_strlen(args->value))
		|| ft_memchr(args->value, '[', ft_strlen(args->value))
		|| ft_memchr(args->value, '~', ft_strlen(args->value)))
	{
		wildcards(&args->value);
		args_list_merge(args, &files);
	}
}
