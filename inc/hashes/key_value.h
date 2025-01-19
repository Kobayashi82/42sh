/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_value.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:10:47 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/19 19:40:33 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Methods

	int		get_key_value(const char *line, char **key, char **value, char sep);
	char	*format_for_shell(const char *value, char quote_type);

#pragma endregion
