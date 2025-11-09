/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 12:38:08 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/30 12:51:45 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <sys/time.h>

#pragma endregion

#pragma region "Methods"

	//	----------- TIME -----------
	char	*format_timestamp(time_t timestamp);
	time_t	get_timestamp(const char *date);

#pragma endregion
