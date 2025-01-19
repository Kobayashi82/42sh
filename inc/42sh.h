/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   42sh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:18:26 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/19 15:07:58 by vzurera-         ###   ########.fr       */
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

	#include <unistd.h>
	#include <sys/wait.h>
	#include <fcntl.h>
	#include <stdio.h>
	#include <errno.h>
	#include <stdlib.h>
	#include <string.h>

	#include "tests.h"

	#define PROYECTNAME	"42sh"
	#define VERSION		"1.0"
	#define DEVELOPER	"Kobayashi"
	#define PATH		"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"

#pragma endregion

#pragma region Methods

	//	-------- INITIALIZE --------
	int		initialize(int argc, const char **argv, const char **envp);

#pragma endregion
