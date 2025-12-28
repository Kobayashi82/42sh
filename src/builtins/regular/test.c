/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:12:14 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/29 00:30:38 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/builtin.h"
	#include "builtins/options.h"

#pragma endregion

// [] es un alias de test, por decirlo de alguna manera

// El builtin test evalúa condiciones relacionadas con archivos, cadenas y números.
// Aquí tienes una descripción de los operadores que necesitas implementar:
//
//	Operadores para archivos
//		-b: Verdadero si el archivo es un especial de bloque.
//		-c: Verdadero si el archivo es un especial de carácter.
//		-d: Verdadero si el archivo es un directorio.
//		-e: Verdadero si el archivo existe.
//		-f: Verdadero si el archivo es un archivo regular.
//		-g: Verdadero si el archivo tiene el bit SGID activado.
//		-L: Verdadero si el archivo es un enlace simbólico.
//		-p: Verdadero si el archivo es un FIFO o pipe con nombre.
//		-r: Verdadero si el archivo es legible.
//		-S: Verdadero si el archivo es un socket.
//		-s: Verdadero si el archivo tiene un tamaño mayor que 0.
//		-u: Verdadero si el archivo tiene el bit SUID activado.
//		-w: Verdadero si el archivo es escribible.
//		-x: Verdadero si el archivo es ejecutable.
//
//	Operadores para cadenas
//		-z: Verdadero si la longitud de la cadena es 0.
//		=: Verdadero si dos cadenas son iguales.
//		!=: Verdadero si dos cadenas son diferentes.
//
//	Operadores para números
//		-eq: Verdadero si los números son iguales.
//		-ne: Verdadero si los números son diferentes.
//		-ge: Verdadero si el primer número es mayor o igual al segundo.
//		-lt: Verdadero si el primer número es menor que el segundo.
//		-le: Verdadero si el primer número es menor o igual al segundo.
//	Otros operadores
//		!: Negación de la condición.
//		Sin operador: Un solo operando se evalúa como verdadero si no está vacío.
//
//	Notas importantes
//		Las expresiones tienen que evaluarse de manera estricta con la sintaxis esperada.
//		El builtin debe manejar tanto expresiones simples como compuestas. Ejemplo:
//
//	Ejemplo
//		test -f archivo.txt && echo "Es un archivo regular"
//