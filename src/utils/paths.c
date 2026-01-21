/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:37:42 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/21 21:55:08 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "internal/shell.h"
	#include "utils/utils.h"

	#include <sys/stat.h>
	#include <dirent.h>

#pragma endregion

#pragma region "Normalize"

	char *normalize_path(char *path, int free_mode) {
		errno = 0;
		if (!path) {
			if (free_mode == J_FREE_VAL_1) free(path);
			return (NULL);
		}

		char *new_path = ft_strdup(path);
		if (free_mode == J_FREE_VAL_1) free(path);
		if (!new_path) return (errno = E_NO_MEMORY, NULL);

		// Remove duplicate slashes
		int i = 0;
		while (new_path[i]) {
			if (new_path[i] == '/' && new_path[i + 1] == '/') {
				memmove(&new_path[i], &new_path[i + 1], ft_strlen(&new_path[i]));
			} else {
				i++;
			}
		}

		// Remove trailing slash
		int len = ft_strlen(new_path);
		if (len > 1 && new_path[len - 1] == '/') new_path[len - 1] = '\0';
		
		int count = 0;
		char *components[1024] = {NULL};
		char *copy = ft_strdup(new_path);
		if (!copy) return (free(new_path), errno = E_NO_MEMORY, NULL);
		char *token = ft_strtok(copy, "/", 17);

		// Handle . and ..
		while (token) {
			if (!strcmp(token, "..")) {
				if (count > 0) free(components[--count]);
			} else if (strcmp(token, ".")) {
				components[count] = ft_strdup(token);
				if (!components[count]) {
					array_free(components);
					free(copy);
					free(new_path);
					return (errno = E_NO_MEMORY, NULL);
				}
				count++;
			}
			token = ft_strtok(NULL, "/", 17);
		}
		free(copy);
		free(new_path);

		// Rebuild path
		new_path = ft_strdup("/");
		if (!new_path) {
			array_free(components);
			return (errno = E_NO_MEMORY, NULL);
		}

		for (int j = 0; j < count; ++j) {
			new_path = ft_strjoin(new_path, components[j], J_FREE_VAL_1);
			if (new_path && j < count - 1) {
				new_path = ft_strjoin(new_path, "/", J_FREE_VAL_1);
			}
			if (!new_path) {
				array_free(components);
				return (errno = E_NO_MEMORY, NULL);
			}
		}

		array_free(components);
		return (new_path);
	}

#pragma endregion

#pragma region "Resolve"

	#pragma region "Path"

		char *resolve_path(const char *path) {
			errno = 0;
			if (!path) return (NULL);

			// Make absolute if relative
			char *abs_path = (path[0] != '/') ? ft_strjoin_sep(shell.dirs.cwd, "/", path, J_FREE_NONE) : ft_strdup(path);
			if (!abs_path) return (errno = E_NO_MEMORY, NULL);

			// Split into components
			char **components = ft_split(abs_path, '/');
			free(abs_path);
			if (!components) return (errno = E_NO_MEMORY, NULL);

			// Filter . and .. 
			char *stack[256];
			int count = 0;

			for (int i = 0; components[i]; i++) {
				if (!strcmp(components[i], ".") || !*components[i]) {
					// Skip . and empty components
					continue;
				} else if (!strcmp(components[i], "..")) {
					// Go up if possible
					if (count > 0) count--;
				} else {
					// Add component
					if (count < 256) stack[count++] = components[i];
				}
			}

			// Build result
			size_t total_len = 1;
			for (int i = 0; i < count; ++i) {
				total_len += ft_strlen(stack[i]) + 1;  // component + '/'
			}

			char *result = malloc(total_len + 1);
			if (!result) {
				array_free(components);
				return (errno = E_NO_MEMORY, NULL);
			}

			char *ptr = result;
			*ptr++ = '/';
			for (int i = 0; i < count; i++) {
				size_t len = ft_strlen(stack[i]);
				memcpy(ptr, stack[i], len);
				ptr += len;
				if (i < count - 1) *ptr++ = '/';
			}
			*ptr = '\0';

			array_free(components);
			return (result);
		}

	#pragma endregion

	#pragma region "FullPath"

		char *get_fullpath(char *path) {
			if (!path || strchr(path, '/')) return (resolve_path(path));

			char *path_list = ft_strdup(variable_scalar_get(shell.env, "PATH"));
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
					char *fullpath = ft_strjoin_sep(search_dir, "/", path, J_FREE_NONE);
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
				char *fullpath = realpath(cmd, NULL);
				if (access(fullpath, F_OK) != -1) return (fullpath);
				return (free(fullpath), NULL);
			}

			if (!paths && !(paths = variable_scalar_get(shell.env, "PATH"))) return (NULL);
			char **search_paths = ft_split(paths, ':');
			if (!search_paths) return (NULL);

			char *final_path = NULL;
			for (int i = 0; search_paths[i]; ++i) {

				char *fullpath = ft_strjoin_sep(search_paths[i], "/", cmd, J_FREE_NONE);
				if (!fullpath) break;

				char *resolved_path = realpath(fullpath, NULL);
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
				char *fullpath = realpath(cmd, NULL);
				if (access(fullpath, F_OK) != -1) {
					final_paths[0] = ft_strdup(fullpath);
					return (final_paths);
				} return (free(fullpath), NULL);
			}

			if (!paths && !(paths = variable_scalar_get(shell.env, "PATH"))) return (NULL);
			char **search_paths = ft_split(paths, ':');
			if (!search_paths) return (NULL);

			int i = 0;
			for (i = 0; search_paths && search_paths[i]; ++i) ;

			char **final_paths = calloc(i + 1, sizeof (char *));
			if (!final_paths) { array_free(search_paths); return (NULL); }

			int j = 0;
			for (int i = 0; search_paths[i]; ++i) {

				char *fullpath = ft_strjoin_sep(search_paths[i], "/", cmd, J_FREE_NONE);
				if (!fullpath) break;

				char *resolved_path = realpath(fullpath, NULL);
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

#pragma region "Get CWD"

	char *get_cwd(char *sender) {
		char cwd[4096];

		if (!getcwd(cwd, sizeof(cwd))) {
			if (sender) {
				print(STDERR_FILENO, sender, P_RESET);
				print(STDERR_FILENO, ": error retrieving current directory: ", P_JOIN);
				print(STDERR_FILENO, "getcwd: cannot access parent directories: ", P_JOIN);
				print(STDERR_FILENO, strerror(errno), P_JOIN);
				print(STDERR_FILENO, "\n", P_PRINT);
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
			char resolved_path[4096] = {0};
			char *token, *temp_path = ft_strdup(path);

			// Handle absolute and relative paths
			if (path[0] == '/') strcpy(resolved_path, "/");
			else {
				if (getcwd(resolved_path, sizeof(resolved_path)) == NULL) {
					free(temp_path);
					return (ft_strdup(path));
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
			return (ft_strdup(resolved_path));
		}

	#pragma endregion

#pragma endregion

#pragma region "Is Directory"

	// Checks whether a path refers to a directory, resolving symlinks if needed
	int is_directory(const char *path) {
		struct stat path_stat;

		if (stat(path, &path_stat) == -1) return (0);
		return (S_ISDIR(path_stat.st_mode));
	}

#pragma endregion
