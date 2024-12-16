/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 12:38:08 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/16 23:09:49 by vzurera-         ###   ########.fr       */
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
		}	t_user;

	#pragma endregion

#pragma endregion

#pragma region Methods

	//	----------- USER -----------
	t_user *get_user_by_id(int uid);
	t_user *get_user_by_name(const char *name);
	char	*get_home_by_id(int uid);
	char	*get_home_by_name(const char *name);
	void	free_user(t_user *user);

	//	----------- TIME -----------
	char	*format_timestamp(time_t timestamp);
	time_t	get_timestamp(const char *date);

#pragma endregion
