/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:43:32 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/03 14:13:36 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/readinput/history.h"
	#include "hashes/variable.h"
	#include "expansion/globbing.h"
	#include "main/options.h"
	#include "utils/libft.h"
	#include "utils/print.h"

#pragma endregion

#pragma region "Variables"

	#define HIST_MAXSIZE 50000							// Hard maximum history entries

	static char			hist_control[4096];				// History control settings used to exclude when adding to history
	static char			hist_ignore[4096];				// Command patterns used to exclude when adding to history
	static char			hist_timeformat[4096];			// Timestamp format used when displaying history entries
	static char			hist_file[4096];				// Path to the history file
	static size_t		hist_filesize		= 2000;		// Maximum number of commands in the history file
	static size_t		hist_size			= 1000;		// Maximum number of commands kept in memory

	static size_t		capacity			= 10;		// Size of the history array
	static size_t		length				= 0;		// Length of the history array
	static size_t		position			= 0;		// Current position in the history
	static size_t		event				= 1;		// Current event number

	static size_t		first_line			= 1;		// First command number used when displaying history entries
	static size_t		first_new_pos		= 0;		// Position in the history where newly added commands start
	static size_t		last_file_pos		= 0;		// Position in the history of the last command read from file

	
	static int			added				= 0;		// Tracks whether the last added command is still considered active; cleared when ignored, removed, or after navigation
	static int			begining			= 0;		// Flag set when the history position is at the first entry (can't move up)
	static int			middle				= 0;		// Flag set when navigating within the history (not at begining or end)

	static HIST_ENTRY	**history			= NULL;		// History array

#pragma endregion

#pragma region "Size"

	#pragma region "Get"

		//	Get the maximum size for the history
		size_t history_size_get(int type) {
			if (type == HIST_MEM)	return (hist_size);
			if (type == HIST_FILE)	return (hist_filesize);

			return (hist_size);
		}

	#pragma endregion

	#pragma region "Set"

		//	Set a maximum size for the history
		void history_size_set(size_t value, int type) {
			if (type == HIST_FILE) hist_filesize = ft_min(value, HIST_MAXSIZE);
			if (type == HIST_MEM) {
				hist_size  = ft_min(value, HIST_MAXSIZE);
				if (hist_size < length) {
					capacity = (hist_size < 10) ? 10 : hist_size;

					HIST_ENTRY **tmp_history = calloc(capacity + 1, sizeof(HIST_ENTRY *));
					size_t i = 0;

					for (size_t start = length - hist_size; start < length; ++start) {
						tmp_history[i++] = history[start];
						history[start] = NULL;
					}
					tmp_history[i] = NULL;

					history_clear();

					length = hist_size;
					position = (length > 0) ? length - 1 : 0;
					history = tmp_history;
				}
			}
		}

	#pragma endregion

	#pragma region "Unset"

		//	Remove the size limitation for the history
		void history_size_unset(int type) {
			if (type == HIST_MEM)	hist_size = INT_MAX;
			if (type == HIST_FILE)	hist_filesize = INT_MAX;
		}

	#pragma endregion

	#pragma region "Re-Size"

		//	Initialize or resize the history
		//	- If `initialize` is 1 or `history` is NULL, it sets up a new buffer with a default capacity
		//	- If the buffer is full, it doubles the capacity by a power of 2 and preserves existing entries
		static void history_resize(int initialize) {
			if (initialize || !history) {
				if (history) history_clear();
				capacity = 10;
				position = first_new_pos = last_file_pos = length = 0;
				history = calloc(capacity + 1, sizeof(HIST_ENTRY *));
			} else if (length == capacity) {
				capacity *= 2;

				HIST_ENTRY **new_history = calloc(capacity + 1, sizeof(HIST_ENTRY *));

				for (size_t i = 0; i < length && history[i]; ++i) {
					new_history[i] = history[i];
				}

				free(history);
				history = new_history;
			}
		}

	#pragma endregion

#pragma endregion

#pragma region "Local"

	#pragma region "Set File"

		void history_file_set(const char *filename) {
			if (!filename || !*filename) {
				hist_file[0] = '\0';
				return;
			}

			strncpy(hist_file, filename, 4095);
			hist_file[ft_strlen(filename)] = '\0';
		}

	#pragma endregion

	#pragma region "Read File"

		//	Read entries from a history file into a temporary array
		static int read_file(const char *filename, HIST_ENTRY **tmp_history, size_t *tmp_length) {
			if (!filename || access(filename, R_OK))	filename = hist_file;
			if (!filename || access(filename, R_OK))	return (1);
			if (!hist_size)								return (0);

			int fd = open(filename, O_RDONLY);
			if (fd < 0)									return (1);

			*tmp_length = 0;
			char *line = NULL;

			*tmp_history = calloc(HIST_MAXSIZE, sizeof(HIST_ENTRY));

			while ((line = get_next_line(fd))) {
				if (*tmp_length >= HIST_MAXSIZE - 1)	{	free(line);	break;		}
				if (ft_isspace_s(line))					{	free(line);	continue;	}

				char *sep1 = strchr(line, '\x1F');
				if (!sep1) {
					// No metadata (use full line)
					char *newline = strchr(line, '\n');
					if (newline) *newline = '\0';
					(*tmp_history)[*tmp_length].line = line;
					(*tmp_history)[*tmp_length].length = ft_strlen(line);
					(*tmp_history)[*tmp_length].timestamp = time(NULL);
					(*tmp_length)++;
					continue;
				}

				// Metadata
				*sep1 = '\0';
				char *first_field = line;
				char *second_field = sep1 + 1;
				char *sep2 = strchr(second_field, '\x1F');
				long first_field_long = atol(first_field);
				long second_field_long = atol(second_field);
				time_t timestamp = time(NULL);
				size_t cmd_length = 0;
				char *command_start = NULL;

				if (sep2) {
					*sep2 = '\0';
					timestamp = (first_field_long > 0) ? (time_t)first_field_long : time(NULL);
					cmd_length = second_field_long;
					command_start = sep2 + 1;
				} else {
					cmd_length = first_field_long;
					command_start = second_field;
				}

				char *final_cmd = NULL;

				if (cmd_length > 0) {
					size_t current_len = ft_strlen(command_start);

					if (current_len < cmd_length) {
						char *full_cmd = malloc(cmd_length + 1);
						if (!full_cmd) {
							free(line);
							continue;
						}

						memcpy(full_cmd, command_start, current_len);
						size_t read_so_far = current_len;

						while (read_so_far < cmd_length) {
							char *next = get_next_line(fd);
							if (!next) break;

							size_t next_len = ft_strlen(next);
							size_t to_copy = cmd_length - read_so_far;
							if (to_copy > next_len) to_copy = next_len;

							memcpy(full_cmd + read_so_far, next, to_copy);
							read_so_far += to_copy;

							free(next);
						}

						full_cmd[cmd_length] = '\0';
						final_cmd = full_cmd;
						free(line);
					} else {
						char *newline = strchr(command_start, '\n');
						if (newline) *newline = '\0';

						memmove(line, command_start, ft_strlen(command_start) + 1);
						final_cmd = line;
					}
				} else {
					char *newline = strchr(command_start, '\n');
					if (newline) *newline = '\0';

					memmove(line, command_start, ft_strlen(command_start) + 1);
					final_cmd = line;
				}

				if (final_cmd) {
					(*tmp_history)[*tmp_length].line = final_cmd;
					(*tmp_history)[*tmp_length].length = ft_strlen(final_cmd);
					(*tmp_history)[*tmp_length].timestamp = timestamp;
					(*tmp_length)++;
				} else {
					free(line);
				}
			}

			get_next_line(-1);
			close(fd);

			return (0);
		}

	#pragma endregion

	#pragma region "Read"

		//	Add the entries to the history
		int history_read(const char *filename) {
			HIST_ENTRY *tmp_history = NULL;
			size_t tmp_length = 0;

			if (read_file(filename, &tmp_history, &tmp_length)) {
				history_resize(1);
				return (1);
			}

			if (tmp_length < 1) { 
				if (tmp_history) free(tmp_history);
				tmp_history = NULL;
				history_resize(1);
				return (0); 
			}

			if (history) history_clear();

			//	Adjust capacity to the required size
			while (capacity <= tmp_length) capacity *= 2;
			if (capacity > hist_size) capacity = hist_size;

			//	Load only the most recent entries
			size_t start_idx = (tmp_length > capacity) ? tmp_length - capacity : 0;
			for (size_t i = 0; i < start_idx; i++) {
				if (tmp_history[i].line) {
					free(tmp_history[i].line);
					tmp_history[i].line = NULL;
				}
			}

			first_line = start_idx + 1;
			length = 0;

			history = calloc(capacity + 1, sizeof(HIST_ENTRY *));

			//	Copy entries from the temporary to final history
			for (size_t i = start_idx; i < tmp_length; i++) {
				if (length >= hist_size) {
					free(tmp_history[i].line);
					continue;
				}

				history[length] = malloc(sizeof(HIST_ENTRY));
				history[length]->line = tmp_history[i].line;
				history[length]->length = ft_strlen(history[length]->line);
				history[length]->timestamp = tmp_history[i].timestamp;
				length++;
			}
			history[length] = NULL;

			//	Free the temporary history
			free(tmp_history);
			tmp_history = NULL;
			tmp_length = 0;

			position = (length > 0) ? length - 1 : 0;
			first_new_pos = length;
			last_file_pos = (first_new_pos) ? first_new_pos - 1 : 0;

			return (0);
		}

	#pragma endregion

	#pragma region "Write"

		//	Save the entry to a file
		int history_write(const char *filename) {
			if (!filename) filename = hist_file;

			int fd;
			if (options.histappend)	fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0664);
			else					fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0664);
			if (fd < 0) return (1);

			if (!history || !hist_filesize || !length) {
				close(fd);
				return (0);
			}

			if (!options.histappend) {
				size_t i = 0;
				if (length > hist_filesize) i = length - hist_filesize;

				for (; i < length; i++) {
					if (history[i] && history[i]->line) {
						char timestamp_str[32];
						char length_str[32];

						// Timestamp
						if (history[i]->timestamp > 0) {
							snprintf(timestamp_str, sizeof(timestamp_str), "%ld", (long)history[i]->timestamp);
							write(fd, timestamp_str, ft_strlen(timestamp_str));
							write(fd, "\x1F", 1);
						}

						// Length
						size_t cmd_len = ft_strlen(history[i]->line);
						snprintf(length_str, sizeof(length_str), "%zu", cmd_len);
						write(fd, length_str, ft_strlen(length_str));
						write(fd, "\x1F", 1);

						// Command
						write(fd, history[i]->line, cmd_len);
						write(fd, "\n", 1);
					}
				}
			} else {
				size_t i = 0;
				if (length > hist_filesize) i = length - hist_filesize;

				for (; i < length; i++) {
					if (history[i] && history[i]->line) {
						char timestamp_str[32];
						char length_str[32];

						// Timestamp
						if (history[i]->timestamp > 0) {
							snprintf(timestamp_str, sizeof(timestamp_str), "%ld", (long)history[i]->timestamp);
							write(fd, timestamp_str, ft_strlen(timestamp_str));
							write(fd, "\x1F", 1);
						}

						// Length
						size_t cmd_len = ft_strlen(history[i]->line);
						snprintf(length_str, sizeof(length_str), "%zu", cmd_len);
						write(fd, length_str, ft_strlen(length_str));
						write(fd, "\x1F", 1);

						// Command
						write(fd, history[i]->line, cmd_len);
						write(fd, "\n", 1);
					}
				}
			}

			close(fd);
			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Add"

	#pragma region "Set Hist Control"

	//	Set hist_control
	void history_hist_control_set(const char *value) {
		if (!value || !*value) {
			hist_control[0] = '\0';
			return;
		}

		strncpy(hist_control, value, 4095);
		hist_control[ft_strlen(value)] = '\0';
	}

	#pragma endregion

	#pragma region "Set Hist Ignore"

	//	Set hist_ignore
	void history_hist_ignore_set(const char *value) {
		if (!value || !*value) {
			hist_ignore[0] = '\0';
			return;
		}

		strncpy(hist_ignore, value, 4095);
		hist_ignore[ft_strlen(value)] = '\0';
	}

	#pragma endregion

	#pragma region "Erase Dups"

		//	Remove copies of the same line in the history
		static void erase_dups(const char *line, size_t pos) {
			if (!history || !length) return;

			size_t i = length;
			while (i-- > 0 && history[i]) {
				if (i != pos && !strcmp(history[i]->line, line)) history_remove_position(i);
			}
		}

	#pragma endregion

	#pragma region "Hist Ignore"

		#pragma region "Free"

			// Free patterns array
			static void free_patterns(char **patterns) {
				if (!patterns) return;

				for (int i = 0; patterns[i]; ++i) free(patterns[i]);
				free(patterns);
			}

		#pragma endregion

		#pragma region "Split"

			// Split hist_ignore by ':'
			static char **split_patterns(char *hist_ignore) {
				if (!hist_ignore || !*hist_ignore) return (NULL);

				int count = 1;
				for (int i = 0; hist_ignore[i]; i++) {
					if (hist_ignore[i] == ':') count++;
				}

				char **patterns = calloc(count + 1, sizeof(char *));
				if (!patterns) return (NULL);

				int		i = 0;
				char	*colon, *start = hist_ignore;

				while ((colon = strchr(start, ':'))) {
					if (colon - start > 0) {
						patterns[i] = ft_substr(start, 0, colon - start);
						if (!patterns[i++]) return (free_patterns(patterns), NULL);
					}
					start = colon + 1;
				}
				if (*start) {
					patterns[i] = ft_strdup(start);
					if (!patterns[i]) return (free_patterns(patterns), NULL);
				}

				return (patterns);
			}

		#pragma endregion

		#pragma region "Check"

			// Checks whether the line should be ignored according to hist_ignore
			static int check_histignore(char *line) {
				if (!line || !*line || !hist_ignore[0]) return (0);

				char **patterns = split_patterns(hist_ignore);
				if (!patterns) return (0);

				for (int i = 0; patterns[i]; ++i) {
					if (!strcmp(patterns[i], "&")) {
						history_set_pos_last();
						HIST_ENTRY *entry = history_entry_current();
						if (entry && entry->line && !strcmp(line, entry->line)) {
							free_patterns(patterns);
							return (1);
						}
					} else if (match_pattern(line, patterns[i])) {
						free_patterns(patterns);
						return (1);
					}
				}

				free_patterns(patterns);
				return (0);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Add"

		//	Add an entry to the history
		int history_add(char *line, int force) {
			if (!line || ft_isspace_s(line) || !hist_size) return (1);

			event++;
			int ignoredups = 0, ignorespace = 0, erasedups = 0;
			if (hist_control[0] != '\0') {
				char *token = ft_strtok(hist_control, ":", 30);
				while (token) {
					if (!strcmp(token, "ignoredups"))	ignoredups = 1;
					if (!strcmp(token, "ignorespace"))	ignorespace = 1;
					if (!strcmp(token, "erasedups"))	erasedups = 1;
					if (!strcmp(token, "ignoreboth"))	ignoredups = 1; ignorespace = 1;
					token = ft_strtok(NULL, ":", 30);
				}
			}

			if (!force && check_histignore(line)) { added = 0; return (1); }
			if (!force && ignoredups && length && history && history[length - 1] && history[length - 1]->line && !strcmp(history[length - 1]->line, line)) { added = 0; return (1); }
			if (!force && ignorespace && isspace(*line)) { added = 0; return (1); }
			if (!force && erasedups) erase_dups(line, INT_MAX);

			history_resize(0);
			if (length >= hist_size) {
				free(history[0]->line);
				free(history[0]);
				history[0] = NULL;

				for (size_t i = 0; i < length; ++i) {
					history[i] = history[i + 1];
				}

				if (length > 0) length -= 1;
				first_line++;
			}

			history[length] = malloc(sizeof(HIST_ENTRY));
			history[length]->line = ft_strdup(line);
			history[length]->length = ft_strlen(line);
			history[length++]->timestamp = time(NULL);
			history[length] = NULL;
			history_set_pos_last();
			added = 1;
			begining = middle = 0;

			return (0);
		}

	#pragma endregion

	#pragma region "Replace"

		//	Replace the indicated entry
		int history_replace(size_t pos, char *line) {
			if (!history || !line || ft_isspace_s(line) || !length || !hist_size) return (1);

			int ignoredups = 0, ignorespace = 0, erasedups = 0;
			if (hist_control[0] != '\0') {
				char *token = ft_strtok(hist_control, ":", 30);
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
				history[pos]->line = ft_strdup(line);
				history[pos]->length = ft_strlen(line);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Clone"

		//	Free a cloned history
		static void history_clone_free(HIST_ENTRY **copy, int i) {
			while (i >= 0) {
				free(copy[i]->line);
				free(copy[i]);
				i--;
			}
			free(copy);
		}

		//	Return a clone of the history
		HIST_ENTRY **history_clone() {
			if (!history) return (NULL);

			HIST_ENTRY **copy = malloc((length + 1) * sizeof(HIST_ENTRY *));
			if (!copy) return (NULL);

			for (size_t i = 0; i < length; ++i) {
				copy[i] = malloc(sizeof(HIST_ENTRY));
				if (!copy[i]) return (history_clone_free(copy, i - 1), NULL);

				copy[i]->line = ft_strdup(history[i]->line);
				if (!copy[i]->line) return (history_clone_free(copy, i), NULL);

				copy[i]->timestamp = history[i]->timestamp;
				copy[i]->length = history[i]->length;
			}

			copy[length] = NULL;

			return (copy);
		}

	#pragma endregion

#pragma endregion

#pragma region "Remove"

	#pragma region "Position"

		//	Remove the indicate entry by a position (n)
		void history_remove_position(size_t pos) {
			if (!history || !length) return;

			if (history && pos < length && history[pos]) {
				if (history[pos]->line) free(history[pos]->line);
				free(history[pos]);
				history[pos] = NULL;

				for (size_t i = pos; i < length; ++i) {
					history[i] = history[i + 1];
				}

				if (pos < first_new_pos) first_new_pos--;
				last_file_pos = (first_new_pos) ? first_new_pos - 1 : 0;
				if (length > 0) length -= 1;
				if (length && position == length) { history_set_pos_last(); added = 0; }
			}
		}

	#pragma endregion

	#pragma region "Offset Range"

		//	Remove the indicated entries by an offset range (2 offset separated by '-'. ex: +5-20 or -25-+35)
		void history_remove_offset_range(size_t start, size_t end) {
			if (!history || !length) return;

			if (start >= length || end >= length) return;
			if (start > end) return;
			
			for (size_t i = start; i <= end; ++i) {
				if (history[i]) {
					free(history[i]->line);
					free(history[i]);
					history[i] = NULL;
				}
			}

			for (size_t i = end + 1; i < length; ++i) {
				history[i - (end - start + 1)] = history[i];
			}

			if (length >= end - start + 1) length -= (end - start + 1);
			history[length] = NULL;

			if (start < first_new_pos) first_new_pos -= (first_new_pos - start);
			last_file_pos = (first_new_pos) ? first_new_pos - 1 : 0;
			if (length && position == length) { history_set_pos_last(); added = 0; }
		}

	#pragma endregion

	#pragma region "Offset"

		//	Remove the indicate entry by an offset (+n, -n, n)
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

			history_remove_position(pos);
		}

	#pragma endregion

	#pragma region "Current"

		//	Remove the current entry
		void history_remove_current(int remove_event) {
			if (!history || !length) return;

			if (history && position < length && history[position]) {
				if (history[position]->line) free(history[position]->line);
				free(history[position]); history[position] = NULL;
				for (size_t i = position; i < length; ++i) {
					history[i] = history[i + 1];
				}
				length -= 1;

				if (position < first_new_pos) first_new_pos--;
				last_file_pos = (first_new_pos) ? first_new_pos - 1 : 0;
				if (length && position >= length) { history_set_pos_last(); added = 0; }
				if (remove_event) event--;
			}
		}

	#pragma endregion

	#pragma region "Last"

		//	Remove the last entry (id added)
		void history_remove_last_if_added(int remove_event) {
			if (!history || !length || !added) return;

			history_set_pos_last();
			history_remove_current(remove_event);
			added = 0;
		}

	#pragma endregion

#pragma endregion

#pragma region "Clear"

	//	Clear all entries
	void history_clear() {
		if (history) {
			for (size_t i = 0; i < length; ++i) {
				if (history && history[i]) {
					if (history[i]->line) free(history[i]->line);
					free(history[i]);
					history[i] = NULL;
				}
			}

			free(history);
			history = NULL;
		}

		capacity = 10;
		position = first_new_pos = last_file_pos = length = 0;
		first_line = 1;
	}

#pragma endregion

#pragma region "Get"

	#pragma region "Entry"

		#pragma region "Position"

			//	Return a pointer to the entry with the indicated position
			HIST_ENTRY *history_entry_position(size_t pos) {
				if (history && pos < length) return (history[pos]);

				return (NULL);
			}

		#pragma endregion

		#pragma region "Offset"

			//	Return a pointer to the entry with the indicated position
			HIST_ENTRY *history_entry_offset(int offset) {
				if (!offset) return (NULL);

				size_t pos = 0;
				if (offset < 0) {
					if ((size_t)(-offset) > length) return (NULL);
					pos = length + offset;
				} else {
					if ((size_t)(offset - first_line) > length) return (NULL);
					pos = offset - first_line;
				}

				if (history && pos < length) return (history[pos]);
				return (NULL);
			}

		#pragma endregion

		#pragma region "Current"

			//	Return a pointer to the current entry
			HIST_ENTRY *history_entry_current() {
				if (history && position < length) return (history[position]);
				return (NULL);
			}

		#pragma endregion

		#pragma region "Last"

			//	Return a pointer to the last entry if added
			HIST_ENTRY *history_entry_last_if_added() {
				if (!history || !length || !added) return (NULL);

				history_set_pos_last();
				return (history_entry_current());
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Position"

		#pragma region "Current"

			//	Return the current position in the history
			size_t history_position() {
				return (position);
			}

		#pragma endregion

		#pragma region "Offset"

			//	Returns the position to the entry with the indicated offset
			int history_position_offset(int offset, size_t *out) {
				if (!offset) return (1);

				if (offset < 0) {
					if ((size_t)(-offset) > length) return (1);
					*out = length + offset;
				} else {
					if ((size_t)(offset - first_line) > length) return (1);
					*out = offset - first_line;
				}

				return (0);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Length"

		//	Return the length of the history
		size_t history_length() {
			return (length);
		}

	#pragma endregion

	#pragma region "Event"

		//	Return the event number
		size_t history_event() {
			return (event);
		}

	#pragma endregion

	#pragma region "HistCMD"

		// Return the next command number available ($HISTCMD)
		size_t history_histcmd() {
			return (first_line + length);
		}

	#pragma endregion

#pragma endregion

#pragma region "Navigate"

	#pragma region "Previous"

		//	Return the previous entry line
		char *history_prev() {
			if (!history || !options.history || !hist_size || !length) return (NULL);

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
			if (!history || !options.history || !hist_size || !length) return (NULL);

			if (position == length) position = length -1;
			begining = 0; middle = 1;
			if (position >= length - 1) { middle = 0; return (NULL); }
			position++;

			return (history[position]->line);
		}

	#pragma endregion

	#pragma region "Set Position"

		//	Set the position in the history
		void history_set_pos(size_t pos) {
			position = ft_min(ft_max(pos, 0), length - 1);
		}

		//	Set the position in the history to the last entry
		void history_set_pos_last() {
			begining = 0; middle = 0;
			position = 0;
			if (length > 0) position = length - 1;
		}

	#pragma endregion

#pragma endregion

#pragma region "Print"

	#pragma region "Set Time Format"

		//	Set timeformat for timestamp
		void history_hist_timeformat_set(const char *format) {
			if (!format || !*format) {
				hist_timeformat[0] = '\0';
				return;
			}

			strncpy(hist_timeformat, format, 4095);
			hist_timeformat[ft_strlen(format)] = '\0';
		}

	#pragma endregion

	#pragma region "Print"

		//	Print all entries
		int history_print(size_t offset, int hide_pos) {
			if (!history || !length) return (1);
			if (offset > length) offset = length;

			print(STDOUT_FILENO, NULL, RESET);

			for (size_t i = length - offset; i < length && history[i]; ++i) {
				if (!hide_pos) {
					char *txt_line = ft_itoa(first_line + i);
					int spaces = 5 - ft_strlen(txt_line);
					while (spaces--) print(STDOUT_FILENO, " ", JOIN);
					print(STDOUT_FILENO, txt_line, FREE_JOIN);
					print(STDOUT_FILENO, "  ", JOIN);
				}

				if (hist_timeformat[0] != '\0') {
					char time_buffer[128];
					struct tm *tm_info = localtime(&history[i]->timestamp);
					if (tm_info && strftime(time_buffer, sizeof(time_buffer), hist_timeformat, tm_info) > 0) {
						print(STDOUT_FILENO, time_buffer, JOIN);
					}
				}

				print(STDOUT_FILENO, history[i]->line, JOIN);
				print(STDOUT_FILENO, "\n", JOIN);
			}

			print(STDOUT_FILENO, NULL, PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Initialize"

	//	Initialize the history
	int history_initialize() {
		char *value = NULL;
		if ((value = variables_find_value(vars_table, "42_HISTFILE")))			history_file_set(value);
		if ((value = variables_find_value(vars_table, "42_HISTSIZE")))			history_size_set(atol(value), HIST_MEM);
		if ((value = variables_find_value(vars_table, "42_HISTFILESIZE")))		history_size_set(atol(value), HIST_FILE);
		if ((value = variables_find_value(vars_table, "42_HISTTIMEFORMAT")))	history_hist_timeformat_set(value);
		if ((value = variables_find_value(vars_table, "42_HISTCONTROL")))		history_hist_control_set(value);
		if ((value = variables_find_value(vars_table, "42_HISTIGNORE")))		history_hist_ignore_set(value);

		history_size_set(5, HIST_MEM);
		history_read(NULL);
		
		return (0);
	}

#pragma endregion
