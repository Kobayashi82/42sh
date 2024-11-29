/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_maths.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 17:50:48 by vzurera-          #+#    #+#             */
/*   Updated: 2024/01/10 22:01:17 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_max(int n1, int n2)
{
	if (n1 > n2)
		return (n1);
	return (n2);
}

int	ft_min(int n1, int n2)
{
	if (n1 < n2)
		return (n1);
	return (n2);
}

int	ft_abs(int n)
{
	if (n < 0)
		n = -n;
	return (n);
}

char	*ft_max_ptr(char *s1, char *s2)
{
	if (s1 < s2)
		return (s2);
	return (s1);
}
