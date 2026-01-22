/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:51:51 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/22 13:42:47 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region "Enumerators"

		enum e_option {
			O_SET,					// 
			O_SHOPT,				// 
			O_BOTH					// 
		};

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_options {

			//	READINPUT
			int	emacs;				//	SET			1 = Emacs mode ON													0 = OFF
			int	vi;					//	SET			1 = Vi mode ON														0 = OFF
			int	hide_ctrl_chars;	//	SHOPT		1 = Show control chars (^C)											0 = OFF
			int	multiwidth_chars;	//	SHOPT		1 = Enable multi-width chars (there are some issues)				0 = OFF

			//	HISTORY
			int	history;			//	SET			1 = History enabled													0 = Disabled
			int	histexpand;			//	SET			1 = Expand history (set -H)											0 = Disabled
			int	histappend;			//	SHOPT		1 = Append history to file											0 = Overwrite
			int	histreedit;			//	SHOPT		1 = Edit failed history expansion									0 = Disabled
			int	histverify;			//	SHOPT		1 = Show expansion and wait for confirmation						0 = Execute

			//	ALIAS
			int	expand_aliases;		//	SHOPT		1 = Expand alias													0 = Disabled

			//	GLOBBING
			int	noglob;				//	SET			1 = Globbing disabled (set -f)										0 = Enabled
			int	dotglob;			//	SHOPT		1 = Include hidden files (starting with .)							0 = Exclude
			int	nullglob;			//	SHOPT		1 = Patterns that do not match expand to an empty string			0 = Keep pattern
			int	failglob;			//	SHOPT		1 = Patterns that do not match cause an error						0 = No error
			int	nocaseglob;			//	SHOPT		1 = Case-insensitive globbing										0 = Case-sensitive

			//	CD
			int	physical;			//	SET			1 = Physical paths (set -P)											0 = Logical
			int	cdable_vars;		//	SHOPT		1 = Variables can be used as directories for 'cd'					0 = Disabled
			int	autocd;				//	SHOPT		1 = Automatically change to a directory without 'cd'				0 = Disabled
			int	cdspell;			//	SHOPT		1 = Automatically correct minor errors in directory names			0 = Disabled
			int	dirspell;			//	SHOPT		1 = Correct minor errors in the first segment of an absolute path	0 = Disabled

		} t_options;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	int		option_set(const char *name, int value, int type);
	int		option_set_char(char c, int value);
	int		option_get(const char *name);
	int		option_get_char(char c);
	int		options_print(int type, int reusable, int suppress);
	int		option_print(const char *name, int type, int reusable, int suppress);
	int		options_initialize();

#pragma endregion
