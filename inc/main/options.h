/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:51:51 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/28 23:12:10 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region "Enumerators"

		enum e_option {
			SHOPT,
			SET
		};

	#pragma endregion

	#pragma region "Structures"

		typedef struct {

			//	READINPUT
			int	emacs;				//	1 = Emacs mode ON													0 = OFF
			int	vi;					//	1 = Vi mode ON														0 = OFF
			int	hide_ctrl_chars ;	//	1 = Show control chars (^C)											0 = OFF
			int	multiwidth_chars;	//	1 = Enable multi-width chars (there are some issues)				0 = OFF

			//	HISTORY
			int	history;			//	1 = History enabled													0 = Disabled
			int	hist_local;			//	1 = Save history to disk											0 = Do not save
			int	histexpand;			//	1 = Expand history													0 = Disabled

			//	ALIAS
			int	expand_aliases;		//	1 = Expand alias													0 = Disabled

			//	GLOBBING
			int	noglob;				//	1 = Globbing disabled												0 = Enabled
			int	dotglob;			//	1 = Include hidden files (starting with .)							0 = Exclude
			int	nullglob;			//	1 = Patterns that do not match expand to an empty string			0 = Keep pattern
			int	failglob;			//	1 = Patterns that do not match cause an error						0 = No error
			int	nocaseglob;			//	1 = Case-insensitive globbing										0 = Case-sensitive

			//	CD
			int	cd_resolve;			//	1 = Resolve symbolic links as default (set -P)						0 = Disbaled (set +P)
			int	cdable_vars;		//	1 = Variables can be used as directories for 'cd'					0 = Disabled
			int	autocd;				//	1 = Automatically change to a directory without 'cd'				0 = Disabled
			int	cdspell;			//	1 = Automatically correct minor errors in directory names			0 = Disabled
			int	dirspell;			//	1 = Correct minor errors in the first segment of an absolute path	0 = Disabled

		} t_options;

	#pragma endregion

	extern t_options options;

#pragma endregion

#pragma region "Methods"

	int		options_initialize();
	int		options_set(const char *option, int value);
	char	*options_print(char *value, int type);

#pragma endregion
