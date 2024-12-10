/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 20:53:31 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/10 20:54:31 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#pragma region Variables

	#pragma region Enumerators

		enum e_prompt_type { PS1, PS2 };

	#pragma endregion

	#pragma region Structures



	#pragma endregion

	extern char		*prompt_PS1;
	extern char		*prompt_PS2;

#pragma endregion

#pragma region Methods

	//	---------- PROMPT ----------
	void	set_prompt(int type, char *new_prompt);

#pragma endregion
