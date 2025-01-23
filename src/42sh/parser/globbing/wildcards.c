/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 15:42:59 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/23 13:07:19 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globbing.h"

//	Check if path is a directory
bool	is_directory(char *path)
{
	struct stat	statbuf;

	return (!stat(path, &statbuf) && S_ISDIR(statbuf.st_mode));
}

//	Sort the files lists in alphabetical order
static void	files_sort(t_args **files)
{
	t_args	*current;
	t_args	*tmp;
	char	*temp;

	if (!files || !*files)
		return ;
	current = *files;
	while (current)
	{
		tmp = current->next;
		while (tmp)
		{
			if (ft_strcmp(current->arg, tmp->arg) > 0)
			{
				temp = current->arg;
				current->arg = tmp->arg;
				tmp->arg = temp;
			}
			tmp = tmp->next;
		}
		current = current->next;
	}
}

// change ~ home only at the start of a file
void	tilde(t_data *data, char **cpattern)
{
	char	*home;
	int		start;

	start = 0;
	home = env_get(data->envp, "HOME");
	if (!ft_strncmp(*cpattern, "~", 1) && home)
		*cpattern = ft_replace(*cpattern, &start, 1, home);
}

//	Check if there is a match for every file and directory using cpattern
void	wildcards(t_data *data, char **cpattern)
{
	int		dir_len;
	char	dir[1024];
	char	*pattern;
	char	*last_slash;

	args_list_clear(&data->files);
	tilde(data, cpattern);
	last_slash = ft_strrchr(*cpattern, '/');
	if (last_slash)
	{
		pattern = last_slash + 1;
		dir_len = last_slash - *cpattern;
		ft_strncpy(dir, *cpattern, dir_len);
		dir[dir_len] = '\0';
	}
	else
	{
		pattern = *cpattern;
		if (!getcwd(dir, sizeof(dir)))
			return ;
	}
	data->extra = !ft_memchr(*cpattern, '/', ft_strlen(*cpattern));
	dir_get(data, pattern, dir);
	data->extra = 0;
	files_sort(&data->files);
}

//	Search for wildcards matches in every arg and join them in a single list
void	expand_wildcards(t_data *data, t_args *args)
{
	if (args == NULL)
		return ;
	expand_wildcards(data, args->next);
	if (ft_memchr(args->arg, '?', ft_strlen(args->arg))
		|| ft_memchr(args->arg, '*', ft_strlen(args->arg))
		|| ft_memchr(args->arg, '[', ft_strlen(args->arg))
		|| ft_memchr(args->arg, '~', ft_strlen(args->arg)))
	{
		wildcards(data, &args->arg);
		args_list_merge(args, &data->files);
	}
}
