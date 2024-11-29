/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 11:27:58 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/08 20:31:20 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_all(char **result, int i)
{
	if (result)
	{
		while (i-- > 0)
			free(result[i]);
		free(result);
	}
	return (NULL);
}

static int	count_words(const char *str, char c)
{
	int		i;
	int		trigger;
	char	*tmp;

	i = 0;
	trigger = 0;
	tmp = (char *) str;
	while (*str)
	{
		if ((*str != c || (*str == c && str != tmp && *(str - 1) == c))
			&& trigger == 0)
		{
			trigger = 1;
			i++;
		}
		else if (*str == c)
			trigger = 0;
		str++;
	}
	return (i);
}

static char	*word_dup(const char *str, int start, int finish, char c)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc((finish - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	if (finish - start == 1 && str[start] == c)
	{
		free(word);
		return (ft_strdup(""));
	}
	while (start < finish)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	int		index;
	char	**split;

	split = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!s || !split)
		return (free_all(split, 0));
	i = -1;
	j = 0;
	index = -1;
	while (i + 1 <= ft_strlen(s))
	{
		if ((s[++i] != c || (s[i] == c && i > 0 && s[i - 1] == c)) && index < 0)
			index = i;
		else if ((s[i] == c || i == ft_strlen(s)) && index >= 0)
		{
			split[j++] = word_dup(s, index, i, c);
			if (split[j - 1] == NULL)
				return (free_all(split, j));
			index = -1;
		}
	}
	split[j] = 0;
	return (split);
}
