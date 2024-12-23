/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 12:38:08 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/23 21:29:55 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#include <time.h>

#pragma endregion

#pragma region Variables

	#pragma region Structures

		typedef struct {
			char	*username;
			char	*home;
			char	*shell;
			int		uid;
			int		gid;
		}	t_userinfo;

	#pragma endregion

#pragma endregion

#pragma region Methods

	//	----------- PATH -----------
	char		*resolve_symlink(const char *path);
	char		*resolve_path(const char *path);
	char		*path_find_first(char *cmd, char *paths);
	char		**path_find_all(char *cmd, char *paths);

	//	----------- USER -----------
	t_userinfo	*get_userinfo();
	t_userinfo	*get_userinfo_by_id(int uid);
	t_userinfo	*get_userinfo_by_name(const char *name);
	char		*get_home();
	char		*get_home_by_id(int uid);
	char		*get_home_by_name(const char *name);
	void		free_user(t_userinfo *user);

	//	----------- TIME -----------
	char		*format_timestamp(time_t timestamp);
	time_t		get_timestamp(const char *date);

#pragma endregion
