/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 20:27:09 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/07 20:29:32 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// $'...'
//
// \' → comilla simple
// \" → comilla doble
// \\ → backslash
// \a → alert (bell)
// \b → backspace
// \e o \E → escape (ESC)
// \f → form feed
// \n → nueva línea
// \r → retorno de carro
// \t → tabulación horizontal
// \v → tabulación vertical

// Escapes numéricos:

// \nnn → carácter octal (1 a 3 dígitos octales)
// \xHH → carácter hexadecimal (1 o 2 dígitos hex)
// \uHHHH → carácter Unicode (4 dígitos hex)
// \UHHHHHHHH → carácter Unicode (8 dígitos hex)

// Escapes de control:

// \cx → carácter de control (Ctrl+x)

// Todo lo demás NO se expande:

// Variables: $var se imprime literalmente
// Command substitution: $(cmd) se imprime literalmente
// Aritmética: $((2+2)) se imprime literalmente
// Globbing: * se imprime literalmente