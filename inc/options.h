/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:51:51 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/06 23:01:16 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdbool.h>

typedef struct {
	int		input_mode;
	bool	hist_local;		//	1 = Guardar historial en disco, 0 = No guardar
	bool	hist_on;			//	1 = ON, 0 = OFF
}	t_options;

extern t_options	options;

void	options_default();
int		options_set(char *key, char *value);
void	options_list();
