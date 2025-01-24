/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 12:38:08 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/24 15:00:45 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include "libft.h"

	#include <stdbool.h>
	#include <sys/time.h>

	#define PATH	"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"

#pragma endregion

#pragma region "Variables"

	#pragma region "Structures"

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	//	----------- PATH -----------
	char		*resolve_symlink(const char *path);
	char		*resolve_path(const char *path);
	char		*path_find_first(char *cmd, char *paths);
	char		**path_find_all(char *cmd, char *paths);
	char		*get_fullpath(char *path);
	int			create_temp_file(char *template);
	const char	*default_editor();
	bool		is_directory(char *path);

	//	----------- TIME -----------
	char		*format_timestamp(time_t timestamp);
	time_t		get_timestamp(const char *date);

#pragma endregion
