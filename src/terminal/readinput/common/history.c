/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:43:32 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/30 23:17:22 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// SIGHUP, SIGTERM para guardar historial

#pragma region "Includes"

	#include "terminal/readinput/history.h"
	#include "hashes/variable.h"
	#include "main/options.h"
	#include "utils/libft.h"
	#include "utils/print.h"

	#include <limits.h>
	#include <stdint.h>

#pragma endregion

#pragma region "Variables"

	#define HIST_MAXSIZE 5000

	//.42shrc
	static HIST_ENTRY	**history			= NULL;		//	History array

	static char			history_file[4096];				//	Path to the physical history file
	static size_t		file_max			= 2000;		//	Maximum number of entry
	static int			file_unlimited		= 0;		//	Indicates if it is limited by a maximum size

	static size_t		mem_max				= 1000;		//	Maximum number of entry
	static int			mem_unlimited		= 0;		//	Indicates if it is limited by a maximum size

	static size_t		first_line_fix		= 1;		//	First line to print
	static size_t		first_line			= 1;		//	First line to print
	static size_t		length				= 0;		//	Current number of entry
	static size_t		capacity			= 10;		//	Array size
	static size_t		event				= 1;		//	Current event number

	static size_t		position			= 0;		//	Current position within the array

	static int			begining			= 0;
	static int			middle				= 0;
	static int			added				= 0;

	static char			**tmp_history		= NULL;		//	Temporary array to store history entry
	static size_t		tmp_length			= 0;		//	Number of entry currently in the temporary history

#pragma endregion

#pragma region "File"

	void history_file_set(const char *filename) {
		if (!filename) return;
		strcpy(history_file, filename);
	}

#pragma endregion

#pragma region "Size"

	#pragma region "Get Size"

		//	Get the maximum size for the history
		size_t history_get_size(int type) {
			if (type == HIST_MEM)	{ return (mem_max); }
			if (type == HIST_FILE)	{ return (file_max); }
			return (mem_max);
		}

	#pragma endregion

	#pragma region "Set Size"

		//	Set a maximum size for the history
		void history_set_size(size_t value, int type) {
			size_t new_size = ft_min(ft_max(0, value), HIST_MAXSIZE);

			if (type == HIST_FILE) { file_max = new_size; file_unlimited = 0; }
			if (type == HIST_MEM)  { mem_max = new_size;  mem_unlimited = 0;
				if (mem_max < length) {
					HIST_ENTRY **tmp_history = calloc((mem_max * 2) + 1, sizeof(HIST_ENTRY *));
					size_t i = 0;

					for (size_t start = length - mem_max; start < length && history[start]; ++start) {
						tmp_history[i++] = history[start]; history[start] = NULL;
					} tmp_history[i] = NULL;
					history_clear();

					length = mem_max;
					capacity = ft_max(0, mem_max * 2);
					position = length > 0 ? length - 1 : 0;
					history = tmp_history;
				}
			}
		}

	#pragma endregion

	#pragma region "Unset Size"

		//	Remove the size limitation for the history
		void history_unset_size(int type) {
			if (type == HIST_MEM)	{  mem_max = INT_MAX;  mem_unlimited = 1; }
			if (type == HIST_FILE)	{ file_max = INT_MAX; file_unlimited = 1; }
		}

	#pragma endregion

	#pragma region "Resize"

		//	Initialize or resize the history
		//	- If `initialize` is 1 or `history` is NULL, it sets up a new buffer with a default capacity
		//	- If the buffer is full, it doubles the capacity by a power of 2 and preserves existing entries
		static void history_resize(int initialize) {
			if (initialize || !history) {
				if (history) history_clear();
				capacity = 10; length = 0;
				history = calloc(capacity + 1, sizeof(HIST_ENTRY *));
			} else if (length == capacity) {
				capacity *= 2;
				HIST_ENTRY **new_history = calloc(capacity + 1, sizeof(HIST_ENTRY *));
				for (size_t i = 0; i < length && history[i]; ++i)
					new_history[i] = history[i];
				free(history);
				history = new_history;
			}
		}

	#pragma endregion

#pragma endregion

#pragma region "Local"

	#pragma region "Read"

		//	Read entries from a history file into a temporary array
		static int read_history_file(const char *filename) {
			if (!filename || access(filename, R_OK)) filename = history_file;
			if (!filename || access(filename, R_OK)) return (1);
			if (!mem_max && !mem_unlimited)  return (0);

			int fd = open(filename, O_RDONLY);
			if (fd < 0) return (1);

			tmp_length = 0;
			char *line = NULL;

			//	Reserve space for the temporary history
			tmp_history = calloc(HIST_MAXSIZE, sizeof(char *));

			while (tmp_length < HIST_MAXSIZE - 1 && (line = get_next_line(fd))) {
				if (ft_isspace_s(line)) { free(line); continue; }
				//	Replace "\\n" with actual newlines
				const char *src = line; char *dst = line;
				while (*src) {
					if (src[0] == '\\' && src[1] == 'n')	{ *dst++ = '\n'; src += 2; }
					else if (src[0] == '\n')				{ *dst++ = '\0'; break;	   }
					else 									  *dst++ = *src++;
				} *dst = '\0';
				tmp_history[tmp_length++] = line;
			} tmp_history[tmp_length] = NULL;

			get_next_line(-1);
			close(fd);

			return (0);
		}

		//	Add the entries to the history
		int history_read(const char *filename) {
			if (read_history_file(filename)) {		 	history_resize(1); return (1); }
			if (tmp_length < 1) { free(tmp_history);	history_resize(1); return (0); }

			if (history) history_clear();
			event = 1;
			//	Adjust capacity to the required size
			while (capacity <= tmp_length) capacity *= 2;
			if (capacity > mem_max && !mem_unlimited) capacity = mem_max;

			//	Adjust `tmp_length` to load only the most recent entry
			if (capacity + 1 > tmp_length)	tmp_length = 0;
			else							tmp_length -= (capacity + 1);

			first_line_fix = tmp_length + 1;
			first_line = first_line_fix;

			//	Allocate memory for the final history
			history = calloc(capacity + 1, sizeof(HIST_ENTRY *));

			length = 0;
			//	Copy entry from the temporary array to the final history
			while (tmp_history[tmp_length]) {

				if (length >= mem_max && !mem_unlimited) {
					free(tmp_history[tmp_length]);
					tmp_history[tmp_length++] = NULL;
					continue;
				}

				history[length] = malloc(sizeof(HIST_ENTRY));
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

	#pragma region "Write"

		//	Save the entry to a file
		int history_write(const char *filename) {
			if (!filename) filename = history_file;
			if (!filename)				return (1);
			if (!options.hist_local)	return (0);

			int fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0664);
			if (fd < 0) return (1);

			if (!history || !file_max || !length) { close(fd); return (0); }

			size_t i = 0;
			if (length > file_max) i = length - file_max;

			for (;i < length; i++) {
				if (history[i]) {
					//	Replace newlines with "\\n"
					const char *entry = history[i]->line;
					while (*entry) {
						if (*entry == '\n')	write(fd, "\\n", 2);
						else				write(fd, entry, 1);
						entry++;
					}
					write(fd, "\n", 1);
				}
			}

			close(fd);
			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Add"

		//	Remove copies of the same line in the history
		static void erase_dups(const char *line, size_t pos) {
			if (!history || !length) return;

			size_t i = length;
			while (i-- > 0 && history[i])
				if (i != pos && !strcmp(history[i]->line, line)) history_remove(i);
		}

	#pragma region "Add"

		//	Add an entry to the history
		int history_add(char *line, int force) {
			if (!line || ft_isspace_s(line) || !mem_max) return (1);

			int ignoredups = 0, ignorespace = 0, erasedups = 0;
			char *control = variables_find_value(vars_table, "42_HISTCONTROL");
			if (control) {
				char *token = ft_strtok(control, ":", 30);
				while (token) {
					if (!strcmp(token, "ignoredups"))	ignoredups = 1;
					if (!strcmp(token, "ignorespace"))	ignorespace = 1;
					if (!strcmp(token, "erasedups"))	erasedups = 1;
					if (!strcmp(token, "ignoreboth"))	ignoredups = 1; ignorespace = 1;
					token = ft_strtok(NULL, ":", 30);
				}
			}

			if (!force && ignoredups && length && history && history[length - 1] && history[length - 1]->line && !strcmp(history[length - 1]->line, line)) { added = 0; return (1); }
			if (!force && ignorespace && isspace(*line)) { added = 0; return (1); }
			if (!force && erasedups) erase_dups(line, INT_MAX);

			history_resize(0);
			if (length >= mem_max && !mem_unlimited) {
				free(history[0]->line);
				free(history[0]->data);
				free(history[0]); history[0] = NULL;
				for (size_t i = 0; i < length; ++i)
					history[i] = history[i + 1];
				length -= 1;
				first_line++;
			}
			history[length] = malloc(sizeof(HIST_ENTRY));
			history[length]->line = ft_strdup(line);
			history[length]->length = ft_strlen(line);
			history[length]->event = event++;
			history[length++]->data = NULL;
			history[length] = NULL;
			history_set_pos_end();
			added = 1;

			begining = middle = 0;

			return (0);
		}

	#pragma endregion

	#pragma region "Replace"

		//	Replace the indicated entry
		int history_replace(size_t pos, char *line, void *data) {
			if (!history || !line || ft_isspace_s(line) || !length || !mem_max) return (1);

			int ignoredups = 0, ignorespace = 0, erasedups = 0;
			char *control = variables_find_value(vars_table, "42_HISTCONTROL");
			if (control) {
				char *token = ft_strtok(control, ":", 30);
				while (token) {
					if (!strcmp(token, "ignoredups"))	ignoredups = 1;
					if (!strcmp(token, "ignorespace"))	ignorespace = 1;
					if (!strcmp(token, "erasedups"))		erasedups = 1;
					if (!strcmp(token, "ignoreboth"))	ignoredups = 1; ignorespace = 1;
					token = ft_strtok(NULL, ":", 30);
				}
			}

			if (pos > 0 && ignoredups && history[pos - 1] && history[pos - 1]->line && !strcmp(history[pos - 1]->line, line)) return (1);
			if (pos == length - 1 && ignoredups && history[pos + 1] && history[pos + 1]->line && !strcmp(history[pos + 1]->line, line)) return (1);
			if (ignorespace && isspace(*line)) return (1);
			if (erasedups) erase_dups(line, pos);

			if (history && pos < length && history[pos]) {
				if (history[pos]->line) free(history[pos]->line);
				if (history[pos]->data) free(history[pos]->data);
				history[pos]->line = ft_strdup(line);
				history[pos]->length = ft_strlen(line);
				history[pos]->data = data;
			}

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Delete"

	#pragma region "Remove"

		//	Remove the indicated offset range
		void history_remove_offset_range(size_t start, size_t end) {
			if (!history || !length) return;

			if (start >= length || end >= length) return;
			if (start > end) return;
			
			for (size_t i = start; i <= end; ++i) {
				if (history[i]) {
					free(history[i]->line);
					free(history[i]->data);
					free(history[i]);
					history[i] = NULL;
				}
			}

			for (size_t i = end + 1; i < length; ++i)
				history[i - (end - start + 1)] = history[i];

			length -= (end - start + 1);
			history[length] = NULL;

		    if (first_line > first_line_fix) first_line -= (end - start + 1);
			if (first_line < first_line_fix) first_line = first_line_fix;

			if (position >= length) {
				history_set_pos_end();
				added = 0;
			}
		}

		//	Remove the indicate entry by an offset
		void history_remove_offset(int offset) {
			if (!history || !length) return;

			size_t pos;
			if (offset < 0) {
				if ((size_t)(-offset) > length) return;				//	Negative offset out of range
				pos = length + offset;
			} else {
				if (!offset || (size_t)offset > length) return;		//	Positivo offset out of range
				pos = offset - 1;
			}

			if (history && pos < length && history[pos]) {
				if (history[pos]->line) free(history[pos]->line);
				if (history[pos]->data) free(history[pos]->data);
				free(history[pos]); history[pos] = NULL;
				for (size_t i = pos; i < length; ++i)
					history[i] = history[i + 1];
				length -= 1;

				if (first_line > first_line_fix) first_line--;

				if (length && position == length) { history_set_pos_end(); added = 0; }
			}
		}

		//	Remove the indicate entry
		void history_remove(size_t pos) {
			if (!history || !length) return;

			if (history && pos < length && history[pos]) {
				if (history[pos]->line) free(history[pos]->line);
				if (history[pos]->data) free(history[pos]->data);
				free(history[pos]); history[pos] = NULL;
				for (size_t i = pos; i < length; ++i)
					history[i] = history[i + 1];
				length -= 1;

				if (first_line > first_line_fix) first_line--;

				if (length && position == length) { history_set_pos_end(); added = 0; }
			}
		}

		//	Remove the indicate event
		void history_remove_event(size_t event) {
			if (!history || !length) return;

			for (size_t i = 0; i < length && history[i]; ++i)
				if (history[i]->event == event) history_remove(i);
		}

		//	Remove the current entry
		void history_remove_current(int remove_event) {
			if (!history || !length) return;

			if (history && position < length && history[position]) {
				if (history[position]->line) free(history[position]->line);
				if (history[position]->data) free(history[position]->data);
				free(history[position]); history[position] = NULL;
				for (size_t i = position; i < length; ++i)
					history[i] = history[i + 1];
				length -= 1;

				if (first_line > first_line_fix) first_line--;

				if (length && position >= length) { history_set_pos_end(); added = 0; }
				if (remove_event) event--;
			}
		}

		//	Remove the last entry
		void history_remove_last_if_added(int remove_event) {
			if (!history || !length || !added) return;

			history_set_pos_end();
			history_remove_current(remove_event);
			added = 0;
		}

	#pragma endregion

	#pragma region "Clear"

		//	Clear all entries
		void history_clear() {
			if (!history) return;

			for (size_t i = 0; i < length; ++i) {
				if (history && history[i]) {
					if (history[i]->line) free(history[i]->line);
					if (history[i]->data) free(history[i]->data);
					free(history[i]); history[i] = NULL;
				}
			}
			if (history) { free(history); history = NULL; }
			position = 0; length = 0; capacity = 10;
			first_line = first_line_fix;
		}

	#pragma endregion

#pragma endregion

#pragma region "Get"

	#pragma region "Clone"

		//	Return a clone of the history
		HIST_ENTRY **history_clone() {
			if (!history) return (NULL);

			HIST_ENTRY **copy = malloc((length + 1) * sizeof(HIST_ENTRY *));

			for (size_t i = 0; i < length && history[i]; ++i)
				copy[i] = history[i];
			copy[length] = NULL;

			return (copy);
		}

	#pragma endregion

	#pragma region "Length"

		//	Return the length of the history
		size_t history_length() {
			return (length);
		}

	#pragma endregion

	#pragma region "Get"

		//	Returns the index associated with an offset
		int history_offset_to_index(int offset, size_t *out, int is_plus) {
			if (!offset) return (1);

			if (offset < 0) {
				if ((size_t)(-offset) > length) return (1);
				*out = length + offset;
			} else if (is_plus) {
				if ((size_t)offset > length) return (1);
				*out = offset - 1;
			} else {
				if ((size_t)(offset - first_line) > length) return (1);
				*out = offset - first_line;
			}

			return (0);
		}

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

		//	Return a pointer to the last entry if added
		HIST_ENTRY *history_get_last_if_added() {
			if (!history || !length || !added) return (NULL);

			history_set_pos_end();
			return (history_current());
		}

	#pragma endregion

	#pragma region "Event"

		//	Return a pointer to the entry with the indicated event
		HIST_ENTRY *history_event(size_t event) {
			if (!history) return (NULL);

			for (size_t i = 0; i < length && history[i]; ++i)
				if (history[i]->event == event) return (history[i]);

			return (NULL);
		}

		//	Return the position to the entry with the indicated event
		int history_event_pos(size_t event) {
			if (!history) return (-1);

			for (size_t i = 0; i < length && history[i]; ++i)
				if (history[i]->event == event) return (i);

			return (-1);
		}

	#pragma endregion

#pragma endregion

#pragma region "Navigate"

	#pragma region "Previous"

		//	Return the previous entry line
		char *history_prev() {
			if (!history || !options.history || !mem_max || !length) return (NULL);

			if (position == length) position = length -1;
			if (begining) return (NULL);
			if (position > 0 && middle) position--;
			if (position == 0) begining = 1;
			middle = 1;

			return (history[position]->line);
		}

	#pragma endregion

	#pragma region "Next"

		//	Return the next entry line
		char *history_next() {
			if (!history || !options.history || !mem_max || !length) return (NULL);

			if (position == length) position = length -1;
			begining = 0; middle = 1;
			if (position >= length - 1) { middle = 0; return (NULL); }
			position++;

			return (history[position]->line);
		}

	#pragma endregion

	#pragma region "Get Position"

		//	Return the current position in the history
		size_t history_get_pos() {
			return (position);
		}

	#pragma endregion

	#pragma region "Set Position"

		//	Change the position in the history
		void history_set_pos(size_t pos) {
			position = ft_min(ft_max(pos, 0), length - 1);
		}

		//	Set the position in the history to the last entry
		void history_set_pos_end() {
			begining = 0; middle = 0;
			position = 0;
			if (length > 0) position = length - 1;
		}

	#pragma endregion

#pragma endregion

#pragma region "Print"

	//	Print all entries
	int history_print(size_t offset, int hide_events) {
		if (!history || !length) return (1);
		if (offset > length) offset = length;

		print(STDOUT_FILENO, NULL, RESET);

		for (size_t i = length - offset; i < length && history[i]; ++i) {
			if (!hide_events) {
				char *txt_line = ft_itoa(first_line + i);
				int spaces = 5 - ft_strlen(txt_line);
				while (spaces--) print(STDOUT_FILENO, " ", JOIN);
				print(STDOUT_FILENO, txt_line, FREE_JOIN);
				print(STDOUT_FILENO, "  ", JOIN);
			}
			print(STDOUT_FILENO, history[i]->line, JOIN);
			print(STDOUT_FILENO, "\n", JOIN);
		}

		print(STDOUT_FILENO, NULL, PRINT);

		return (0);
	}


#pragma endregion

#pragma region "Initialize"

	//	Initialize the history
	int history_initialize() {
		char *home = get_home();
		if (home) home = ft_strjoin_sep(home, "/", ".42sh_history", 1);
		history_file_set(home); free(home);
		history_read(NULL);

		return (0);
	}

#pragma endregion
