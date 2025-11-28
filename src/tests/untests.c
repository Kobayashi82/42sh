/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   untests.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:13:41 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/28 22:33:50 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "tests/args.h"
	#include "expansion/globbing.h"
	#include "tests/tests.h"

	#include <stdio.h>

#pragma endregion

#pragma region "UserInfo"

	void test_userinfo() {
		t_userinfo *user = get_userinfo_by_name("root");

		if (user) {
			printf("Usuario: %s\nHome: %s\nShell: %s\nUID: %d\nGID: %d\n", user->username, user->home, user->shell, user->uid, user->gid);
			free_user(user);
		}

		user = get_userinfo_by_id(getuid());

		if (user) {
			printf("Usuario: %s\nHome: %s\nShell: %s\nUID: %d\nGID: %d\n", user->username, user->home, user->shell, user->uid, user->gid);
			free_user(user);
		}

		char *path = get_home_by_name("root");
		if (path) { printf("%s\n", path); free(path); }
		path = get_home_by_id(getuid());
		if (path) { printf("%s\n", path); free(path); }
	}

#pragma endregion
