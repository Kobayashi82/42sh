/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:47:17 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/11 15:56:41 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

int initialize(int argc, const char **argv, const char **envp) {
	(void) argc; (void) argv; (void) envp;

	//	uid, euid
	//	PS1, PS2
	//	column, row
	terminal_initialize();
	options_initialize();
	variables_initialize(main_table);
	shell_initialize();
	//	Execute ~/.42shrc
	history_initialize();
	return (0);
}
