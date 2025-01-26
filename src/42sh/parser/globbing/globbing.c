/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 11:03:39 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/26 14:48:42 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region Includes

	#include "globbing.h"

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

	// Extended Globbing		Description												Example						Matches
	// —————————————————		———————————												———————						———————
	// *(pattern)				Zero or more occurrences of the pattern					file*(a|b).txt				file.txt, filea.txt, fileb.txt, fileab.txt, filebb.txt
	// ?(pattern)				Zero or one occurrence of the pattern					file?(a|b).txt				file.txt, filea.txt, fileb.txt
	// +(pattern)				One or more occurrences of the pattern					file+(a|b).txt				filea.txt, fileb.txt, fileab.txt, filebb.txt
	// @(pattern)				Exactly one occurrence of the pattern					file@(a|b).txt				filea.txt, fileb.txt
	// !(pattern)				Everything except the pattern							file!(a|b).txt				file.txt, filec.txt, file123.txt

	// **/: Matches recursively with all directories and subdirectories
	// When you use a pattern ending with /, the shell interprets it as an attempt to match directories

	// Globbing options (shopt):
	//
	// globstar		Enables recursion with **
	// extglob		Enables extended globbing
	// nocaseglob	Enables case-insensitive globbing
	// dotglob		Includes hidden files in globbing
	// failglob		Throws an error if no matches are found
	// nullglob		Expands empty patterns to nothing if no matches are found

	// Globbing options (set):
	//
	// noglob		Disables globbing

#pragma endregion
