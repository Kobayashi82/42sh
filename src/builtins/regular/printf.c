/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/21 19:23:47 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Variables"

	#define BUFFER_SIZE 8192

	static char		g_output_buffer[BUFFER_SIZE];
	static size_t	g_buffer_pos = 0;

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

			static void	buffer_char(char c) {
				if (g_buffer_pos >= BUFFER_SIZE - 1) {
					write(STDOUT_FILENO, g_output_buffer, g_buffer_pos);
					g_buffer_pos = 0;
				}
				g_output_buffer[g_buffer_pos++] = c;
			}

		#pragma endregion

		#pragma region "String"

			static void	buffer_string(const char *str, size_t len) {
				size_t	i;

				i = 0;
				while (i < len) {
					if (g_buffer_pos >= BUFFER_SIZE - 1) {
						write(STDOUT_FILENO, g_output_buffer, g_buffer_pos);
						g_buffer_pos = 0;
					}
					g_output_buffer[g_buffer_pos++] = str[i++];
				}
			}

		#pragma endregion

		#pragma region "Flash"

			static void	flush_buffer() {
				if (g_buffer_pos > 0) {
					write(STDOUT_FILENO, g_output_buffer, g_buffer_pos);
					g_buffer_pos = 0;
				}
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Parse"

		#pragma region "Flags"

			static void	parse_flags(const char **fmt, int *flags) {
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

			static int	parse_width(const char **fmt) {
				int	width = 0;

				while (isdigit(**fmt)) {
					width = width * 10 + (**fmt - '0');
					(*fmt)++;
				}

				return (width);
			}

		#pragma endregion

		#pragma region "Precision"

			static int	parse_precision(const char **fmt) {
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

	#pragma endregion

	#pragma region "Print"

		#pragma region "Num to Str"

			static int	num_to_str(unsigned long num, char *buffer, int base, int uppercase) {
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

			static void	print_padding(int count, char pad_char) {
				int i = 0;

				while (i < count) {
					buffer_char(pad_char);
					i++;
				}
			}

		#pragma endregion

		#pragma region "Int"

			static void	print_int(long num, int width, int precision, int flags) {
				char	buffer[64];
				int		len;
				char	sign;
				int		pad_char;
				int		is_negative;
				int		total_len;

				is_negative = (num < 0);
				if (is_negative)					num = -num;
				if (num == 0 && precision == 0)		len = 0;
				else								len = num_to_str((unsigned long)num, buffer, 10, 0);

				while (len < precision) {
					memmove(buffer + 1, buffer, len + 1);
					buffer[0] = '0';
					len++;
				}

				sign = 0;
				if		(is_negative)				sign = '-';
				else if (flags & 2)					sign = '+';
				else if (flags & 4)					sign = ' ';

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

			static void	print_unsigned(unsigned long num, int base, int width, int precision, int flags, int uppercase) {
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

			static void	print_float(double num, int width, int precision, int flags) {
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

				int_part = (long)num;
				frac_part = num - int_part;
				mult = 1;
				for (i = 0; i < precision; i++)
					mult *= 10;
				frac = (unsigned long long)(frac_part * (double)mult + 0.5);
				if (frac >= mult) {
					int_part++;
					frac = 0;
				}

				len = num_to_str((unsigned long)int_part, buffer, 10, 0);
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
				if		(is_negative)				sign = '-';
				else if (flags & 2)					sign = '+';
				else if (flags & 4)					sign = ' ';

				total_len = len + (sign ? 1 : 0);
				pad_char = ((flags & 16) && !(flags & 1)) ? '0' : ' ';

				if (!(flags & 1) && pad_char == ' ')	print_padding(width - total_len, ' ');
				if (sign)								buffer_char(sign);
				if (!(flags & 1) && pad_char == '0')	print_padding(width - total_len, '0');
				buffer_string(buffer, len);
				if (flags & 1)							print_padding(width - total_len, ' ');
			}

		#pragma endregion

		#pragma region "String"

			static void	print_string(const char *str, int width, int precision, int flags) {
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

			static char	*process_escapes(const char *str) {
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
								if (*src >= '0' && *src <= '9')
									val = val * 16 + (*src - '0');
								else if (*src >= 'a' && *src <= 'f')
									val = val * 16 + (*src - 'a' + 10);
								else if (*src >= 'A' && *src <= 'F')
									val = val * 16 + (*src - 'A' + 10);
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

			static int	process_specifier(const char **fmt, char **args, int *arg_idx, int arg_count) {
				int			flags;
				int			width;
				int			precision;
				char		spec;
				const char	*arg;
				char		*endptr;
				long		num;

				(*fmt)++;
				if (**fmt == '%') {
					buffer_char('%');
					(*fmt)++;
					return (0);
				}

				parse_flags(fmt, &flags);
				width = parse_width(fmt);
				precision = parse_precision(fmt);
				spec = **fmt;
				if (!spec) return (1);
				arg = (*arg_idx < arg_count) ? args[*arg_idx] : "";

				if (spec == 'd' || spec == 'i') {
					errno = 0;
					num = strtol(arg, &endptr, 10);
					if (*arg && (*endptr != '\0' || errno == ERANGE)) {
						char err_msg[256];
						snprintf(err_msg, sizeof(err_msg), "%s: printf: %s: invalid number\n", shell.name, arg);
						write(2, err_msg, ft_strlen(err_msg));
						num = 0;
					}
					print_int(num, width, precision, flags);
					(*arg_idx)++;
				}
				else if (spec == 'u')
				{
					print_unsigned(strtoul(arg, NULL, 10), 10, width, precision, flags, 0);
					(*arg_idx)++;
				}
				else if (spec == 'o')
				{
					print_unsigned(strtoul(arg, NULL, 10), 8, width, precision, flags, 0);
					(*arg_idx)++;
				}
				else if (spec == 'x')
				{
					print_unsigned(strtoul(arg, NULL, 10), 16, width, precision, flags, 0);
					(*arg_idx)++;
				}
				else if (spec == 'X')
				{
					print_unsigned(strtoul(arg, NULL, 10), 16, width, precision, flags, 1);
					(*arg_idx)++;
				}
				else if (spec == 'f' || spec == 'F')
				{
					print_float(atof(arg), width, precision, flags);
					(*arg_idx)++;
				}
				else if (spec == 'e' || spec == 'E' || spec == 'g' || spec == 'G')
				{
					print_float(atof(arg), width, precision, flags);
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
					char *processed = process_escapes(arg);
					if (!processed) return (1);
					print_string(processed, width, precision, flags);
					free(processed);
					(*arg_idx)++;
				}
				else
				{
					buffer_char('%');
					buffer_char(spec);
					return (1);
				}
				(*fmt)++;

				return (0);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Main"

		static int	printf_main(int argc, char **argv) {
			char		*format;
			char		*processed_fmt;
			const char	*ptr;
			int			arg_idx = 1;
			int			ret = 0;
			int			has_format_specs;

			g_buffer_pos = 0;
			if (!argc) return (1);
			format = argv[0];

			processed_fmt = process_escapes(format);
			if (!processed_fmt) return (1);

			do {
				ptr = processed_fmt;
				has_format_specs = 0;

				while (*ptr) {
					if (*ptr == '%') {
						has_format_specs = 1;
						if (process_specifier(&ptr, argv, &arg_idx, argc) != 0) {
							ret = 1;
							break;
						}
					} else {
						buffer_char(*ptr);
						ptr++;
					}
				}
			} while (has_format_specs && arg_idx < argc && !ret);

			free(processed_fmt);
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

		t_parse_result *result = parse_options(argc, argv, "", NULL, long_opts, "printf [-v var] format [arguments]", IGNORE_OFF);
		if (!result) return (free_options(result), (errno == E_OPT_MAX || errno == E_OPT_INVALID) ? 2 : 1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_printf_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		if (!result->argc) {
			print(STDERR_FILENO, ft_strjoin(shell.name, ": usage: printf [-v var] format [arguments]\n", J_FREE_NONE), P_FREE_RESET_PRINT);
			return (free_options(result), 1);
		}

		ret = printf_main(result->argc, result->argv);

		return (free_options(result), ret);
	}

#pragma endregion

#pragma region "Tests"

	// Comandos básicos de formato:
	// 
	// printf "Hola mundo\n"
	// printf "Número: %d\n" 42
	// printf "String: %s\n" "texto"
	// printf "Carácter: %c\n" A
	// printf "Hexadecimal: %x\n" 255
	// printf "Octal: %o\n" 64
	// printf "Float: %f\n" 3.14159

	// Comandos con múltiples argumentos:
	// 
	// printf "%s %s %s\n" uno dos tres
	// printf "%d + %d = %d\n" 5 3 8
	// printf "%s tiene %d años\n" "María" 25

	// Comandos con modificadores de ancho y precisión:
	// 
	// printf "%10s\n" "derecha"
	// printf "%-10s\n" "izquierda"
	// printf "%05d\n" 42
	// printf "%.2f\n" 3.14159265
	// printf "%10.2f\n" 3.14159265

	// Comandos con secuencias de escape:
	// 
	// printf "Línea 1\nLínea 2\n"
	// printf "Tab\taqui\n"
	// printf "Comillas: \"%s\"\n" texto
	// printf "Backslash: \\\n"
	// printf "Retorno: antes\rdespués\n"

	// Comandos con múltiples especificadores (reutilización de formato):
	// 
	// printf "%s\n" uno dos tres cuatro
	// printf "%d %d\n" 1 2 3 4 5
	// printf "%s-%d\n" a 1 b 2 c 3

	// Comandos con formato especial:
	// 
	// printf "%b\n" "Hola\tmundo\n"
	// printf "%%\n"
	// printf "%10s %-10s\n" "Col1" "Col2"
	// printf "|%-20s| %5d |\n" "Producto" 100

	// Comandos edge case (casos límite):
	// 
	// printf ""
	// printf "\n"
	// printf "%s"
	// printf "%s" ""
	// printf "%d\n"
	// printf "%d\n" abc
	// printf "%s\n" 123

	// Comandos con flags adicionales:
	// 
	// printf "%+d\n" 42
	// printf "% d\n" 42
	// printf "%#x\n" 255
	// printf "%#o\n" 64

#pragma endregion
