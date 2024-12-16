/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   users.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 22:03:36 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/16 23:08:42 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Info

	#pragma region Extract

		static char *extract_field(const char *line, int field) {
			const char	*start = line;
			const char	*end = line;
			int			curr_field = 0;

			while (*end) {
				if (*end == ':' || *end == '\n') {
					if (curr_field == field)
						return (ft_substr(start, 0, end - start));
					curr_field++;
					start = end + 1;
				}
				end++;
			}

			return (NULL);
		}

	#pragma endregion

	#pragma region By ID

		t_user *get_user_by_id(int uid) {
			int fd = open("/etc/passwd", O_RDONLY);
			if (fd == -1) return (NULL);

			char *id = ft_itoa(uid);
			if (!id) exit_error(NO_MEMORY, 1, NULL, true);

			char *line = NULL;
			while ((line = ft_get_next_line(fd))) {
				char *username	= extract_field(line, 0);
				char *uid_str	= extract_field(line, 2);
				char *gid_str	= extract_field(line, 3);
				char *home		= extract_field(line, 5);
				char *shell		= extract_field(line, 6);

				if (id && uid_str && home && !ft_strcmp(uid_str, id)) {
					t_user *info	= safe_calloc(1, sizeof(t_user));
					info->username	= username;
					info->home		= home;
					info->shell		= shell;
					info->uid		= uid_str ? ft_atoi(uid_str) : -1;
					info->gid		= gid_str ? ft_atoi(gid_str) : -1;

					close(fd);
					free(id);
					free(line);
					free(uid_str);
					free(gid_str);
					return (info);
				}

				free(line);
				free(username);
				free(uid_str);
				free(gid_str);
				free(home);
				free(shell);
			}

			free(id);
			close(fd);
			return (NULL);
		}

	#pragma endregion

	#pragma region By Name

		t_user *get_user_by_name(const char *name) {
			int fd = open("/etc/passwd", O_RDONLY);
			if (fd == -1) return (NULL);

			char *line = NULL;
			while ((line = ft_get_next_line(fd))) {
				char *username	= extract_field(line, 0);
				char *uid_str	= extract_field(line, 2);
				char *gid_str	= extract_field(line, 3);
				char *home		= extract_field(line, 5);
				char *shell		= extract_field(line, 6);

				if (username && home && !ft_strcmp(username, name)) {
					t_user *info	= safe_calloc(1, sizeof(t_user));
					info->username	= username;
					info->home		= home;
					info->shell		= shell;
					info->uid		= uid_str ? ft_atoi(uid_str) : -1;
					info->gid		= gid_str ? ft_atoi(gid_str) : -1;

					close(fd);
					free(line);
					free(uid_str);
					free(gid_str);
					return (info);
				}

				free(line);
				free(username);
				free(uid_str);
				free(gid_str);
				free(home);
				free(shell);
			}

			close(fd);
			return (NULL);
		}

	#pragma endregion

	#pragma region Free

		void free_user(t_user *user) {
			if (user) {
				free(user->username);
				free(user->home);
				free(user->shell);
				free(user);
			}
		}

	#pragma endregion

#pragma endregion

#pragma region Home

	#pragma region By ID

		char *get_home_by_id(int uid) {
			t_user *user = get_user_by_id(uid);
			char *tmp = safe_strdup(user->home);
			free_user(user);

			return (tmp);
		}

	#pragma endregion

	#pragma region By Name

		char *get_home_by_name(const char *name) {
			t_user *user = get_user_by_name(name);
			char *tmp = safe_strdup(user->home);
			free_user(user);

			return (tmp);
		}

	#pragma endregion

#pragma endregion
