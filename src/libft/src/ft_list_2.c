/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:28:18 by vzurera-          #+#    #+#             */
/*   Updated: 2024/01/18 13:28:18 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel_last(t_list **lst, void (*del)(void*))
{
	t_list	*last;
	t_list	*second_last;

	if (lst == NULL || *lst == NULL)
		return ;
	last = *lst;
	second_last = NULL;
	while (last->next)
	{
		second_last = last;
		last = last->next;
	}
	if (second_last)
		second_last->next = NULL;
	else
		*lst = NULL;
	if (del)
		del(last->content);
	free(last);
}

int	ft_lstsize(t_list *lst)
{
	int		len;
	t_list	*current;

	len = 0;
	current = lst;
	while (current)
	{
		current = current->next;
		len++;
	}
	return (len);
}

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*current;

	if (!lst)
		return (lst);
	current = lst;
	while (current->next)
		current = current->next;
	return (current);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*n_lst;
	t_list	*tn_lst;

	if (!lst)
		return (NULL);
	n_lst = ft_lstnew(NULL);
	if (!n_lst)
		return (NULL);
	tn_lst = n_lst;
	while (lst)
	{
		n_lst->content = f(lst->content);
		if (!n_lst->content)
			return (ft_lstclear(&tn_lst, del), NULL);
		if (!lst->next)
			break ;
		lst = lst->next;
		n_lst->next = ft_lstnew(NULL);
		if (!n_lst->next)
			return (ft_lstclear(&tn_lst, del), NULL);
		n_lst = n_lst->next;
	}
	return (tn_lst);
}
