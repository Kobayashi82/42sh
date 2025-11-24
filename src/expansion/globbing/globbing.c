/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 11:03:39 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/24 19:38:50 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "parser/args.h"
	#include "expansion/globbing.h"
	#include "main/options.h"
	#include "utils/paths.h"

	#include <sys/stat.h>

#pragma endregion

#pragma region "Search"

	static t_arg *search(t_pattern *patterns, char *basedir, char *dir) {
		if (!patterns) return (NULL);
		t_arg *files = NULL, *final = NULL;

		if (patterns->next && patterns->is_dir) {
			files = match_dir(patterns, basedir, dir);
			t_arg *tmp = files;
			while (tmp) {
				t_arg *tmp_files = search(patterns->next, basedir, tmp->value);
				if (tmp_files) args_merge(&final, &tmp_files);
				tmp = tmp->next;
			} args_clear(&files);
		} else files = match_dir(patterns, basedir, dir);

		args_merge(&files, &final);
		return (files);
	}

#pragma endregion

#pragma region "Match"

	//	Check if there is a match for every file and directory using cpattern
	static t_arg *match(char *value) {
		if (!value || !*value) return (NULL);

		t_pattern *patterns = pattern_create(value);
		if (!patterns) return (NULL);

		char *basedir = NULL;
		char *dir = NULL;
		if (value[0] != '/') {
			char cwd[4096];
			if (!getcwd(cwd, sizeof(cwd))) return (pattern_clear(&patterns), NULL);
			if (!strcmp(cwd, "/"))	basedir = ft_strdup(cwd);
			else						basedir = ft_strjoin(cwd, "/", 0);
		} else							dir = ft_strdup("/");

		t_arg *files = search(patterns, basedir, dir);
		free(basedir); free(dir);
		pattern_clear(&patterns);

		return (args_sort(files));
	}

#pragma endregion

#pragma region "Globbing"

	//	Search for wildcards matches in every arg and join them in a single list
	void globbing(t_arg *args) {
		if (args == NULL || options.noglob) return;

		globbing(args->next);
		if (memchr(args->value, '?', ft_strlen(args->value))
			|| memchr(args->value, '*', ft_strlen(args->value))
			|| memchr(args->value, '[', ft_strlen(args->value)))
		{
			t_arg *files = match(args->value);
			if (!files) {
				if (options.nullglob) {
					free(args->value);
					args->value = NULL;
					args->nullglob = true;
				}
				if (options.failglob) args->failglob = true;
			} else args_insert(args, &files);
		}
	}

#pragma endregion

#pragma region Info

	// Basic Globbing			Description												Example						Matches
	// ——————————————			———————————												———————						———————
	// *						Any sequence of characters, or none						*.txt						file.txt, document.txt, etc.
	// ?						Exactly one character									file?.txt					file1.txt, fileA.txt, etc.
	// [abc]					One of the characters inside the brackets				file[abc].txt				filea.txt, fileb.txt, filec.txt
	// [a-z]					Any character within a range							file[a-z].txt				filea.txt, fileb.txt, ... filez.txt
	// [!abc] o [^abc]			Any character not in the list							[!abc].txt o [^abc].txt		filed.txt, file@.txt, etc.
	// [!a-z] o [^a-z]			Any character that is not a lowercase letter			[!a-z].txt o [^a-z].txt		file0.txt, file@.txt, etc.
	// {a,b,c}					Any of the options separated by commas					file{1,2,3}.txt				file1.txt, file2.txt, file3.txt
	// {a..z}					A range of characters (letters or numbers)				file{a..z}.txt				filea.txt, fileb.txt, ... filez.txt

	// When you use a pattern ending with /, the shell interprets it as an attempt to match directories

	// Globbing options (shopt):
	//
	// extglob		Enables extended globbing
	// nocaseglob	Enables case-insensitive globbing
	// dotglob		Includes hidden files in globbing
	// failglob		Throws an error if no matches are found
	// nullglob		Expands empty patterns to nothing if no matches are found

	// Globbing options (set):
	//
	// noglob		Disables globbing

#pragma endregion
