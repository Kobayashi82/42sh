/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:27:10 by vzurera-          #+#    #+#             */
/*   Updated: 2024/01/18 13:27:10 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	new->next = lst[0];
	lst[0] = new;
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (!lst[0])
		lst[0] = new;
	else
		ft_lstlast(lst[0])->next = new;
}

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (lst && lst->content)
		del (lst->content);
	free(lst);
}

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*current;

	if (!lst || !*lst)
		return ;
	current = *lst;
	*lst = (*lst)->next;
	if (current)
	{
		ft_lstclear(lst, del);
		del(current->content);
	}
	free (current);
}
