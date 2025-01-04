/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 13:25:21 by lhopp             #+#    #+#             */
/*   Updated: 2024/02/27 13:25:22 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*next;

	if (lst)
	{
		while (*lst)
		{
			next = (*lst)->next;
			ft_lstdelone(*lst, del);
			*lst = next;
		}
	}
}*/
void ft_lstclear(t_list **lst, void (*del)(void*))
{
    t_list *current;
    t_list *next;

    if (!lst || !del)
        return;
    
    current = *lst;
    while (current)
    {
        next = current->next;    // Sauvegarde le next avant de free
        ft_lstdelone(current, del);
        current = next;
    }
    *lst = NULL;    // Important: met le pointeur à NULL
}