/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/21 21:27:09 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Variables"

	#define BUFFER_SIZE 8192

	static char		*g_output;
	static char		g_output_buffer[BUFFER_SIZE + 1];
	static size_t	g_buffer_pos;

	typedef struct s_fmt_token {
		int			is_spec;
		const char	*lit;
		size_t		lit_len;
		int			flags;
		int			width;
		int			precision;
		int			length;
		char		spec;
	} t_fmt_token;

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_printf_help(int format, int no_print) {
			char *name = "printf";
			char *syntax = "printf [-v var] format [arguments]";
			char *description = "Formats and prints ARGUMENTS under control of the FORMAT.";
			char *msg =
				"    Options:\n"
				"      -v var    assign the output to shell variable VAR rather than\n"
				"                display it on the standard output\n\n"

				"    FORMAT is a character string which contains three types of objects: plain\n"
				"    characters, which are simply copied to standard output; character escape\n"
				"    sequences, which are converted and copied to the standard output; and\n"
				"    format specifications, each of which causes printing of the next successive\n"
				"    argument.\n\n"

				"    In addition to the standard format specifications described in printf(1),\n"
				"    printf interprets:\n\n"

				"      %b        expand backslash escape sequences in the corresponding argument\n"
				"      %q        quote the argument in a way that can be reused as shell input\n"
				"      %Q        like %q, but apply any precision to the unquoted argument before\n"
				"                quoting\n"
				"      %(fmt)T   output the date-time string resulting from using FMT as a format\n"
				"                string for strftime(3)\n\n"

				"    The format is re-used as necessary to consume all of the arguments.  If\n"
				"    there are fewer arguments than the format requires,  extra format\n"
				"    specifications behave as if a zero value or null string, as appropriate,\n"
				"    had been supplied.\n\n"

				"    Exit Status:\n"
				"      Returns success unless an invalid option is given or a write or assignment\n"
				"      error occurs.\n";

			if (!no_print) print(STDOUT_FILENO, NULL, P_RESET);

				if (format == HELP_SYNTAX) {
					print(STDOUT_FILENO, ft_strjoin(name, ": ",   J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(syntax, "\n", J_FREE_NONE), P_FREE_JOIN);
				}

				if (format == HELP_DESCRIPTION) {
					print(STDOUT_FILENO, ft_strjoin(name, " - ",       J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(description, "\n", J_FREE_NONE), P_FREE_JOIN);
				}

				if (format == HELP_NORMAL) {
					print(STDOUT_FILENO, ft_strjoin(name, ": ",                      J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(syntax, "\n",                    J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(msg, "\n",                       J_FREE_NONE), P_FREE_JOIN);
				}

				if (format == HELP_MANPAGE) {
					print(STDOUT_FILENO, "NAME\n",                                                 P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", name, " - ",         J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(description, "\n\n",             J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, "SYNOPSYS\n",                                             P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", syntax, "\n\n",      J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, "DESCRIPTION\n",                                          P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(msg, "\n\n",                     J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, "SEE ALSO\n    42sh(1)\n\n",                              P_JOIN);
				}

			if (!no_print) print(STDOUT_FILENO, NULL, P_PRINT);

			return (0);
		}

	#pragma endregion

	#pragma region "Version"

		static int version() {
			char *msg =
				"printf 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Internal"

	#pragma region "Buffer"

		#pragma region "Char"

			static void buffer_char(char c) {
				if (g_buffer_pos >= BUFFER_SIZE) {
					g_output_buffer[g_buffer_pos] = '\0';
					g_buffer_pos = 0;
					g_output = ft_strjoin(g_output, g_output_buffer, J_FREE_VAL_1);
				}
				g_output_buffer[g_buffer_pos++] = c;
			}

		#pragma endregion

		#pragma region "String"

			static void buffer_string(const char *str, size_t len) {
				size_t	space;
				size_t	chunk;

				if (!str || len == 0) return;
				while (len > 0) {
					if (g_buffer_pos >= BUFFER_SIZE) {
						g_output_buffer[g_buffer_pos] = '\0';
						g_buffer_pos = 0;
						g_output = ft_strjoin(g_output, g_output_buffer, J_FREE_VAL_1);
						// write(STDOUT_FILENO, g_output_buffer, g_buffer_pos);
						// g_buffer_pos = 0;
					}
					space = BUFFER_SIZE - g_buffer_pos;
					chunk = (len < space) ? len : space;
					memcpy(g_output_buffer + g_buffer_pos, str, chunk);
					g_buffer_pos += chunk;
					str += chunk;
					len -= chunk;
					if (g_buffer_pos == BUFFER_SIZE) {
						g_output_buffer[g_buffer_pos] = '\0';
						g_buffer_pos = 0;
						g_output = ft_strjoin(g_output, g_output_buffer, J_FREE_VAL_1);
						// write(STDOUT_FILENO, g_output_buffer, g_buffer_pos);
						// g_buffer_pos = 0;
					}
				}
			}

		#pragma endregion

		#pragma region "Flash"

			static void flush_buffer() {
				if (g_buffer_pos > 0) {
					g_output_buffer[g_buffer_pos] = '\0';
					g_buffer_pos = 0;
					g_output = ft_strjoin(g_output, g_output_buffer, J_FREE_VAL_1);
					// write(STDOUT_FILENO, g_output_buffer, g_buffer_pos);
					// g_buffer_pos = 0;
				}
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Parse"

		#pragma region "Flags"

			static void parse_flags(const char **fmt, int *flags) {
				*flags = 0;
				while (**fmt) {
					if		(**fmt == '-')		*flags |= 1;
					else if (**fmt == '+')		*flags |= 2;
					else if (**fmt == ' ')		*flags |= 4;
					else if (**fmt == '#')		*flags |= 8;
					else if (**fmt == '0')		*flags |= 16;
					else						break;
					(*fmt)++;
				}
			}

		#pragma endregion

		#pragma region "Width"

			static int parse_width(const char **fmt) {
				int	width = 0;

				while (isdigit(**fmt)) {
					width = width * 10 + (**fmt - '0');
					(*fmt)++;
				}

				return (width);
			}

		#pragma endregion

		#pragma region "Precision"

			static int parse_precision(const char **fmt) {
				int precision;

				if (**fmt != '.') return (-1);
				(*fmt)++;
				precision = 0;
				while (isdigit(**fmt)) {
					precision = precision * 10 + (**fmt - '0');
					(*fmt)++;
				}

				return (precision);
			}

		#pragma endregion

		#pragma region "Length"

			static int parse_length(const char **fmt) {
				if (**fmt == 'l' && *(*fmt + 1) == 'l') {
					(*fmt) += 2;
					return (2);
				}
				if (**fmt == 'l') {
					(*fmt)++;
					return (1);
				}

				return (0);
			}

		#pragma endregion

		#pragma region "Signed Len"

			static long long parse_signed_len(const char *arg, int length, int *ok) {
				char		*endptr;
				long long	val;

				errno = 0;
				if (length == 2)	val = strtoll(arg, &endptr, 10);
				else				val = (long long)strtol(arg, &endptr, 10);
				if (*arg && (*endptr != '\0' || errno == ERANGE))	*ok = 0;
				else												*ok = 1;

				return (val);
			}

		#pragma endregion

		#pragma region "Unsigned Len"

			static unsigned long long parse_unsigned_len(const char *arg, int length) {
				if (length == 2) return (strtoull(arg, NULL, 10));

				return ((unsigned long long)strtoul(arg, NULL, 10));
			}

		#pragma endregion

		#pragma region "Format"

			static int append_token(t_fmt_token **tokens, size_t *count, size_t *cap, t_fmt_token *tok) {
				t_fmt_token	*new_tokens;
				size_t		new_cap;

				if (*count >= *cap) {
					new_cap = (*cap == 0) ? 16 : (*cap * 2);
					new_tokens = realloc(*tokens, new_cap * sizeof(*new_tokens));
					if (!new_tokens) return (1);
					*tokens = new_tokens;
					*cap = new_cap;
				}
				(*tokens)[(*count)++] = *tok;

				return (0);
			}

			static int parse_format(const char *fmt, t_fmt_token **tokens, size_t *count, int *has_specs) {
				size_t		cap;
				const char	*p;
				const char	*lit_start;
				t_fmt_token	tok;
				const char	*spec_ptr;

				*count = 0;
				*has_specs = 0;
				cap = 0;
				*tokens = NULL;
				p = fmt;
				lit_start = p;
				while (*p) {
					if (*p == '%') {
						if (p > lit_start) {
							tok.is_spec = 0;
							tok.lit = lit_start;
							tok.lit_len = (size_t)(p - lit_start);
							if (append_token(tokens, count, &cap, &tok)) return (1);
						}
						if (*(p + 1) == '%') {
							tok.is_spec = 1;
							tok.flags = 0;
							tok.width = 0;
							tok.precision = -1;
							tok.length = 0;
							tok.spec = '%';
							if (append_token(tokens, count, &cap, &tok)) return (1);
							p += 2;
							lit_start = p;
							continue;
						}
						spec_ptr = p + 1;
						parse_flags(&spec_ptr, &tok.flags);
						tok.width = parse_width(&spec_ptr);
						tok.precision = parse_precision(&spec_ptr);
						tok.length = parse_length(&spec_ptr);
						tok.spec = *spec_ptr;
						tok.is_spec = 1;
						if (append_token(tokens, count, &cap, &tok)) return (1);
						if (tok.spec && tok.spec != '%') *has_specs = 1;
						if (*spec_ptr) spec_ptr++;
						p = spec_ptr;
						lit_start = p;
						continue;
					}
					p++;
				}
				if (p > lit_start) {
					tok.is_spec = 0;
					tok.lit = lit_start;
					tok.lit_len = (size_t)(p - lit_start);
					if (append_token(tokens, count, &cap, &tok)) return (1);
				}

				return (0);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Print"

		#pragma region "Num to Str"

			static int num_to_str(unsigned long long num, char *buffer, int base, int uppercase) {
				const char	*digits;
				char		*ptr;
				int			len;

				digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
				ptr = buffer + 63;
				*ptr = '\0';
				if (num == 0) {
					buffer[0] = '0';
					buffer[1] = '\0';
					return (1);
				}
				while (num > 0) {
					*--ptr = digits[num % base];
					num /= base;
				}
				len = buffer + 63 - ptr;
				memmove(buffer, ptr, len + 1);

				return (len);
			}

		#pragma endregion

		#pragma region "Padding"

			static void print_padding(int count, char pad_char) {
				char	pads[64];
				int		chunk;

				if (count <= 0) return;
				memset(pads, pad_char, sizeof(pads));
				while (count > 0) {
					chunk = (count > (int)sizeof(pads)) ? (int)sizeof(pads) : count;
					buffer_string(pads, (size_t)chunk);
					count -= chunk;
				}
			}

		#pragma endregion

		#pragma region "Int"

			static void print_int(long long num, int width, int precision, int flags) {
				char	buffer[64];
				int		len;
				char	sign;
				int		pad_char;
				int		is_negative;
				int		total_len;
				unsigned long long	abs_num;

				is_negative = (num < 0);
				if (is_negative)						abs_num = (unsigned long long)(-(num + 1)) + 1;
				else									abs_num = (unsigned long long)num;
				if (abs_num == 0 && precision == 0)		len = 0;
				else									len = num_to_str(abs_num, buffer, 10, 0);

				while (len < precision) {
					memmove(buffer + 1, buffer, len + 1);
					buffer[0] = '0';
					len++;
				}

				sign = 0;
				if		(is_negative)	sign = '-';
				else if (flags & 2)		sign = '+';
				else if (flags & 4)		sign = ' ';

				total_len = len + (sign ? 1 : 0);
				pad_char = ((flags & 16) && precision < 0 && !(flags & 1)) ? '0' : ' ';

				if (!(flags & 1) && pad_char == ' ')	print_padding(width - total_len, ' ');
				if (sign)								buffer_char(sign);
				if (!(flags & 1) && pad_char == '0')	print_padding(width - total_len, '0');
				if (len > 0)							buffer_string(buffer, len);
				if (flags & 1)							print_padding(width - total_len, ' ');
			}

		#pragma endregion

		#pragma region "Unsigned"

			static void print_unsigned(unsigned long long num, int base, int width, int precision, int flags, int uppercase) {
				char	buffer[64];
				int		len;
				int		pad_char;
				int		prefix_len;
				int		total_len;

				if (num == 0 && precision == 0)		len = 0;
				else								len = num_to_str(num, buffer, base, uppercase);

				while (len < precision) {
					memmove(buffer + 1, buffer, len + 1);
					buffer[0] = '0';
					len++;
				}
				prefix_len = 0;
				if ((flags & 8) && num != 0) {
					if		(base == 8 && buffer[0] != '0')	prefix_len = 1;
					else if (base == 16)					prefix_len = 2;
				}
				total_len = len + prefix_len;

				pad_char = ((flags & 16) && precision < 0 && !(flags & 1)) ? '0' : ' ';
				if (!(flags & 1) && pad_char == ' ')	print_padding(width - total_len, ' ');
				if ((flags & 8) && num != 0) {
					if (base == 8 && buffer[0] != '0')	buffer_char('0');
					else if (base == 16) {
						buffer_char('0');
						buffer_char(uppercase ? 'X' : 'x');
					}
				}
				if (!(flags & 1) && pad_char == '0')	print_padding(width - total_len, '0');
				if (len > 0)							buffer_string(buffer, len);
				if (flags & 1)							print_padding(width - total_len, ' ');
			}

		#pragma endregion

		#pragma region "Float"

			static void print_float(double num, int width, int precision, int flags) {
				char	buffer[128];
				int		len;
				char	sign;
				int		pad_char;
				int		is_negative;
				int		total_len;
				long	int_part;
				double	frac_part;
				int		i;
				unsigned long long	mult;
				unsigned long long	frac;

				if (precision < 0) precision = 6;
				is_negative = (num < 0);
				if (is_negative) num = -num;

				int_part = (long long)num;
				frac_part = num - int_part;
				mult = 1;
				for (i = 0; i < precision; i++) mult *= 10;
				frac = (unsigned long long)(frac_part * (double)mult + 0.5);
				if (frac >= mult) {
					int_part++;
					frac = 0;
				}

				len = num_to_str((unsigned long long)int_part, buffer, 10, 0);
				if (precision > 0) {
					buffer[len++] = '.';
					for (i = precision - 1; i >= 0; i--) {
						buffer[len + i] = '0' + (frac % 10);
						frac /= 10;
					}
					len += precision;
				}
				buffer[len] = '\0';

				sign = 0;
				if		(is_negative)	sign = '-';
				else if (flags & 2)		sign = '+';
				else if (flags & 4)		sign = ' ';

				total_len = len + (sign ? 1 : 0);
				pad_char = ((flags & 16) && !(flags & 1)) ? '0' : ' ';

				if (!(flags & 1) && pad_char == ' ')	print_padding(width - total_len, ' ');
				if (sign)								buffer_char(sign);
				if (!(flags & 1) && pad_char == '0')	print_padding(width - total_len, '0');
				buffer_string(buffer, len);
				if (flags & 1)							print_padding(width - total_len, ' ');
			}

		#pragma endregion

		#pragma region "Float Scientific"

			static void print_float_scientific(double num, int width, int precision, int flags, char spec) {
				char	fmt[32];
				char	out[256];
				int		pos;
				int		n;
				char	*dyn;

				pos = 0;
				fmt[pos++] = '%';
				if (flags & 1)		fmt[pos++] = '-';
				if (flags & 2)		fmt[pos++] = '+';
				if (flags & 4)		fmt[pos++] = ' ';
				if (flags & 8)		fmt[pos++] = '#';
				if (flags & 16)		fmt[pos++] = '0';
				if (width > 0)		pos += snprintf(fmt + pos, sizeof(fmt) - pos, "%d", width);
				if (precision >= 0)	pos += snprintf(fmt + pos, sizeof(fmt) - pos, ".%d", precision);
				fmt[pos++] = spec;
				fmt[pos] = '\0';

				n = snprintf(out, sizeof(out), fmt, num);
				if (n < 0) return;
				if (n < (int)sizeof(out)) {
					buffer_string(out, (size_t)n);
					return;
				}
				dyn = malloc((size_t)n + 1);
				if (!dyn) return;
				snprintf(dyn, (size_t)n + 1, fmt, num);
				buffer_string(dyn, (size_t)n);
				free(dyn);
			}

		#pragma endregion

		#pragma region "String"

			static void print_string(const char *str, int width, int precision, int flags) {
				int len;
				int print_len;

				if (!str) str = "(null)";
				len = ft_strlen(str);
				print_len = len;
				if (precision >= 0 && precision < len)	print_len = precision;
				if (!(flags & 1))						print_padding(width - print_len, ' ');
				buffer_string(str, print_len);
				if (flags & 1)							print_padding(width - print_len, ' ');
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Process"

		#pragma region "Escapes"

			static int has_escape(const char *str) {
				if (!str) return (0);
				while (*str) {
					if (*str == '\\') return (1);
					str++;
				}

				return (0);
			}

			static char *process_escapes(const char *str) {
				char		*result;
				char		*dst;
				const char	*src;
				int			val;
				int			count;

				result = malloc(strlen(str) + 1);
				if (!result) return (NULL);
				dst = result;
				src = str;
				while (*src) {
					if (*src == '\\' && *(src + 1)) {
						src++;
						if		(*src == 'n')		*dst++ = '\n';
						else if (*src == 't')		*dst++ = '\t';
						else if (*src == 'r')		*dst++ = '\r';
						else if (*src == 'b')		*dst++ = '\b';
						else if (*src == 'a')		*dst++ = '\a';
						else if (*src == 'v')		*dst++ = '\v';
						else if (*src == 'f')		*dst++ = '\f';
						else if (*src == '\\')		*dst++ = '\\';
						else if (*src == '"')		*dst++ = '"';
						else if (*src == '\'')		*dst++ = '\'';
						else if (*src >= '0' && *src <= '7') {
							val = count = 0;
							while (*src >= '0' && *src <= '7' && count < 3) {
								val = val * 8 + (*src - '0');
								src++;
								count++;
							}
							*dst++ = (char)val;
							src--;
						} else if (*src == 'x') {
							val = count = 0;
							src++;
							while (isxdigit(*src) && count < 2) {
								if		(*src >= '0' && *src <= '9')	val = val * 16 + (*src - '0');
								else if (*src >= 'a' && *src <= 'f')	val = val * 16 + (*src - 'a' + 10);
								else if (*src >= 'A' && *src <= 'F')	val = val * 16 + (*src - 'A' + 10);
								src++;
								count++;
							}
							*dst++ = (char)val;
							src--;
						} else if (*src == 'c') {
							free(result);
							return (NULL);
						} else *dst++ = *src;
						src++;
					}
					else *dst++ = *src++;
				}
				*dst = '\0';

				return (result);
			}

		#pragma endregion

		#pragma region "Specifier"

			static int process_specifier_parsed(const t_fmt_token *tok, char **args, int *arg_idx, int arg_count) {
				int			flags;
				int			width;
				int			precision;
				int			length;
				char		spec;
				const char	*arg;
				long long	num;
				int			ok;

				flags = tok->flags;
				width = tok->width;
				precision = tok->precision;
				length = tok->length;
				spec = tok->spec;
				if (!spec) return (1);
				arg = (*arg_idx < arg_count) ? args[*arg_idx] : "";
				if (spec == '%') {
					buffer_char('%');
					return (0);
				}

				if (spec == 'd' || spec == 'i') {
					num = parse_signed_len(arg, length, &ok);
					if (!ok) {
						char err_msg[256];
						snprintf(err_msg, sizeof(err_msg), "%s: printf: %s: invalid number\n", shell.name, arg);
						write(STDERR_FILENO, err_msg, ft_strlen(err_msg));
						num = 0;
					}
					print_int(num, width, precision, flags);
					(*arg_idx)++;
				}
				else if (spec == 'u')
				{
					print_unsigned(parse_unsigned_len(arg, length), 10, width, precision, flags, 0);
					(*arg_idx)++;
				}
				else if (spec == 'o')
				{
					print_unsigned(parse_unsigned_len(arg, length), 8, width, precision, flags, 0);
					(*arg_idx)++;
				}
				else if (spec == 'x')
				{
					print_unsigned(parse_unsigned_len(arg, length), 16, width, precision, flags, 0);
					(*arg_idx)++;
				}
				else if (spec == 'X')
				{
					print_unsigned(parse_unsigned_len(arg, length), 16, width, precision, flags, 1);
					(*arg_idx)++;
				}
				else if (spec == 'f' || spec == 'F')
				{
					print_float(strtod(arg, NULL), width, precision, flags);
					(*arg_idx)++;
				}
				else if (spec == 'e' || spec == 'E' || spec == 'g' || spec == 'G')
				{
					print_float_scientific(strtod(arg, NULL), width, precision, flags, spec);
					(*arg_idx)++;
				}
				else if (spec == 's')
				{
					print_string(arg, width, precision, flags);
					(*arg_idx)++;
				}
				else if (spec == 'c')
				{
					char c = arg[0] ? arg[0] : 0;
					if (!(flags & 1))	print_padding(width - 1, ' ');
					buffer_char(c);
					if (flags & 1)		print_padding(width - 1, ' ');
					(*arg_idx)++;
				}
				else if (spec == 'b')
				{
					if (has_escape(arg)) {
						char *processed = process_escapes(arg);
						if (!processed) return (1);
						print_string(processed, width, precision, flags);
						free(processed);
					} else {
						print_string(arg, width, precision, flags);
					}
					(*arg_idx)++;
				}
				else
				{
					buffer_char('%');
					buffer_char(spec);
					return (1);
				}
				return (0);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Main"

		static int printf_main(int argc, char **argv) {
			char		*format;
			char		*processed_fmt;
			int			arg_idx = 1;
			int			ret = 0;
			int			has_format_specs;
			int			free_fmt;
			t_fmt_token	*tokens;
			size_t		count;
			size_t		i;

			g_buffer_pos = 0;
			if (!argc) return (1);
			format = argv[0];

			processed_fmt = format;
			free_fmt = 0;
			if (has_escape(format)) {
				processed_fmt = process_escapes(format);
				if (!processed_fmt) return (1);
				free_fmt = 1;
			}
			if (parse_format(processed_fmt, &tokens, &count, &has_format_specs)) {
				if (free_fmt) free(processed_fmt);
				return (1);
			}

			do {
				for (i = 0; i < count; i++) {
					if (tokens[i].is_spec) {
						if (process_specifier_parsed(&tokens[i], argv, &arg_idx, argc) != 0) {
							ret = 1;
							break;
						}
					} else {
						buffer_string(tokens[i].lit, tokens[i].lit_len);
					}
				}
			} while (has_format_specs && arg_idx < argc && !ret);

			free(tokens);
			if (free_fmt) free(processed_fmt);
			flush_buffer();

			return (ret);
		}

	#pragma endregion

#pragma endregion

#pragma region "Printf"

	int bt_printf(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "v:", NULL, long_opts, "printf [-v var] format [arguments]", IGNORE_OFF);
		if (!result) return (free_options(result), (errno == E_OPT_MAX || errno == E_OPT_INVALID) ? 2 : 1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_printf_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		if (!result->argc) {
			print(STDERR_FILENO, ft_strjoin(shell.name, ": usage: printf [-v var] format [arguments]\n", J_FREE_NONE), P_FREE_RESET_PRINT);
			return (free_options(result), 1);
		}

		const char *name = NULL;
		t_var *var = NULL;

		if (has_option(result, 'v')) {
			name = get_option_value(result, 'v');
			var = variable_get(shell.env, name, 1);
			if (!var) {
				if (errno == E_NO_MEMORY)			ret = exit_error(E_NO_MEMORY,           1, "printf",   NULL,         EE_FREE_NONE, EE_RETURN);
				if (errno == E_VAR_IDENTIFIER)		ret = exit_error(E_VAR_IDENTIFIER,      1, "printf: ", (char *)name, EE_FREE_NONE, EE_RETURN);
				if (errno == E_VAR_MAX_REFERENCES)	ret = exit_error(E_VAR_MAX_REFERENCES,  1, "printf: ", (char *)name, EE_FREE_NONE, EE_RETURN);
				if (errno == E_VAR_CYCLE_REFERENCE)	ret = exit_error(E_VAR_CYCLE_REFERENCE, 1, "printf: ", (char *)name, EE_FREE_NONE, EE_RETURN);
				if (ret) return (free_options(result), 1);
			}
		}

		printf_main(result->argc, result->argv);
		if (g_output) {
			if (has_option(result, 'v')) {
				if (var && (var->flags & VAR_READONLY)) {
					ret = exit_error(E_VAR_READONLY, 1, "printf: ", (char *)name, EE_FREE_NONE, EE_RETURN);
				} else {
					ret = variable_scalar_set(shell.env, name, g_output, 0, VAR_NONE, 0);
					if (ret) {
						if (errno == E_NO_MEMORY)		return (exit_error(E_NO_MEMORY,      1, "printf",   NULL,         EE_FREE_NONE, EE_RETURN));
						if (errno == E_VAR_IDENTIFIER)	return (exit_error(E_VAR_IDENTIFIER, 1, "printf: ", (char *)name, EE_FREE_NONE, EE_RETURN));
						// invalid type...
					}
				}
			} else {
				if (write(STDOUT_FILENO, g_output, ft_strlen(g_output)) == -1) ret = 1;
			}
			free(g_output);
			g_output = NULL;
		}

		return (free_options(result), ret);
	}

#pragma endregion

#pragma region "Tests"

	// Comandos básicos:
	// 
	// printf "Hola mundo\n"
	// printf "Número: %d\n" 42
	// printf "String: %s\n" "texto"
	// printf "Carácter: %c\n" A
	// printf "Hexadecimal: %x\n" 255
	// printf "Octal: %o\n" 64
	// printf "Float: %f\n" 3.14159

	// Ancho, precisión y flags:
	// 
	// printf "%10s\n" "derecha"
	// printf "%-10s\n" "izquierda"
	// printf "%05d\n" 42
	// printf "%.2f\n" 3.14159265
	// printf "%10.2f\n" 3.14159265
	// printf "%+d\n" 42
	// printf "% d\n" 42
	// printf "%#x\n" 255
	// printf "%#o\n" 64

	// Secuencias de escape y %b:
	// 
	// printf "Línea 1\nLínea 2\n"
	// printf "Tab\taqui\n"
	// printf "Comillas: \"%s\"\n" texto
	// printf "Backslash: \\\n"
	// printf "Retorno: antes\rdespués\n"
	// printf "%b\n" "Hola\tmundo\n"
	// printf "%b\n" "Octal:\040ABC\n"
	// printf "%b\n" "Hex:\x41\x42\x43\n"

	// Formatos científicos y generales:
	// 
	// printf "Sci: %e\n" 1234.5
	// printf "Sci: %E\n" 0.0000123
	// printf "Gen: %g\n" 1234.5
	// printf "Gen: %G\n" 0.0000123
	// printf "%.3e\n" 1234.5
	// printf "%.3g\n" 1234.5
	// printf "%#g\n" 10.0

	// Reutilización de formato:
	// 
	// printf "%s\n" uno dos tres cuatro
	// printf "%d %d\n" 1 2 3 4 5
	// printf "%s-%d\n" a 1 b 2 c 3

	// Modificadores de longitud:
	// 
	// printf "%ld\n" 2147483648
	// printf "%lld\n" 9223372036854775807
	// printf "%lu\n" 4294967295
	// printf "%llu\n" 18446744073709551615
	// printf "%llx\n" 18446744073709551615

	// Otros:
	// 
	// printf ""
	// printf "\n"
	// printf "%s"
	// printf "%s" ""
	// printf "%d\n"
	// printf "%d\n" abc
	// printf "%s\n" 123
	// printf "%%\n"
	// printf "%10s %-10s\n" "Col1" "Col2"
	// printf "|%-20s| %5d |\n" "Producto" 100

#pragma endregion
