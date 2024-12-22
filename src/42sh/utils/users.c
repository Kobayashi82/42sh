/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   users.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 22:03:36 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/22 01:53:13 by vzurera-         ###   ########.fr       */
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

	#pragma region Current

		t_userinfo *get_userinfo() { return (get_userinfo_by_id(getuid())); }

	#pragma endregion

	#pragma region By ID

		t_userinfo *get_userinfo_by_id(int uid) {
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
					t_userinfo *info	= safe_calloc(1, sizeof(t_userinfo));
					info->username	= username;
					info->home		= home;
					info->shell		= shell;
					info->uid		= uid_str ? ft_atoi(uid_str) : -1;
					info->gid		= gid_str ? ft_atoi(gid_str) : -1;

					free(id);
					free(line);
					free(uid_str);
					free(gid_str);
					close(fd);
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

		t_userinfo *get_userinfo_by_name(const char *name) {
			if (!name) return (NULL);
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
					t_userinfo *info	= safe_calloc(1, sizeof(t_userinfo));
					info->username	= username;
					info->home		= home;
					info->shell		= shell;
					info->uid		= uid_str ? ft_atoi(uid_str) : -1;
					info->gid		= gid_str ? ft_atoi(gid_str) : -1;

					free(line);
					free(uid_str);
					free(gid_str);
					close(fd);
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

		void free_user(t_userinfo *userinfo) {
			if (userinfo) {
				free(userinfo->username);
				free(userinfo->home);
				free(userinfo->shell);
				free(userinfo);
			}
		}

	#pragma endregion

#pragma endregion

#pragma region Home

	#pragma region Current

		char *get_home() { return get_home_by_id(getuid()); }

	#pragma endregion

	#pragma region By ID

		char *get_home_by_id(int uid) {
			t_userinfo *user = get_userinfo_by_id(uid);
			char *tmp = NULL;
			if (user) {
				tmp = safe_strdup(user->home);
				free_user(user);
			}

			return (tmp);
		}

	#pragma endregion

	#pragma region By Name

		char *get_home_by_name(const char *name) {
			t_userinfo *user = get_userinfo_by_name(name);
			char *tmp = NULL;
			if (user) {
				tmp = safe_strdup(user->home);
				free_user(user);
			}

			return (tmp);
		}

	#pragma endregion

#pragma endregion
