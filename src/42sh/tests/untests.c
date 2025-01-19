/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   untests.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:13:41 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/19 20:51:54 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "tests.h"

#include <stdio.h>

#pragma region UserInfo

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
		if (path) { printf("%s\n", path); sfree(path); }
		path = get_home_by_id(getuid());
		if (path) { printf("%s\n", path); sfree(path); }
	}

#pragma endregion
