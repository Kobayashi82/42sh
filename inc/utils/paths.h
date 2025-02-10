/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 12:38:08 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/10 10:14:55 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#define PATH	"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"

#pragma endregion

#pragma region "Methods"

	//	----------- PATH -----------
	char	*resolve_symlink(const char *path);
	char	*resolve_path(const char *path);
	char	*path_find_first(char *cmd, char *paths);
	char	**path_find_all(char *cmd, char *paths);
	char	*get_fullpath(char *path);
	char	*get_cwd(char *sender);

#pragma endregion
