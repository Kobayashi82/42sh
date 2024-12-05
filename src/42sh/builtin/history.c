/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 21:02:57 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/05 21:03:16 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

// Variable no definida		Usa valor por defecto (1000)
// Variable vacía			Usa valor por defecto (1000)
// Valor no numérico		Usa valor por defecto (1000)
// Valor negativo			Usa valor por defecto (1000)
// Valor grande				Usa un límite máximo  (10000)

// Opción					Acción						Sobrescribe		Lee solo nuevas líneas
// -a		Agrega historial de la sesión actual			No					No
// -n		Carga líneas nuevas desde el archivo			No					Sí
// -r		Carga todo el archivo en el historial actual	No					No
// -w		Guarda el historial actual en el archivo		Sí					No
// -c		Elimina el historial
// -d[+-n]	Elimina el evento desde el inicio/fin o con el numero de evento
// -p		Expansion de comando '!-1' sin ejecutar (imprime el argumento si no es expansible)
// -s		Añade el argumento en el historial

//  history [-c] [-d offset] [n]
//	history -anrw [filename]
//	history -ps arg [arg...]

// Display the history list with line numbers, prefixing each modified
// entry with a `*'.  An argument of N lists only the last N entries.

// If FILENAME is given, it is used as the history file.  Otherwise,
// if HISTFILE has a value, that is used, else ~/.bash_history.

// !! es igual a !-1
// !![n] se sustituye por el comando con ese numero de evento
// ![+n] se sustituye por el comando [n] desde el principio
// ![-n] se sustituye por el comando [n] desde el final

// HISTFILE			Ruta al archivo del historial
// HISTSIZE			Número de entradas lógicas en memoria
// HISTFILESIZE		Número de lineas físicas en el archivo
// HISTCONTROL		ignorespace: No guarda comandos que comiencen con un espacio.
// 					ignoredups: No guarda comandos duplicados consecutivos.
// 					ignoreboth: Combina ignorespace e ignoredups.
// HISTAPPEND		Append en vez de sobreescribir el historial (on/off) añadir a set -o
