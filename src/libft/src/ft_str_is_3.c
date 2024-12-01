/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_is_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 22:49:51 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/02 22:52:52 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_ispunct(const char c)
{
	int		i;
	char	*punct;

	i = 0;
	punct = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
	while (punct[i] != '\0')
		if (c == punct[i++])
			return (1);
	return (0);
}
