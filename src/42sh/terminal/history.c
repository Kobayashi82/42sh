/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:43:32 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/14 21:47:48 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Variables

	//.42shrc
	bool				ignorespace			= true;
	bool				ignoredups			= true;
	bool				erasedups			= false;

	static HIST_ENTRY	**history			= NULL;		//	History array

	static char			*history_file		= NULL;		//	Path to the physical history file
	static size_t		file_max			= 2000;		//	Maximum number of entry
	static bool			file_limited		= false;	//	Indicates if it is limited by a maximum size

	static size_t		mem_max				= 1000;		//	Maximum number of entry
	static bool			mem_limited			= false;	//	Indicates if it is limited by a maximum size

	static size_t		length				= 0;		//	Current number of entry
	static size_t		capacity			= 10;		//	Array size
	static size_t		event				= 1;		//	Current event number

	static size_t		position			= 0;		//	Current position within the array
	// static size_t		search_pos		 = 0;		//	Position for CTRL + R searches
	static bool			begining			= false;
	static bool			middle				= false;

	static char			**tmp_history		= NULL;		//	Temporary array to store history entry
	static size_t		tmp_length			= 0;		//	Number of entry currently in the temporary history

#pragma endregion

#pragma region Size

	#pragma region Get Size

		int history_get_size(int type) {
			if (type == HIST_MEM)	{ return (mem_max); }
			if (type == HIST_FILE)	{ return (file_max); }
			return (mem_max);
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

	#pragma region Resize

		static void history_resize(bool initialize) {
			if (initialize || !history) {
				if (history) history_clear();
				capacity = 10; length = 0;
				history = safe_calloc(capacity + 1, sizeof(HIST_ENTRY *));
			} else if (length == capacity) {
				capacity *= 2;
				HIST_ENTRY **new_history = safe_calloc(capacity + 1, sizeof(HIST_ENTRY *));
				for (size_t i = 0; i < length && history[i]; ++i)
					new_history[i] = history[i];
				free(history);
				history = new_history;
			}
		}

	#pragma endregion

#pragma endregion

#pragma region Local

	#pragma region Read

		//	Read entry from a file into a temporary array
		static int read_history_file(const char *filename) {
			if (!filename) filename = history_file;
			if (!filename) return (1);
			if (!mem_max)  return (0);

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
					if (src[0] == '\\' && src[1] == 'n')	{ *dst++ = '\n'; src += 2; }
					else if (src[0] == '\n')				{ *dst++ = '\0'; break;	   }
					else 									*dst++ = *src++;
				} *dst = '\0';
				tmp_history[tmp_length++] = line;
			} tmp_history[tmp_length] = NULL;

			close(fd);
			return (0);
		}

		//	Add the entry to the history
		int history_read(const char *filename) {
			if (read_history_file(filename)) {		 history_resize(true); return (1); }
			if (tmp_length < 1) { free(tmp_history); history_resize(true); return (0); }

			//	Adjust capacity to the required size
			while (capacity <= tmp_length) capacity *= 2;
			if (capacity > mem_max) capacity = mem_max;

			//	Adjust `tmp_length` to load only the most recent entry
			if (capacity + 1 > tmp_length)	tmp_length = 0;
			else							tmp_length -= (capacity + 1);

			//	Allocate memory for the final history
			history = ft_calloc(capacity + 1, sizeof(HIST_ENTRY *));
			if (!history) {
				array_free(tmp_history);
				exit_error(NO_MEMORY, 1, NULL, true);
			}

			length = 0;
			//	Copy entry from the temporary array to the final history
			while (tmp_history[tmp_length]) {
				if (length >= mem_max) { break; }
				history[length] = malloc(sizeof(HIST_ENTRY));
				if (!history[length]) { history_clear(); break; }
				history[length]->line = tmp_history[tmp_length];
				history[length]->length = ft_strlen(history[length]->line);
				history[length]->event = event++;
				history[length++]->data = NULL;
				tmp_history[tmp_length++] = NULL;
			} history[length] = NULL;

			//	Free the temporary history
			array_free(tmp_history); tmp_history = NULL; tmp_length = 0;
			position = length - 1;

			return (0);
		}

	#pragma endregion

	#pragma region Write

		//	Save the entry to a file
		int history_write(const char *filename) {
			if (!filename) filename = history_file;
			if (!filename)				return (1);
			if (!options.hist_local)	return (0);

			int fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0664);
			if (fd < 0) return (1);

			if (!history || !file_max || !length) { close (fd); return (0); }

			size_t i = 0;
			if (length > file_max) i = length - file_max;

			for (;i < length; i++) {
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

#pragma endregion

#pragma region Add

	#pragma region Add

		//	Remove copies of the line in the history
		static void erase_dups(const char *line, size_t pos) {
			if (!history) return;

			size_t i = length;
			while (i-- > 0 && history[i])
				if (i != pos && !ft_strcmp(history[i]->line, line)) history_remove(i);
		}

		//	Add an entry to the history
		int history_add(char *line) {
			if (!history || !line || ft_isspace_s(line) || !mem_max) return (1);
			if (ignoredups && history[length - 1] && history[length - 1]->line && !ft_strcmp(history[length - 1]->line, line)) return (1);
			if (ignorespace && ft_isspace(*line)) return (1);
			if (erasedups) erase_dups(line, INT_MAX);

			history_resize(false);
			if (length >= mem_max) {
				free(history[0]->line);
				free(history[0]->data);
				free(history[0]); history[0] = NULL;
				for (size_t i = 0; i < length; ++i)
					history[i] = history[i + 1];
				length -= 1;
			}
			history[length] = safe_malloc(sizeof(HIST_ENTRY));
			history[length]->line = safe_strdup(line);
			history[length]->length = ft_strlen(line);
			history[length]->event = event++;
			history[length++]->data = NULL;
			history[length] = NULL;
			position = length - 1;

			begining = middle = false;
			return (0);
		}

	#pragma endregion

	#pragma region Replace

		//	Replace the indicated entry
		int history_replace(size_t pos, char *line, void *data) {
			if (!history || !line || ft_isspace_s(line) || !length || !mem_max) return (1);
			if (pos > 0 && ignoredups && history[pos - 1] && history[pos - 1]->line && !ft_strcmp(history[pos - 1]->line, line)) return (1);
			if (pos == length - 1 && ignoredups && history[pos + 1] && history[pos + 1]->line && !ft_strcmp(history[pos + 1]->line, line)) return (1);
			if (ignorespace && ft_isspace(*line)) return (1);
			if (erasedups) erase_dups(line, pos);

			if (history && pos < length && history[pos]) {
				if (history[pos]->line) free(history[pos]->line);
				if (history[pos]->data) free(history[pos]->data);
				history[pos]->line = safe_strdup(line);
				history[pos]->length = ft_strlen(line);
				history[pos]->data = data;
			}

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region Delete

	#pragma region Remove

		//	Remove the indicate entry
		void history_remove(size_t pos) {
			if (history && pos < length && history[pos]) {
				if (history[pos]->line) free(history[pos]->line);
				if (history[pos]->data) free(history[pos]->data);
				free(history[pos]); history[pos] = NULL;
				for (size_t i = pos; i < length; ++i)
					history[i] = history[i + 1];
				length -= 1;
			}
		}

		//	Remove the current entry
		void history_remove_current() {
			if (history && position < length && history[position]) {
				if (history[position]->line) free(history[position]->line);
				if (history[position]->data) free(history[position]->data);
				free(history[position]); history[position] = NULL;
				for (size_t i = position; i < length; ++i)
					history[i] = history[i + 1];
				length -= 1;
			}
		}

	#pragma endregion

	#pragma region Clear

		//	Clear all entries
		void history_clear() {
			for (size_t i = 0; i < length; ++i) {
				if (!history || !history[i]) break;
				if (history[i]->line) free(history[i]->line);
				if (history[i]->data) free(history[i]->data);
				free(history[i]); history[i] = NULL;
			}
			if (history) { free(history); history = NULL; }
			position = 0; length = 0; capacity = 10;
		}

	#pragma endregion

#pragma endregion

#pragma region Get

	#pragma region Clone

		//	Return a clone of the history
		HIST_ENTRY **history_clone() {
			if (!history) return (NULL);

			HIST_ENTRY **copy = safe_malloc((length + 1) * sizeof(HIST_ENTRY *));

			for (size_t i = 0; i < length && history[i]; ++i)
				copy[i] = history[i];
			copy[length] = NULL;

			return (copy);
		}

	#pragma endregion

	#pragma region Length

		//	Return the length of the history
		size_t history_length() {
			return (length);
		}

	#pragma endregion

	#pragma region Get

		//	Return a pointer to the indicated entry
		HIST_ENTRY *history_get(size_t pos) {
			if (history && pos < length) return (history[pos]);
			return (NULL);
		}

		//	Return a pointer to the current entry
		HIST_ENTRY *history_current() {
			if (history && position < length) return (history[position]);
			return (NULL);
		}

	#pragma endregion

	#pragma region Event

		//	Return a pointer to the entry with the indicated event
		HIST_ENTRY *history_event(size_t event) {
			if (!history) return (NULL);

			for (size_t i = 0; i < length && history[i]; ++i)
				if (history[i]->event == event) return (history[i]);

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

		//	Return the previous entry line
		char *history_prev() {
			if (!history || !options.hist_on || mem_max == 0 || length == 0) return (NULL);

			if (begining) return (NULL);
			if (position > 0 && middle) position--;
			if (position == 0) begining = true;
			middle = true;

			return (history[position]->line);
		}

	#pragma endregion

	#pragma region Next

		//	Return the next entry line
		char *history_next() {
			if (!history || !options.hist_on || mem_max == 0 || length == 0) return (NULL);

			begining = false; middle = true;
			if (position >= length - 1) { middle = false; return (NULL); }
			position++;

			return (history[position]->line);
		}

	#pragma endregion

	#pragma region Get Position

		//	Return the history position (does not display it on screen)
		size_t history_get_pos() {
			return (position);
		}

	#pragma endregion

	#pragma region Set Position

		//	Change the history position (does not display it on screen)
		void history_set_pos(size_t pos) {
			position = ft_min(pos, ft_max(ft_min(mem_max, HIST_MAXSIZE - 1), 0));
		}


		void history_set_pos_end() {
			begining = false; middle = false;
			position = 0;
			if (length > 0) position = length - 1;
		}

	#pragma endregion

#pragma endregion

#pragma region Print

	void history_print(bool hide_events) {
		if (!history) return;

		print(STDOUT_FILENO, NULL, RESET);

		for (size_t i = 0; i < length && history[i]; ++i) {
			if (!hide_events) {
				char * txt_event = ft_itoa(history[i]->event);
				if (!txt_event) exit_error(NO_MEMORY, 1, NULL, true);
				int spaces = 5 - ft_strlen(txt_event);
				while (spaces--) print(STDOUT_FILENO, " ", JOIN);
				print(STDOUT_FILENO, txt_event, FREE_JOIN);
				print(STDOUT_FILENO, "  ", JOIN);
			}
			print(STDOUT_FILENO, history[i]->line, JOIN);
			print(STDOUT_FILENO, "\n", JOIN);
		}

		print(STDOUT_FILENO, NULL, PRINT);
	}

#pragma endregion

#pragma region Initialize

	int history_initialize() {
		history_read("history");

		return (0);
	}

#pragma endregion

// SIGHUP, SIGTERM para guardar historial
