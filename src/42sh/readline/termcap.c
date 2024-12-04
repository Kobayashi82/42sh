/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:07:05 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/04 19:35:33 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

void init_terminal_data() {
	char *termtype = getenv("TERM");
	if (!termtype) termtype = "dumb";

	int success = tgetent(NULL, termtype);
	if (success < 0)	write(2, "Could not access the termcap data base.\n", 41);
	if (success == 0)	write(2, "Terminal type is not defined.\n", 31);
}

void move_cursor(int x, int y) {
    char *move_cmd;
    char buffer[2048]; // Buffer para almacenar el comando de movimiento

    // Obtén el código para mover el cursor a una posición específica (x, y)
    move_cmd = tgetstr("cm", NULL); // "cm" es el código de comando para "cursor motion"

    // Si `move_cmd` no es NULL, mueve el cursor a la posición solicitada
    if (move_cmd != NULL) {
        snprintf(buffer, sizeof(buffer), move_cmd, y, x); // Formateamos el código con las coordenadas (y, x)
        fputs(buffer, stdout); // Enviar el comando a la terminal
    } else {
        fprintf(stderr, "No se pudo obtener el comando para mover el cursor.\n");
    }
}

void cursor_up() {
    //char buffer[2048];
    char *action;

    action = tgetstr("up", NULL);

    if (action)	fputs(action, stdout);
    else		fprintf(stderr, "No se pudo obtener el comando para mover el cursor hacia arriba.\n");
}

void cursor_down() {
	
}

void cursor_left() {
	
}

void cursor_right() {
	
}
