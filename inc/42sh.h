/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   42sh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:18:26 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/21 17:43:22 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#include "libft.h"
	#include "colors.h"
	#include "clean.h"
	#include "options.h"
	#include "terminal.h"
	#include "readinput.h"
	#include "history.h"
	#include "prompt.h"
	#include "shell.h"
	#include "variables.h"
	#include "parser.h"
	#include "builtin.h"
	#include "utils.h"

	#include <sys/wait.h>
	#include <fcntl.h>
	#include <stdio.h>
	#include <unistd.h>
	#include <errno.h>
	#include <stdlib.h>
	#include <string.h>
	#include <limits.h>

	#include "tests.h"

	#define PROYECTNAME	"42sh"
	#define VERSION		"1.0"
	#define DEVELOPER	"Kobayashi"

#pragma endregion

#pragma region Methods

	//	-------- INITIALIZE --------
	int		initialize(int argc, const char **argv, const char **envp);

	//	---------- VARIOS ----------
	char	*home_path(const char *user);

#pragma endregion
