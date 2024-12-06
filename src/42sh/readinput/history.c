/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:43:32 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/06 14:07:16 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Variables

	// static HIST_ENTRY	**history = NULL;		//	Array del historial
	// static char			*history_file = NULL;	//	Ruta al archivo fisico del historial
	// static bool			limited = false;		//	Indica si está limitado por un tamaño máximo
	// static size_t		max_entries = 500;		//	Número máximo de entradas
	// static size_t		length = 0;				//	Número de entradas actuales
	// static size_t		capacity = 10;			//	Tamaño del array
	// static size_t		search_pos = 0;			//	Posicion para las busquedas CTRL + R
	// static size_t		position = 0;			//	Posición actual dentro del array
	// static size_t		event = 1;				//	Número del próximo evento

#pragma endregion

//	Muestra la entrada anterior
void history_prev() {

}

//	Muestra la entrada siguiente
void history_next() {

}

//	Añade una entrada al historial
void history_add(const char *line) {
	(void) line;
}

//	Lee las entradas desde un archivo
void history_read(const char *filename) {
	(void) filename;
}

//	Guarda las entradas en un archivo
void history_write(const char *filename) {
	(void) filename;
}

//	Elimina todas las entradas
void history_clear() {

}

//	Asigna un tamaño maximo al historial
void history_set_size(int size) {
	(void) size;
}

//	Elimina la asignacion de tamaño máximo
void history_unset_size() {
	
}

//	Devuelve el indice de la entrada actual
int history_where() {
	return (0);
}

//	Devuelve un array de punteros al historial
HIST_ENTRY **history_list() {
	return (NULL);
}

//	Obtiene el tamaño del historial
size_t history_length() {
	return (0);
}

//	Devuelve un puntero a la entrada indicada
HIST_ENTRY *history_get(int pos) {
	(void) pos;
	return (NULL);
}

//	Elimina la entrada indicada
void history_remove(int pos) {
	(void) pos;
}

//	Reemplaza la entrada indicada
void history_replace_entry(int pos, const char *line, void *data) {
	(void) pos;
	(void) line;
	(void) data;
}

//	Devuelve un puntero a la entrada actual del historial
HIST_ENTRY *history_current() {
	return (NULL);
}

//	Cambia la posicion del historial (no lo muestra en pantalla)
void history_set_pos(int pos) {
	(void) pos;
}

//	
HIST_ENTRY *history_event(size_t event) {
	(void) event;
	return (NULL);
}

HIST_ENTRY *history_search(const char *term, bool reverse) {
	(void) term;
	(void) reverse;
	return (NULL);
}

//	Actualiza el timestamp de la entrada
void history_update_timestamp(int pos) {
	(void) pos;
}

// SIGHUP, SIGTERM para guardar historial