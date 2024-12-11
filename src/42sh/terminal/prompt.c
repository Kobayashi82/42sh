/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:44:04 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/11 13:26:11 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Variables

	char	*prompt_PS1 = NULL;			//	Prompt normal
	char	*prompt_PS2 = NULL;			//	Prompt para heredoc y lineas extras

#pragma endregion

#pragma region Utils

	#pragma region Backslashes

		static char *backslashes(char *str) {
			char *tmp = str;

			while (tmp && *tmp) {
				if (*tmp == '\\' && *(tmp + 1) == '\\')
					ft_memmove(tmp, tmp + 1, ft_strlen(tmp + 1) + 1);
				tmp++;
			}

			return (str);
		}

	#pragma endregion

#pragma endregion

#pragma region Set

	void set_prompt(int type, char *new_prompt) {
		// char *str_PS1 = "\\\\\\$USER-\\u > ";						//	"\\\$USER-\u"
		char *str_PS1 = BLUE600"["GREEN600"kobayashi"BLUE600"]"GREEN600"-"RED600"42"Y"sh"BLUE600"> "NC;
		char *tmp_prompt = NULL;

		if (type == PS1 && prompt_PS1) { free(prompt_PS1); prompt_PS1 = NULL; }
		if (type == PS2 && prompt_PS2) { free(prompt_PS2); prompt_PS2 = NULL; }

		if (new_prompt) {
			//	Procesa barras, variables ($var & \u)
			tmp_prompt = safe_strdup(new_prompt);
		} else {
			tmp_prompt = backslashes(safe_strdup(str_PS1));	//	"\\$USER-\u"
			//	Procesa barras, variables ($var & \u)
		}

		if (type == PS1) prompt_PS1 = tmp_prompt;
		if (type == PS2) prompt_PS2 = tmp_prompt;
	}

#pragma endregion

#pragma region Information

	//	PROMPT_COMMAND			Ejecuta antes de mostrar el prompt

	// \d	Fecha en formato día mes fecha (e.g., Tue Mar 7).
	// \h	Nombre del host hasta el primer punto (e.g., mi-host).
	// \H	Nombre completo del host (e.g., mi-host.localdomain).
	// \j	Número de trabajos en segundo plano manejados por el shell.
	// \l	Nombre del terminal en el que estás (e.g., tty1, pts/0).
	// \s	Nombre del shell (e.g., bash).
	// \t	Hora actual en formato 24 horas (hh:mm:ss).
	// \T	Hora actual en formato 12 horas (hh:mm:ss).
	// \@	Hora actual en formato 12 horas con AM/PM (hh:mm AM).
	// \u	Nombre del usuario actual.
	// \v	Versión de 42sh
	// \V	Versión de 42sh
	// \w	Directorio de trabajo actual (con rutas cortas, e.g., ~/Documentos).
	// \W	Nombre base del directorio actual (solo el último componente, e.g., Documentos).
	// \!	Número del comando actual en el historial.
	// \#	Número del comando actual en esta sesión.
	// \$	Muestra # si eres root, o $ si eres un usuario normal.

	// \a	Carácter de campana (alerta sonora).
	// \n	Nueva línea.
	// \r	Retorno de carro.
	// \e	Carácter de escape ASCII (código 033, útil para colores).
	// \[	Inicia una secuencia de caracteres no imprimibles (útil para colores y estilos).
	// \]	Finaliza una secuencia de caracteres no imprimibles.

#pragma endregion
