/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:04:42 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/23 20:50:50 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

//	-v				Muestra la primera ocurrencia solo
//		alias		alias [alias]='[value]'
//		builtin		[builtin]
//		cmd			[cmd_path]
//		no_valid	NOTHING

//	-V				Muestra el tipo:							(solo muestra la primera ocurrencia)
//		alias		[alias] is aliased to `[value]'
//		builtin		[builtin] is a shell builtin
//		cmd			[cmd] is [cmd path]							(muestra todas las rutas donde se encuentre, no solo la primera)
//		no_valid	42sh: type: [cmd]: not found

//	-p				Usa un path interno para buscar el comando (/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin)

//	no_opts			Ejecuta el comando con los argumentos

//	Exit Status:
//		no_opts		Returns exit status of COMMAND
//		-vV			failure if COMMAND is not found, but if a command is found even if there are commands not found, returns success
