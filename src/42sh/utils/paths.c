/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:37:42 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/17 15:38:47 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

char *resolve_symlink(const char *path) {
    static char	resolved_path[PATH_MAX];
    char 		temp_path[PATH_MAX];
    ssize_t		len;

	ft_strcpy(temp_path, path);
	while ((len = readlink(temp_path, resolved_path, sizeof(resolved_path) - 1)) != -1) {
        resolved_path[len] = '\0';
        ft_strcpy(temp_path, resolved_path);
    }

    return (resolved_path);
}
