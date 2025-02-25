/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_alias.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:58:15 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/25 21:46:09 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/input.h"
	#include "hashes/alias.h"
	#include "main/options.h"

#pragma endregion

#pragma region "Expand"

void expand_alias(char **input) {
	if (!options.expand_aliases || input || !*input) return;

	char *value = *input;

	char *new_value = ft_strdup(value);
	sfree(value);
	*input = new_value;
}

#pragma endregion
