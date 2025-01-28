/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:37:42 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/28 18:36:20 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils.h"
	#include "variables.h"

	#include <sys/stat.h>

#pragma endregion

#pragma region "Resolve"

	#pragma region "Symlink"

		char *resolve_symlink(const char *path) {
			static char	resolved_path[4096];
			char 		temp_path[4096];
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

	#pragma region "Path"

		char *resolve_path(const char *path) {
			if (!path) return (NULL);
			char abs_path[4096], cwd[4096];

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

				if (ft_strlen(home) + ft_strlen(path) >= 4096) {
					write(2, "Error: Ruta demasiado larga\n", 28);
					sfree(home);
					return (NULL);
				}

				if (home) {
					ft_strcpy(abs_path, home);
					ft_strcat(abs_path, path);
					path = abs_path;
					sfree(home);
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
				if (ft_strlen(cwd) + ft_strlen(path) + 1 >= 4096) {
					write(2, "Error: Ruta demasiado larga\n", 28);
					return (NULL);
				}
				ft_strcpy(abs_path, cwd);
				ft_strcat(abs_path, "/");
				ft_strcat(abs_path, path);
			} else {
				if (ft_strlen(path) >= 4096) {
					write(2, "Error: Ruta demasiado larga\n", 28);
					return (NULL);
				}
				ft_strcpy(abs_path, path);
			}

			char *components[4096 / 2];
			int index = 0;

			char *token = ft_strtok(abs_path, "/", 1);
			while (token) {
				if (!ft_strcmp(token, "."))			{ ; } // Ignore '.'
				else if (!ft_strcmp(token, ".."))	{ if (index > 0) index--; }
				else								{ components[index++] = token; }
				token = ft_strtok(NULL, "/", 1);
			}

			char final_path[4096] = "/";
			for (int i = 0; i < index; ++i) {
				if (ft_strlen(final_path) + ft_strlen(components[i]) + 1 >= 4096) {
					write(2, "Error: Ruta demasiado larga\n", 28);
					return (NULL);
				}
				ft_strcat(final_path, components[i]);
				if (i < index - 1) ft_strcat(final_path, "/");
			}

			return (ft_strdup(final_path));
		}

	#pragma endregion

	#pragma region "FullPath"

		char *get_fullpath(char *path) {
			if (ft_strchr(path, '/')) return (resolve_path(path));

			char *path_list = ft_strdup(variables_find_value(vars_table, "PATH"));
			if (!path_list) return (ft_strdup(path));

			char *dir = ft_strtok(path_list, ":", 2);
			while (dir) {
				if (*dir || (dir = getcwd(NULL, 0))) {
					char *fullpath = ft_strjoin_sep(dir, "/", path, 0);
					char *resolved_path = resolve_path(fullpath);
					sfree(fullpath); fullpath = resolved_path;

					if (!access(fullpath, X_OK)) {
						sfree(path_list);
						return (fullpath);
					} sfree(fullpath);
				} dir = ft_strtok(NULL, ":", 2);
			} sfree(path_list);

			return (ft_strdup(path));
		}

	#pragma endregion

#pragma endregion

#pragma region "Find"

	#pragma region "First"

		char *path_find_first(char *cmd, char *paths) {
			if (!cmd) return (NULL);
			if (ft_strchr(cmd, '/')) {
				char *fullpath = resolve_path(resolve_symlink(cmd));
				if (access(fullpath, F_OK) != -1) return (fullpath);
				return (sfree(fullpath), NULL);
			}

			if (!paths && !(paths = variables_find_value(vars_table, "PATH"))) return (NULL);
			char **search_paths = ft_split(paths, ':');
			if (!search_paths) return (NULL);

			char *final_path = NULL;
			for (int i = 0; search_paths[i]; ++i) {

				char *fullpath = ft_strjoin_sep(search_paths[i], "/", cmd, 0);
				if (!fullpath) break;

				char *resolved_path = resolve_path(resolve_symlink(fullpath));
				sfree(fullpath);
				if (!resolved_path) break;

				fullpath = resolved_path;

				if (access(fullpath, F_OK) != -1) { final_path = fullpath; break; }
				sfree(fullpath);
			}

			array_free(search_paths);
			return (final_path);
		}

	#pragma endregion

	#pragma region "Path All"

		char **path_find_all(char *cmd, char *paths) {
			if (!cmd) return (NULL);
			if (ft_strchr(cmd, '/')) {
				char **final_paths = ft_calloc(2, sizeof (char *));
				char *fullpath = resolve_path(resolve_symlink(cmd));
				if (access(fullpath, F_OK) != -1) {
					final_paths[0] = ft_strdup(fullpath);
					return (final_paths);
				} return (sfree(fullpath), NULL);
			}

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
				sfree(fullpath);
				if (!resolved_path) break;

				fullpath = resolved_path;

				if (access(fullpath, F_OK) != -1) final_paths[j++] = fullpath;
				else sfree(fullpath);
			} final_paths[j] = NULL;

			array_free(search_paths);

			return (final_paths);
		}

	#pragma endregion

#pragma endregion

const char *default_editor() {
	const char	*editor = variables_find_value(vars_table, "FCEDIT");
	if (!editor || !*editor) editor = variables_find_value(vars_table, "EDITOR");
	if (!editor || !*editor) editor = variables_find_value(vars_table, "VISUAL");
	if (!editor || !*editor) editor = resolve_symlink("/usr/bin/editor");
	if (!editor || !*editor) editor = "nano";
    return (editor);
}

//	Check if path is a directory
bool is_directory(char *path) {
	struct stat	statbuf;

	return (!stat(path, &statbuf) && S_ISDIR(statbuf.st_mode));
}
