/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:43:32 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/11 13:56:42 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Variables

	//.42shrc
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

#pragma region Local

	#pragma region Read

		//	Lee las entradas desde un archivo
		void history_read(const char *filename) {
			(void) filename;
		}

	#pragma endregion

	#pragma region Write

		//	Guarda las entradas en un archivo
		void history_write(const char *filename) {
			(void) filename;
		}

	#pragma endregion

	#pragma region Set Size

		//	Asigna un tamaño maximo al historial
		void history_set_size(int size) {
			(void) size;
		}

	#pragma endregion

	#pragma region Unset Size

		//	Elimina la asignacion de tamaño máximo
		void history_unset_size() {
			
		}

	#pragma endregion

#pragma endregion

#pragma region Add

	#pragma region Add

		//	Añade una entrada al historial
		void history_add(const char *line) {
			(void) line;
		}

	#pragma endregion

	#pragma region Replace

		//	Reemplaza la entrada indicada
		void history_replace_entry(int pos, const char *line, void *data) {
			(void) pos;
			(void) line;
			(void) data;
		}

	#pragma endregion

	#pragma region Timestamp

		//	Actualiza el timestamp de la entrada
		void history_update_timestamp(int pos) {
			(void) pos;
		}

	#pragma endregion

#pragma endregion

#pragma region Delete

	#pragma region Remove

		//	Elimina la entrada indicada
		void history_remove(int pos) {
			(void) pos;
		}

	#pragma endregion

	#pragma region Clear

		//	Elimina todas las entradas
		void history_clear() {

		}

	#pragma endregion

#pragma endregion

#pragma region Get

	#pragma region Where

		//	Devuelve el indice de la entrada actual
		int history_where() {
			return (0);
		}

	#pragma endregion

	#pragma region List

		//	Devuelve un array de punteros al historial
		HIST_ENTRY **history_list() {
			return (NULL);
		}

	#pragma endregion

	#pragma region Length

		//	Obtiene el tamaño del historial
		size_t history_length() {
			return (0);
		}

	#pragma endregion

	#pragma region Get

		//	Devuelve un puntero a la entrada indicada
		HIST_ENTRY *history_get(int pos) {
			(void) pos;
			return (NULL);
		}

	#pragma endregion

	#pragma region Current

		//	Devuelve un puntero a la entrada actual del historial
		HIST_ENTRY *history_current() {
			return (NULL);
		}

	#pragma endregion

	#pragma region Event

		//	Devuelve un puntero a la con el evento indicado
		HIST_ENTRY *history_event(size_t event) {
			(void) event;
			return (NULL);
		}

	#pragma endregion

	#pragma region Search

		HIST_ENTRY *history_search(const char *term, bool reverse) {
			(void) term;
			(void) reverse;
			return (NULL);
		}

	#pragma endregion

#pragma endregion

#pragma region Navigate

	#pragma region Previous

		//	Muestra la entrada anterior
		void history_prev() {

		}

	#pragma endregion

	#pragma region Next

		//	Muestra la entrada siguiente
		void history_next() {

		}

	#pragma endregion

	#pragma region Set Position

		//	Cambia la posicion del historial (no lo muestra en pantalla)
		void history_set_pos(int pos) {
			(void) pos;
		}

	#pragma endregion

	#pragma region Get Position

		//	Devuelve la posicion del historial (no lo muestra en pantalla)
		int history_get_pos() {
			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region Initialize

	int history_initialize() {
		return (0);
	}

#pragma endregion

// SIGHUP, SIGTERM para guardar historial
