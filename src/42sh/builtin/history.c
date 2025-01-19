/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 21:02:57 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/19 18:29:16 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins/builtins.h"

//  history [-c] [-d offset] [n]
//	history [-rw] [filename]
//	history [-sp] arg [arg...]

// -c				Elimina el historial
// -d[+-n]			Elimina el evento desde el inicio/fin o con el numero de evento
// -r [filename]	Carga todo el archivo en el historial actual	([filename], HISTFILE or ~/.bash_history)
// -w [filename]	Guarda el historial actual en el archivo		([filename], HISTFILE or ~/.bash_history)
// -s				Añade el argumento en el historial y se aplica HISTCONTROL									(no añade el de history, pero solo si tiene argumentos)
// -p				Muestra cada argumento en una linea después de realizar todas las expansiones				(no añade el comando al historial, ni siquiera el de history, pero solo si tiene argumentos)
// --help			Muestra la ayuda

// !! es igual a !-1
// !![n] se sustituye por el comando con ese numero de evento
// ![+n] se sustituye por el comando [n] desde el principio
// ![-n] se sustituye por el comando [n] desde el final

//	HISTFILE		= ~/.bash_history		Ruta al archivo del historial
//
//	HISTFILESIZE	= 2000					Número de entradas en el archivo
//	HISTSIZE		= 1000					Número de entradas en la memoria
//											Variable no definida	Usa valor por defecto (1000)
//											Variable vacía			Usa valor por defecto (1000)
//											Valor no numérico		Usa valor por defecto (1000)
//											Valor negativo			Usa valor por defecto (1000)
//											Valor grande			Usa un límite máximo  (5000)
//
//	HISTCONTROL		= ignoreboth
//											ignorespace: No guarda comandos que comiencen con un espacio.
//						 					ignoredups:	 No guarda comandos duplicados consecutivos.
// 											ignoreboth:	 Combina ignorespace e ignoredups.
//											erasedups:	 Elimina duplicados.
//
// Nota:	si se juntan flags, tiene prioridad [--help], [--version], [-c], [-d], [-s], [-p], [-w], [-r]
