/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 21:00:29 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/29 00:30:38 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/builtin.h"
	#include "builtins/options.h"

#pragma endregion

// 42sh (16) + 2 alias
//
// banner
// builtin
// declare, typeset
// dirs
// disown
// enable
// help
// history
// let
// local
// logout
// mapfile, readarray
// popd
// pushd
// shopt
// suspend

// Regular (22) + 1 alias
//
// alias
// bg
// cd
// command
// echo
// false
// fc
// fg
// getopts
// hash
// jobs
// kill
// printf
// pwd
// read
// test, [
// true
// type
// ulimit
// umask
// unalias
// wait

// Special (15) + 1 alias
//
// break
// continue
// ., source
// :
// eval
// exec
// exit
// export
// readonly
// return
// set
// shift
// times
// trap
// unset

//	Total: 53 + 4 alias

// Not Implemented
//
// bind
// caller
// compgen
// complete
// compopt
