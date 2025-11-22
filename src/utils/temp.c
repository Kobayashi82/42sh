/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 20:14:51 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/22 15:01:23 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"

	#include <string.h>
	#include <sys/time.h>
	#include <sys/stat.h>

#pragma endregion

#pragma region "HASH"

	#pragma region "Variables"

		#define TMP_HASH_SIZE	1031
		#define TEMPLATE_SIZE	6		//	Número de 'X' para generar un nombre aleatorio
		#define TEMPLATE_TRIES	100		//	Numero de intentos de crear el nombre aleatorio
		#define TEMPLATE_CHARS	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

		typedef struct s_tmp {
			char			*path;
			int				fd;
			struct s_tmp	*next;
		}	t_tmp;

		t_tmp	*tmp_table[TMP_HASH_SIZE];

	#pragma endregion

	#pragma region "Index"

		static unsigned int hash_index(const char *key) {
			unsigned int hash = 0;

			while (*key) hash = (hash * 31) + *key++;
			return (hash % TMP_HASH_SIZE);
		}

	#pragma endregion

	#pragma region "Find"

		#pragma region "Path"

			static t_tmp *tmp_find_path_node(char *path) {
				if (!path) return (NULL);

				t_tmp *tmp = tmp_table[hash_index(path)];

				while (tmp) {
					if (tmp->path == path) return (tmp);
					tmp = tmp->next;
				}

				return (NULL);
			}

			char *tmp_find_path(char *path) {
				if (!path) return (NULL);

				t_tmp *tmp = tmp_table[hash_index(path)];

				while (tmp) {
					if (tmp->path == path) return (tmp->path);
					tmp = tmp->next;
				}

				return (NULL);
			}

			char *tmp_find_path_fd(int fd) {
				if (fd < 0) return (NULL);

				for (unsigned int index = 0; index < TMP_HASH_SIZE; index++) {
					if (tmp_table[index]) {
						t_tmp *tmp = tmp_table[index];
						while (tmp) {
							if (tmp->fd == fd) return (tmp->path);
							tmp = tmp->next;
						}
					}
				}

				return (NULL);
			}

		#pragma endregion

		#pragma region "FD"

			// static t_tmp *tmp_find_fd_node(int fd) {
			// 	if (fd < 0) return (NULL);

			// 	for (unsigned int index = 0; index < TMP_HASH_SIZE; index++) {
			// 		if (tmp_table[index]) {
			// 			t_tmp *tmp = tmp_table[index];
			// 			while (tmp) {
			// 				if (tmp->fd == fd) return (tmp);
			// 				tmp = tmp->next;
			// 			}
			// 		}
			// 	}

			// 	return (NULL);
			// }

			int tmp_find_fd_path(char *path) {
				if (!path) return (-1);

				t_tmp *tmp = tmp_table[hash_index(path)];

				while (tmp) {
					if (tmp->path == path) return (tmp->fd);
					tmp = tmp->next;
				}

				return (-1);
			}

			int tmp_find_fd(int fd) {
				if (fd < 0) return (-1);

				for (unsigned int index = 0; index < TMP_HASH_SIZE; index++) {
					if (tmp_table[index]) {
						t_tmp *tmp = tmp_table[index];
						while (tmp) {
							if (tmp->fd == fd) return (tmp->fd);
							tmp = tmp->next;
						}
					}
				}

				return (-1);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Close"

		void tmp_close_path(char *path) {
			if (!path) return;

			for (unsigned int index = 0; index < TMP_HASH_SIZE; index++) {
				if (tmp_table[index]) {
					t_tmp *tmp = tmp_table[index];
					while (tmp) {
						if (!strcmp(path, tmp->path)) {
							close(tmp->fd);
							tmp->fd = -1;
							return;
						}
						tmp = tmp->next;
					}
				}
			}
		}

		void tmp_close_fd(int fd) {
			if (fd < 0) return;

			for (unsigned int index = 0; index < TMP_HASH_SIZE; index++) {
				if (tmp_table[index]) {
					t_tmp *tmp = tmp_table[index];
					while (tmp) {
						if (tmp->fd == fd) {
							close(tmp->fd);
							tmp->fd = -1;
							return;
						}
						tmp = tmp->next;
					}
				}
			}
		}

	#pragma endregion

	#pragma region "Add"

		void tmp_add(char *path, int fd) {
			if (!path) return;

			t_tmp *new_tmp = tmp_find_path_node(path);
			if (new_tmp) {
				free(new_tmp->path);
				close(new_tmp->fd);
				new_tmp->path = path;
				new_tmp->fd = fd;
			} else {
				unsigned int index = hash_index(path) % TMP_HASH_SIZE;
				t_tmp *new_node = malloc(sizeof(t_tmp));

				new_node->path = path;
				new_node->fd = fd;
				new_node->next = tmp_table[index];
				tmp_table[index] = new_node;
			}
		}

	#pragma endregion

	#pragma region "Delete"

		void tmp_delete_path(char *path) {
			if (!path) return;

			unsigned int index = hash_index(path);
			t_tmp *tmp = tmp_table[index];
			t_tmp *prev = NULL;

			while (tmp) {
				if (tmp->path == path) {
					if (prev)	prev->next = tmp->next;
					else		tmp_table[index] = tmp->next;
					close(tmp->fd);
					unlink (tmp->path);
					free(tmp->path);
					free(tmp);
					return;
				}
				prev = tmp;
				tmp = tmp->next;
			}
			free(path);
		}

		void tmp_delete_fd(int fd) {
			if (fd < 0) return;

			for (unsigned int index = 0; index < TMP_HASH_SIZE; index++) {
				if (tmp_table[index]) {
					t_tmp *tmp = tmp_table[index];
					t_tmp *prev = NULL;
					while (tmp) {
						if (tmp->fd == fd) {
							if (prev)	prev->next = tmp->next;
							else		tmp_table[index] = tmp->next;
							close(tmp->fd);
							unlink (tmp->path);
							free(tmp->path);
							free(tmp);
							return;
						}
						prev = tmp;
						tmp = tmp->next;
					}
				}
			}
		}

	#pragma endregion

	#pragma region "Clear"

		void tmp_clear() {
			for (unsigned int index = 0; index < TMP_HASH_SIZE; index++) {
				if (tmp_table[index]) {
					t_tmp *tmp = tmp_table[index];
					while (tmp) {
						t_tmp *next = tmp->next;
						close(tmp->fd);
						unlink (tmp->path);
						free(tmp->path);
						free(tmp);
						tmp = next;
					}
					tmp_table[index] = NULL;
				}
			}
		}

	#pragma endregion

#pragma endregion

#pragma region "Temp"

	#pragma region "Template"

		static char *random_template() {
			struct timeval tv; gettimeofday(&tv, NULL);
			unsigned int seed = (unsigned int)(tv.tv_sec ^ tv.tv_usec);

			char *template = malloc(TEMPLATE_SIZE + 1);
			unsigned int r = seed;
			for (int j = 0; j < TEMPLATE_SIZE; ++j, r /= 36)
				template[j] = TEMPLATE_CHARS[r % ft_strlen(TEMPLATE_CHARS)];
			template[TEMPLATE_SIZE] = '\0';

			return (template);
		}

	#pragma endregion

	#pragma region "Path"

		int is_writable_dir(const char *dir) {
			struct stat st;

			if (stat(dir, &st) == -1)		return (0);		// Si no se puede acceder al directorio, devolver 0
			if (!(st.st_mode & S_IWUSR))	return (0);		// Si no tiene permisos de escritura, devolver 0

			return (1);
		}

		char *get_temp_dir() {
			if (is_writable_dir("/tmp"))		return (strdup("/tmp"));
			if (is_writable_dir("/var/tmp"))	return (strdup("/var/tmp"));

			char *home = get_home();
			if (home && is_writable_dir(home))	return (home);

			return (NULL);
		}

	#pragma endregion

	#pragma region "MKDTEMP"

		char *ft_mkdtemp(char *path, char *name) {
			if (!path || !is_writable_dir(path)) {
				if (!(path = get_temp_dir())) return (NULL);
			} else path = ft_strdup(path);
			path = ft_strjoin(path, "/", 1);
			char *template = random_template();
			char *fullpath = ft_strjoin_sep(path, name, template, 6);


			int fd = open(fullpath, O_RDWR | O_CREAT | O_EXCL, 0600);
			if (fd == -1) { free(fullpath); return (NULL); }
			tmp_add(fullpath, fd);

			return (fullpath);
		}

	#pragma endregion

#pragma endregion
