/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:44:04 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/07 16:35:25 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

char	*prompt_PS1 = NULL;
char	*prompt_PS2 = NULL;

void set_prompt(int type, char *new_prompt) {
	char * tmp_prompt = NULL;
	if (type == PS1 && prompt_PS1) { free(prompt_PS1); prompt_PS1 = NULL; }
	if (type == PS2 && prompt_PS2) { free(prompt_PS2); prompt_PS2 = NULL; }
	if (new_prompt) {
		//	Procesa barras, variables ($var & \u)
		tmp_prompt = safe_strdup(new_prompt);
	} else {
		//	Se asigna a PS1 "\\\\\\\\\\\\\\$USER-\\u"	//	"\\\\\\\$USER-\u"
		//char *PS1 = "\\\\\\$USER-\\u";					//	"\\\$USER-\u"
		//	Procesa \\ barras invetidas
		//char *medio = "\\\\$USER-\\u";					//	"\\$USER-\u"
		//	Procesa barras, variables ($var & \u)
		//char *final = "\\kobayashi-kobayashi";
		tmp_prompt = safe_strdup("promp from PS1> ");
	}
	if (type == PS1) prompt_PS1 = tmp_prompt;
	if (type == PS2) prompt_PS2 = tmp_prompt;
}

//	PROMPT_COMMAND			Ejecuta antes de mostrar el prompt
//	PS1						Prompt normal
//	PS2						Prompt para heredoc y lineas extras

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
