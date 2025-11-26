/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_num_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 21:33:44 by vzurera-          #+#    #+#             */
/*   Updated: 2024/03/19 21:33:44 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"

#pragma endregion

	int		ft_min(int n1, int n2) { if (n1 < n2) { return (n1); } return (n2); }
	int		ft_max(int n1, int n2) { if (n1 > n2) { return (n1); } return (n2); }

	char	*ft_itoa(int n) {
		char *str;
		int len = 0, sign = 1, tmp = n;

		if (n == 0) return (ft_strdup("0"));
		if (n < 0) { sign = -1; len++; }
		while (tmp != 0) { tmp /= 10; len++; }
		str = malloc((len + 1));
		if (!str) return (NULL);
		str[len] = '\0';
		while (n != 0) {
			str[--len] = '0' + (sign * (n % 10));
			n /= 10;
		}
		if (sign == -1) str[0] = '-';
		return (str);
	}
