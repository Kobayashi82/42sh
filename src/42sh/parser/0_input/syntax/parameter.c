/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:44:28 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/05 14:01:44 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "parser/syntax.h"

#pragma endregion

#pragma region "Parameter"

	int syntax_parameter(const char *input, size_t *i, t_context *context, char *last_token, int *line) {
		if (!input || !*input || !context) return (0);

		(void) i;
		(void) last_token;
		(void) line;

		return (0);
	}

#pragma endregion

#pragma region "Info"

	//	En parameter expansion:

	//	Primera parte literal
	//	Pperado literal
	//	Valor: $(()), $(), `, ${}, globbing

	//	┌──────────────────────────┬──────────────────────────┬─────────────────────────────┬───────────────────────────────────────────────┐
	//	│         Concepto         │         Sintaxis         │           Ejemplo           │                  Descripción                  │
	//	├──────────────────────────┼──────────────────────────┼─────────────────────────────┼───────────────────────────────────────────────┤
	//	│ Acceso básico            │ ${variable}              │ echo "${user}"              │ Obtiene el valor de la variable.              │
	//	│ Valor predeterminado     │ ${var:-valor}            │ echo "${name:-Anónimo}"     │ Usa valor si var no está definida o vacía.    │
	//	│ Asignación predeter.     │ ${var:=valor}            │ echo "${count:=0}"          │ Asigna valor a var si no existe.              │
	//	│ Error si no definida     │ ${var:?mensaje}          │ echo "${file:?Error}"       │ Muestra mensaje y termina si var no existe.   │
	//	│ Valor alternativo        │ ${var:+valor}            │ echo "${debug:+ON}"         │ Usa valor solo si var está definida.          │
	//	│ Longitud de cadena       │ ${#var}                  │ echo "${#str}"              │ Devuelve la cantidad de caracteres en var.    │
	//	│ Subcadena (offset, len)  │ ${var:inicio:longitud}   │ echo "${str:2:4}"           │ Extrae longitud caracteres desde inicio.      │
	//	│ Subcadena (solo offset)  │ ${var:inicio}            │ echo "${str:5}"             │ Extrae desde inicio hasta el final.           │
	//	│ Eliminar prefijo (corto) │ ${var#patrón}            │ echo "${path#*/}"           │ Elimina el prefijo más corto.                 │
	//	│ Eliminar prefijo (largo) │ ${var##patrón}           │ echo "${path##*/}"          │ Elimina el prefijo más largo.                 │
	//	│ Eliminar sufijo (corto)  │ ${var%patrón}            │ echo "${file%.*}"           │ Elimina el sufijo más corto.                  │
	//	│ Eliminar sufijo (largo)  │ ${var%%patrón}           │ echo "${file%%.*}"          │ Elimina el sufijo más largo.                  │
	//	│ Reemplazo (primero)      │ ${var/antiguo/nuevo}     │ echo "${text/foo/bar}"      │ Reemplaza la primera ocurrencia.              │
	//	│ Reemplazo (todos)        │ ${var//antiguo/nuevo}    │ echo "${text//foo/bar}"     │ Reemplaza todas las ocurrencias.              │
	//	│ Mayúsculas               │ ${var^^}                 │ echo "${palabra^^}"         │ Convierte toda la cadena a mayúsculas.        │
	//	│ Minúsculas               │ ${var,,}                 │ echo "${PALABRA,,}"         │ Convierte toda la cadena a minúsculas.        │
	//	│ Primera mayúscula        │ ${var^}                  │ echo "${palabra^}"          │ Convierte solo la primera letra.              │
	//	│ Conversión selectiva     │ ${var^^patrón}           │ echo "${str,,[A-Z]}"        │ Convierte solo ciertos caracteres.            │
	//	│ Expansión indirecta      │ ${!var}                  │ var=a; a=b; echo ${!var}    │ Obtiene el valor de otra variable.            │
	//	│ Arreglo: elemento        │ ${array[indice]}         │ echo "${frutas[0]}"         │ Obtiene un elemento del arreglo.              │
	//	│ Arreglo: todos elementos │ ${array[@]}              │ echo "${frutas[@]}"         │ Lista todos los elementos del arreglo.        │
	//	│ Arreglo: longitud        │ ${#array[@]}             │ echo "${#frutas[@]}"        │ Devuelve el número de elementos.              │
	//	│ Arreglo: subarreglo      │ ${array[@]:inicio:long}  │ echo "${frutas[@]:1:2}"     │ Extrae un subarreglo.                         │
	//	│ Arreglo: índices         │ ${!array[@]}             │ echo "${!frutas[@]}"        │ Lista los índices del arreglo.                │
	//	│ Variables con prefijo    │ ${!prefijo*}             │ echo "${!var*}"             │ Lista variables que inician con prefijo.      │
	//	│ Número de argumentos     │ $#                       │ echo "Args: $#"             │ Muestra la cantidad de argumentos.            │
	//	│ Todos los argumentos     │ $@                       │ echo "$@"                   │ Lista los argumentos como elementos.          │
	//	│ Todos como cadena        │ $*                       │ echo "$*"                   │ Une los argumentos en una cadena.             │
	//	│ Código de salida         │ $? o ${?}                │ echo "Código: $?"           │ Código de salida del último comando.          │
	//	│ Expansión segura         │ ${var@Q}                 │ str="hola"; echo "${str@Q}" │ Expande con escapado seguro.                  │
	//	└──────────────────────────┴──────────────────────────┴─────────────────────────────┴───────────────────────────────────────────────┘

	//	Operators Begining:	!	#
	//	Operators Middle:	:-	:=	:?	:+	:	#	##	%	%%	/	//	,,	^	^^	...

	//	Las operaciones de eliminación de prefijo/sufijo usan sintaxis de globbing, por lo que aceptan comodines como *, ?, [a-z], [!a-z], etc.

	//	Obligados
	//
	//	${parameter:-word}
	//	${parameter:=word}
	//	${parameter:?word}
	//	${parameter:+word}
	//	${#parameter}
	//	${parameter%}
	//	${parameter%%}
	//	${parameter#}
	//	${parameter##}

#pragma endregion
