/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:37:42 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/10 14:09:36 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Resolve

	#pragma region Symlink

		char *resolve_symlink(const char *path) {
			static char	resolved_path[PATH_MAX];
			char 		temp_path[PATH_MAX];
			ssize_t		len;

			ft_strcpy(temp_path, path);
			ft_strcpy(resolved_path, path);
			while ((len = readlink(temp_path, resolved_path, sizeof(resolved_path) - 1)) != -1) {
				resolved_path[len] = '\0';
				ft_strcpy(temp_path, resolved_path);
			}

			return (resolved_path);
		}

	#pragma endregion

	#pragma region Path

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
					safe_free(home);
					return (NULL);
				}

				if (home) {
					ft_strcpy(abs_path, home);
					ft_strcat(abs_path, path);
					path = abs_path;
					safe_free(home);
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

	#pragma endregion

#pragma endregion

#pragma region Find

	#pragma region First

		char *path_find_first(char *cmd, char *paths) {
			if (!cmd) return (NULL);
			if (!paths && !(paths = variables_find_value(vars_table, "PATH"))) return (NULL);
			char **search_paths = ft_split(paths, ':');
			if (!search_paths) return (NULL);

			char *final_path = NULL;
			for (int i = 0; search_paths[i]; ++i) {

				char *fullpath = ft_strjoin_sep(search_paths[i], "/", cmd, 0);
				if (!fullpath) break;

				char *resolved_path = resolve_path(resolve_symlink(fullpath));
				safe_free(fullpath);
				if (!resolved_path) break;

				fullpath = resolved_path;

				if (access(fullpath, F_OK) != -1) { final_path = fullpath; break; }
				safe_free(fullpath);
			}

			array_free(search_paths);
			return (final_path);
		}

	#pragma endregion

	#pragma region Path All

		char **path_find_all(char *cmd, char *paths) {
			if (!cmd) return (NULL);
			if (!paths && !(paths = variables_find_value(vars_table, "PATH"))) return (NULL);
			char **search_paths = ft_split(paths, ':');
			if (!search_paths) return (NULL);

			int i = 0;
			for (i = 0; search_paths && search_paths[i]; ++i) ;

			char **final_paths = ft_calloc(i + 1, sizeof (char *));
			if (!final_paths) { array_free(search_paths); return (NULL); }

			int j = 0;
			for (int i = 0; search_paths[i]; ++i) {

				char *fullpath = ft_strjoin_sep(search_paths[i], "/", cmd, 0);
				if (!fullpath) break;

				char *resolved_path = resolve_path(resolve_symlink(fullpath));
				safe_free(fullpath);
				if (!resolved_path) break;

				fullpath = resolved_path;

				if (access(fullpath, F_OK) != -1) final_paths[j++] = fullpath;
				else safe_free(fullpath);
			} final_paths[j] = NULL;

			array_free(search_paths);

			return (final_paths);
		}

	#pragma endregion

#pragma endregion
