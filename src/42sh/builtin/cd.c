/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:18 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/19 18:28:35 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins/builtins.h"

// /home/pollon/
// ├── real_dir/
// │       ├── real_subdir/
// ├── symlink -> /home/pollon/real_dir/real_subdir

// Inicialmente:
// - symlink apunta a /home/pollon/real_dir/real_subdir.

// Caso 1: cd symlink
//   pwd: /home/pollon/symlink

// Caso 2: cd symlink; cd -L ..    (o cd .. sin el -L)
//   pwd: /home/pollon
//   (Usa el directorio lógico: trata symlink como parte del árbol lógico)

// Caso 3: cd symlink; cd -P ..
//   pwd: /home/pollon/real_dir
//   (Usa el directorio físico real: ignora el enlace simbólico y sube desde real_subdir)
//	Si la opción [-e] se establece y el directorio del nivel inferior no se puede obtener, sale con un codigo diferente a 0 (1 probablemente)

// - Cambia al directorio anterior ($OLDPWD)

//	La variable CDPATH es como PATH, pero se usa para buscar rutas relativas desde esas ubicaciones en orden.
//	Si no se encuentra una ruta válida y la opción 'cdable_vars' está activada, se tratará el argumento como una variable
//	y si existe, se usará como ruta a seguir (siguiendo el mismo proceso de CDPATH y tal...)

//	En CDPATH como en PATH si se pone un ':' al principio, al final o dos seguidos entremedio se considera la ruta actual.
//	Esto también es válido con '.' como ruta.

//	Exit Status: returns 0 if the directory is changed, and if $PWD is set successfully when -P is used; non-zero otherwise.
