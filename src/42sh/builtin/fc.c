/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 21:00:36 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/05 21:11:55 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

const char *resolve_symlink(const char *path) {
    static char	resolved_path[PATH_MAX];
    char 		temp_path[PATH_MAX];
    ssize_t		len;

	ft_strcpy(temp_path, path);
	while ((len = readlink(temp_path, resolved_path, sizeof(resolved_path) - 1)) != -1) {
        resolved_path[len] = '\0';
        ft_strcpy(temp_path, resolved_path);
    }

    return (resolved_path);
}

const char *default_editor() {
	const char	*editor = getenv("FCEDIT");
	if (!editor || !*editor) editor = getenv("EDITOR");
	if (!editor || !*editor) editor = getenv("VISUAL");
	if (!editor || !*editor) editor = resolve_symlink("/usr/bin/editor");
	if (!editor || !*editor) editor = "nano";
    return (editor);
}

// fc [-e ename] [-lnr] [first] [last] or fc -s [pat=rep] [command]
// Display or execute commands from the history list.
// FIRST and LAST can be numbers specifying the range, or FIRST can be a string, which means the most recent command beginning with that string.

// -e ENAME  select which editor to use.  Default is FCEDIT, then EDITOR, then vi
// -l        list lines instead of editing
// -n        omit line numbers when listing
// -r        reverse the order of the lines (newest listed first)
