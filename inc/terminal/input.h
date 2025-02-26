/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:04:02 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/26 22:23:12 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region Enumerators

		enum e_syntax_type { FULL, PARTIAL };

	#pragma endregion

#pragma region Methods

	//	---------- INPUT -----------
	char	*get_input();

	//	------- CHECK SYNTAX -------
	int		check_syntax(char **input, int type, int partial_mode);

	//	------- EXPAND ALIAS -------
	void	expand_alias(char **input, int partial_mode);

	//	------ EXPAND HISTORY ------
	void	expand_history(char **input, int partial_mode);


	size_t find_arithmetic_expression(const char *str, size_t *index);
	void expand_aliases(char** input);

#pragma endregion
