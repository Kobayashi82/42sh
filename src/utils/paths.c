/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:37:42 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/07 23:58:41 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/variable.h"
	#include "utils/utils.h"

	#include <sys/stat.h>
	#include <dirent.h>

#pragma endregion

#pragma region "Resolve"

	#pragma region "Symlink"

		char *resolve_symlink(const char *path) {
			static char resolved_path[4096];
			char buffer[4096];
			ssize_t len;

			if (!path) return (NULL);

			strncpy(resolved_path, path, sizeof(resolved_path) - 1);
			resolved_path[sizeof(resolved_path) - 1] = '\0';
			
			if ((len = readlink(path, buffer, sizeof(buffer) - 1)) != -1) {
				buffer[len] = '\0';
				strncpy(resolved_path, buffer, sizeof(resolved_path) - 1);
				resolved_path[sizeof(resolved_path) - 1] = '\0';
				return (resolved_path);
			}

			if (!strchr(path, '/')) return (resolved_path);

			char temp[4096];
			strncpy(temp, path, sizeof(temp) - 1);
			temp[sizeof(temp) - 1] = '\0';

			char *last_slash = NULL;
			char *p = temp;

			while ((p = strchr(p, '/'))) {
				*p = '\0';

				if (*temp) {
					if ((len = readlink(temp, buffer, sizeof(buffer) - 1)) != -1) {
						buffer[len] = '\0';

						if (buffer[0] == '/')
							strncpy(resolved_path, buffer, sizeof(resolved_path) - 1);
						else {
							char base_dir[4096] = "";
							if (last_slash) {
								strncpy(base_dir, temp, last_slash - temp + 1);
								base_dir[last_slash - temp + 1] = '\0';
							}
							strncpy(resolved_path, base_dir, sizeof(resolved_path) - 1);

							size_t base_len = ft_strlen(resolved_path);
							size_t buffer_len = ft_strlen(buffer);
							if (base_len + buffer_len < sizeof(resolved_path) - 1) {
								strncpy(resolved_path + base_len, buffer, sizeof(resolved_path) - base_len - 1);
								resolved_path[base_len + buffer_len] = '\0';
							}
						}

						if (*(p + 1)) {
							size_t current_len = ft_strlen(resolved_path);
							if (current_len < sizeof(resolved_path) - 2) {
								resolved_path[current_len] = '/';
								strncpy(resolved_path + current_len + 1, p + 1, sizeof(resolved_path) - current_len - 2);
								resolved_path[sizeof(resolved_path) - 1] = '\0';
							}
						}

						return (resolved_path);
					}
				}

				last_slash = p;
				*p = '/';
				p++;
			}

			if (*temp && (len = readlink(temp, buffer, sizeof(buffer) - 1)) != -1) {
				buffer[len] = '\0';
				strncpy(resolved_path, buffer, sizeof(resolved_path) - 1);
				resolved_path[sizeof(resolved_path) - 1] = '\0';
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
					free(home);
					return (NULL);
				}

				if (home) {
					strcpy(abs_path, home);
					strcat(abs_path, path);
					path = abs_path;
					free(home);
				}
			}

			if (path && path[0] != '/') {
				if (!getcwd(cwd, sizeof(cwd))) {
					const char *pwd = variables_find_value(vars_table, "PWD");	// No deberia usar shell.cwd ??
					if (!pwd) {
						write(2, "Error: No se puede determinar el directorio actual\n", 52);
						return (NULL);
					}
					strcpy(cwd, pwd);
				}
				if (ft_strlen(cwd) + ft_strlen(path) + 1 >= 4096) {
					write(2, "Error: Ruta demasiado larga\n", 28);
					return (NULL);
				}
				strcpy(abs_path, cwd);
				strcat(abs_path, "/");
				strcat(abs_path, path);
			} else {
				if (ft_strlen(path) >= 4096) {
					write(2, "Error: Ruta demasiado larga\n", 28);
					return (NULL);
				}
				strcpy(abs_path, path);
			}

			char *components[4096 / 2];
			int index = 0;

			char *token = ft_strtok(abs_path, "/", 1);
			while (token) {
				if (!strcmp(token, "."))			{ ; } // Ignore '.'
				else if (!strcmp(token, ".."))	{ if (index > 0) index--; }
				else								{ components[index++] = token; }
				token = ft_strtok(NULL, "/", 1);
			}

			char final_path[4096] = "/";
			for (int i = 0; i < index; ++i) {
				if (ft_strlen(final_path) + ft_strlen(components[i]) + 1 >= 4096) {
					write(2, "Error: Ruta demasiado larga\n", 28);
					return (NULL);
				}
				strcat(final_path, components[i]);
				if (i < index - 1) strcat(final_path, "/");
			}

			return (ft_strdup(final_path));
		}

	#pragma endregion

	#pragma region "FullPath"

		char *get_fullpath(char *path) {
			if (!path || strchr(path, '/')) return (resolve_path(path));

			char *path_list = ft_strdup(variables_find_value(vars_table, "PATH"));
			if (!path_list) return (ft_strdup(path));

			char *dir = ft_strtok(path_list, ":", 2);
			while (dir) {
				char *current_dir = NULL;
				char *search_dir = dir;

				if (!*dir) {
					current_dir = getcwd(NULL, 0);
					search_dir = current_dir;
				}

				if (search_dir) {
					char *fullpath = ft_strjoin_sep(search_dir, "/", path, 0);
					char *resolved_path = resolve_path(fullpath);
					free(fullpath);
					fullpath = resolved_path;

					if (!access(fullpath, F_OK)) {
						free(current_dir);
						free(path_list);
						return (fullpath);
					}
					free(fullpath);
				}
				free(current_dir);
				dir = ft_strtok(NULL, ":", 2);
			}
			free(path_list);

			return (ft_strdup(path));
		}

	#pragma endregion

	#pragma region "Get Fullpath Command"

		// Resolves a command string to its executable path; returns NULL if not executable
		char *get_fullpath_command(const char *value, int fullpath) {
			if (!value) return (NULL);

			char	current[4096];
			char	quoteChar = 0;
			int		escaped = 0;
			int		j = 0;

			for (int i = 0; j < 4095 && value[i]; ++i) {
				char c = value[i];

				if (escaped)											{ escaped = 0;		current[j++] = c;	continue; }
				if (quoteChar != '\'' && c == '\\')						{ escaped = 1;							continue; }
				if (!quoteChar && (c == '"' || c == '\''))				{ quoteChar = c;						continue; }
				if (quoteChar && c == quoteChar)						{ quoteChar = 0;						continue; }
				if (!quoteChar && isspace(c))							{ 										break;    }

				current[j++] = c;
			}

			current[j] = '\0';

			return ((fullpath) ? get_fullpath(current) : ft_strdup(current));
		}

	#pragma endregion

#pragma endregion

#pragma region "Find"

	#pragma region "First"

		char *path_find_first(char *cmd, char *paths) {
			if (!cmd) return (NULL);
			if (strchr(cmd, '/')) {
				char *fullpath = resolve_path(resolve_symlink(cmd));
				if (access(fullpath, F_OK) != -1) return (fullpath);
				return (free(fullpath), NULL);
			}

			if (!paths && !(paths = variables_find_value(vars_table, "PATH"))) return (NULL);
			char **search_paths = ft_split(paths, ':');
			if (!search_paths) return (NULL);

			char *final_path = NULL;
			for (int i = 0; search_paths[i]; ++i) {

				char *fullpath = ft_strjoin_sep(search_paths[i], "/", cmd, 0);
				if (!fullpath) break;

				char *resolved_path = resolve_path(resolve_symlink(fullpath));
				free(fullpath);
				if (!resolved_path) break;

				fullpath = resolved_path;

				if (access(fullpath, F_OK) != -1) { final_path = fullpath; break; }
				free(fullpath);
			}

			array_free(search_paths);
			return (final_path);
		}

	#pragma endregion

	#pragma region "Path All"

		char **path_find_all(char *cmd, char *paths) {
			if (!cmd) return (NULL);

			if (strchr(cmd, '/')) {
				char **final_paths = calloc(2, sizeof (char *));
				char *fullpath = resolve_path(resolve_symlink(cmd));
				if (access(fullpath, F_OK) != -1) {
					final_paths[0] = ft_strdup(fullpath);
					return (final_paths);
				} return (free(fullpath), NULL);
			}

			if (!paths && !(paths = variables_find_value(vars_table, "PATH"))) return (NULL);
			char **search_paths = ft_split(paths, ':');
			if (!search_paths) return (NULL);

			int i = 0;
			for (i = 0; search_paths && search_paths[i]; ++i) ;

			char **final_paths = calloc(i + 1, sizeof (char *));
			if (!final_paths) { array_free(search_paths); return (NULL); }

			int j = 0;
			for (int i = 0; search_paths[i]; ++i) {

				char *fullpath = ft_strjoin_sep(search_paths[i], "/", cmd, 0);
				if (!fullpath) break;

				char *resolved_path = resolve_path(resolve_symlink(fullpath));
				free(fullpath);
				if (!resolved_path) break;

				fullpath = resolved_path;

				if (access(fullpath, F_OK) != -1) final_paths[j++] = fullpath;
				else free(fullpath);
			} final_paths[j] = NULL;

			array_free(search_paths);

			return (final_paths);
		}

	#pragma endregion

#pragma endregion

#pragma region "Working Dir"

	char *get_cwd(char *sender) {
		char cwd[4096];

		if (!getcwd(cwd, sizeof(cwd))) {
			if (sender) {
				print(STDERR_FILENO, sender, RESET);
				print(STDERR_FILENO, ": error retrieving current directory: ", JOIN);
				print(STDERR_FILENO, "getcwd: cannot access parent directories: ", JOIN);
				print(STDERR_FILENO, strerror(errno), JOIN);
				print(STDERR_FILENO, "\n", PRINT);
			}
			return (NULL);
		}

		return (ft_strdup(cwd));
	}

#pragma endregion

#pragma region "Correct Path"

	#pragma region "Levenshtein"

		// Computes the Damerau-Levenshtein distance between two strings
		static int levenshtein(const char *s1, const char *s2) {
			int len1 = ft_strlen(s1), len2 = ft_strlen(s2);
			int matrix[len1 + 1][len2 + 1];

			// Initialize the matrix: first row and first column represent empty string transformations
			for (int i = 0; i <= len1; i++) matrix[i][0] = i;
			for (int j = 0; j <= len2; j++) matrix[0][j] = j;

			// Compute the minimum edit distance
			for (int i = 1; i <= len1; i++) {
				for (int j = 1; j <= len2; j++) {
					int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

					// Allow transposition of adjacent characters (Damerau-Levenshtein extension)
					if (i > 1 && j > 1 && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1]) {
						matrix[i][j] =	ft_min(matrix[i - 1][j] + 1,							// Deletion
										ft_min(matrix[i][j - 1] + 1,							// Insertion
										ft_min(matrix[i - 1][j - 1] + cost,						// Substitution
										matrix[i - 2][j - 2] + 1)));							// Transposition (swap)
					} else {
						matrix[i][j] =	ft_min(matrix[i - 1][j] + 1,							// Deletion
										ft_min(matrix[i][j - 1] + 1,							// Insertion
										matrix[i - 1][j - 1] + cost));							// Substitution
					}
				}
			}
			return (matrix[len1][len2]);
		}

	#pragma endregion

	#pragma region "Closest Dir"

		// Finds the closest matching directory name based on the Levenshtein distance
		static char *find_closest_dir(const char *input, const char *path) {
			DIR *dir = opendir(path);
			if (!dir) return (NULL);

			struct dirent *entry;
			char *best_match = NULL;
			int min_distance = INT_MAX;

			// Iterate through directory entries to find the closest match
			while ((entry = readdir(dir))) {
				if (entry->d_type == DT_DIR || entry->d_type == DT_LNK) {
					int dist = levenshtein(input, entry->d_name);
					if (dist < min_distance) {
						min_distance = dist;
						free(best_match);
						best_match = ft_strdup(entry->d_name);
					}
				}
			} closedir(dir);

			// Return the best match if the distance is small enough
			if (min_distance < 3)		return (best_match);
			else { free(best_match);	return (NULL); }
		}

	#pragma endregion

	#pragma region "Correct Path"

		// Corrects a potentially misspelled directory path using fuzzy matching
		char *correct_path(char *path) {
			char resolved_path[1024] = {0};
			char *token, *temp_path = ft_strdup(path);

			// Handle absolute and relative paths
			if (path[0] == '/') strcpy(resolved_path, "/");
			else {
				if (getcwd(resolved_path, sizeof(resolved_path)) == NULL) {
					free(temp_path); return (path);
				}
				size_t len = ft_strlen(resolved_path);
				if (len == 0 || resolved_path[len - 1] != '/') {
					strcat(resolved_path, "/");
				}
			}

			// Tokenize the path and attempt to correct each segment
			token = ft_strtok(temp_path, "/", 60);
			while (token) {
				char *corrected = find_closest_dir(token, resolved_path);
				if (corrected) {
					strcat(resolved_path, corrected);
					free(corrected);
				} else strcat(resolved_path, token);

				strcat(resolved_path, "/");
				token = ft_strtok(NULL, "/", 60);
			} free(temp_path);

			// Remove trailing slash if necessary
			size_t len = ft_strlen(resolved_path);
			if (len > 1 && resolved_path[len - 1] == '/') resolved_path[len - 1] = '\0';

			// Resolve final path and copy it back
			free(path);
			return (resolve_path(resolved_path));
		}

	#pragma endregion

#pragma endregion

#pragma region "Is Directory"

	// Checks whether a path refers to a directory, resolving symlinks if needed
	int is_directory(const char *path) {
		struct stat path_stat;
		char *resolved_path;

		if (lstat(path, &path_stat) == -1)					return (0);
		if (S_ISLNK(path_stat.st_mode)) {
			resolved_path = resolve_symlink(path);
			if (*resolved_path) {
				if (lstat(resolved_path, &path_stat) == -1)	return (0);
				if (S_ISDIR(path_stat.st_mode))				return (1);
			}
		}

		return ((S_ISDIR(path_stat.st_mode)) ? 1 : 0);
	}

#pragma endregion
