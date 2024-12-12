/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:43:32 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/12 13:50:31 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Variables

	//.42shrc
	static HIST_ENTRY	**history		 = NULL;	//	History array

	static char			*history_file	 = NULL;	//	Path to the physical history file
	static size_t		file_max		 = 2000;	//	Maximum number of entries
	static bool			file_limited	 = false;	//	Indicates if it is limited by a maximum size

	static size_t		mem_max			 = 1000;	//	Maximum number of entries
	static bool			mem_limited		 = false;	//	Indicates if it is limited by a maximum size

	static size_t		length			 = 0;		//	Current number of entries
	static size_t		capacity		 = 10;		//	Array size
	static size_t		event			 = 1;		//	Current event number

	static size_t		position		 = 0;		//	Current position within the array
	// static size_t		search_pos		 = 0;		//	Position for CTRL + R searches

	static char			**tmp_history	= NULL;		//	Temporary array to store history entries
	static size_t		tmp_length		= 0;		//	Number of entries currently in the temporary history

#pragma endregion

#pragma region Local

	#pragma region Read

		//	Read entries from a file into a temporary array
		static int read_history_file(const char *filename) {
			if (!filename) filename = history_file;
			if (!filename || !options.hist_on || !options.hist_local || mem_max == 0) return (1);

			int fd = open(filename, O_RDONLY);
			if (fd < 0) return (1);

			tmp_length = 0;
			char	*line = NULL;

			//	Reserve space for the temporary history
			tmp_history = safe_malloc(HIST_MAXSIZE * sizeof(char *));
			while ((line = ft_get_next_line(fd)) && tmp_length < HIST_MAXSIZE) {
				//	Replace "\\n" with actual newlines
				const char *src = line; char *dst = line;
				while (*src) {
					if (src[0] == '\\' && src[1] == 'n') { *dst++ = '\n'; src += 2; }
					else 								   *dst++ = *src++;
				} *dst = '\0';
				tmp_history[tmp_length++] = line;
			} tmp_history[tmp_length] = NULL;

			close(fd);
			return (0);
		}

		//	Add the entries to the history
		int history_read(const char *filename) {
			if (!read_history_file(filename) || tmp_length < 1) return (1);

			//	Adjust capacity to the required size
			while (capacity <= tmp_length) capacity *= 2;
			if (capacity > mem_max) capacity = mem_max;

			//	Adjust `tmp_length` to load only the most recent entries
			if (capacity + 1 > tmp_length)	tmp_length = 0;
			else							tmp_length -= (capacity + 1);

			//	Allocate memory for the final history
			history = malloc((capacity + 1) * sizeof(char *));
			if (!history) {
				array_free(tmp_history);
				exit_error(NO_MEMORY, 1, NULL, true);
			}

			//	Copy entries from the temporary array to the final history
			while (tmp_history[tmp_length]) {
				if (length >= mem_max) { break; }
				history[length]->line = tmp_history[tmp_length];
				history[length]->length = ft_strlen(history[length]->line);
				history[length]->event = event++;
				history[length++]->data = NULL;
				tmp_history[tmp_length++] = NULL;
			} history[length] = NULL;

			//	Free the temporary history
			array_free(tmp_history); tmp_history = NULL; tmp_length = 0;

			return (0);
		}

	#pragma endregion

	#pragma region Write

		//	42HISTFILESIZE

		//	Save the entries to a file
		int history_write(const char *filename) {
			if (!filename) filename = history_file;
			if (!filename || !history || !options.hist_on || !options.hist_local || file_max == 0 || length == 0) return (1);

			int fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0664);
			if (fd < 0) return (1);

			for (size_t i = 0; i < length; i++) {
				if (history[i]) {
					//	Replace newlines with "\\n"
					const char *entry = history[i]->line;
					while (entry) {
						if (*entry == '\n')	write(fd, "\\n", 2);
						else				write(fd, entry, 1);
						entry++;
					}
				}
			}

			close(fd);
			return (0);
		}

	#pragma endregion

	#pragma region Set Size

		//	Set a maximum size for the history
		void history_set_size(int value, int type) {
			if (type == HIST_MEM)	{  mem_max = ft_min(ft_max(0, value), HIST_MAXSIZE);  mem_limited = false; }
			if (type == HIST_FILE)	{ file_max = ft_min(ft_max(0, value), HIST_MAXSIZE); file_limited = false; }
		}

	#pragma endregion

	#pragma region Unset Size

		//	Remove the size assignment for the history
		void history_unset_size(int type) {
			if (type == HIST_MEM)	{  mem_max = -1;  mem_limited = true; }
			if (type == HIST_FILE)	{ file_max = -1; file_limited = true; }
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

		//	Display the previous entry (display it on screen)
		int history_prev() {
			if (!options.hist_on || mem_max == 0) return (1);
			size_t curr = position;
			if (position > 0) position--;

			if (position != curr && position <= capacity && history && history[position]->line) {
				write(STDOUT_FILENO, history[position]->line, history[position]->length);
				write(STDOUT_FILENO, "\n", 1);
			}

			return (0);
		}

	#pragma endregion

	#pragma region Next

		//	Display the next entry (display it on screen)
		int history_next() {
			if (!options.hist_on || mem_max == 0) return (1);
			size_t curr = position;
			position = ft_min(position + 1, ft_max(ft_min(mem_max, HIST_MAXSIZE - 1), 0));

			if (position != curr && position <= capacity && history && history[position]->line) {
				write(STDOUT_FILENO, history[position]->line, history[position]->length);
				write(STDOUT_FILENO, "\n", 1);
			}

			return (0);
		}

	#pragma endregion

	#pragma region Set Position

		//	Change the history position (does not display it on screen)
		void history_set_pos(size_t pos) {
			position = ft_min(pos, ft_max(ft_min(mem_max, HIST_MAXSIZE - 1), 0));
		}

	#pragma endregion

	#pragma region Get Position

		//	Return the history position (does not display it on screen)
		size_t history_get_pos() {
			return (position);
		}

	#pragma endregion

#pragma endregion

#pragma region Initialize

	int history_initialize() {

		return (0);
	}

#pragma endregion

// SIGHUP, SIGTERM para guardar historial
