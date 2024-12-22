/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:37:42 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/22 02:01:42 by vzurera-         ###   ########.fr       */
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

char *resolve_path(const char *path) {
	if (!path) return (NULL);
    char abs_path[PATH_MAX], cwd[PATH_MAX];

    if (path[0] == '~') {
        char *home = NULL;

        if (path[1] == '/' || path[1] == '\0') {
            home = get_home();
			path++;
        } else {
            char username[256] = {0}; size_t i = 1;
            while (path[i] && path[i] != '/' && i < sizeof(username) - 1) {
                username[i - 1] = path[i]; i++;
            } username[i - 1] = '\0';

            home = get_home_by_name(username);
			if (home) path += i;
        }

        if (ft_strlen(home) + ft_strlen(path) >= PATH_MAX) {
            write(2, "Error: Ruta demasiado larga\n", 28);
            free(home);
            return (NULL);
        }

		if (home) {
	        ft_strcpy(abs_path, home);
        	ft_strcat(abs_path, path);
        	path = abs_path;
        	free(home);
		}
    }

    if (path && path[0] != '/') {
        if (!getcwd(cwd, sizeof(cwd))) {
            const char *pwd = variables_find_value(vars_table, "PWD");
            if (!pwd) {
                write(2, "Error: No se puede determinar el directorio actual\n", 52);
                return (NULL);
            }
            ft_strcpy(cwd, pwd);
        }
        if (ft_strlen(cwd) + ft_strlen(path) + 1 >= PATH_MAX) {
            write(2, "Error: Ruta demasiado larga\n", 28);
            return (NULL);
        }
        ft_strcpy(abs_path, cwd);
        ft_strcat(abs_path, "/");
        ft_strcat(abs_path, path);
    } else {
        if (ft_strlen(path) >= PATH_MAX) {
            write(2, "Error: Ruta demasiado larga\n", 28);
            return (NULL);
        }
        ft_strcpy(abs_path, path);
    }

    char *components[PATH_MAX / 2];
    int index = 0;

    char *token = ft_strtok(abs_path, "/");
    while (token) {
        if (!ft_strcmp(token, "."))			{ ; } // Ignore '.'
        else if (!ft_strcmp(token, ".."))	{ if (index > 0) index--; }
        else								{ components[index++] = token; }
        token = ft_strtok(NULL, "/");
    }

    char final_path[PATH_MAX] = "/";
    for (int i = 0; i < index; ++i) {
        if (ft_strlen(final_path) + ft_strlen(components[i]) + 1 >= PATH_MAX) {
            write(2, "Error: Ruta demasiado larga\n", 28);
            return (NULL);
        }
        ft_strcat(final_path, components[i]);
        if (i < index - 1) ft_strcat(final_path, "/");
    }

    return (ft_strdup(final_path));
}
