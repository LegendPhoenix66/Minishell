/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:43:38 by drenquin          #+#    #+#             */
/*   Updated: 2025/02/07 20:20:47 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_list	*cmd_after_heredoc(t_list **top)
{
	t_list	*current;
	t_list	*after;

	after = NULL;
	if (top == NULL || *top == NULL)
		return (NULL);
	current = *top;
	while (current != NULL)
	{
		if (ft_strcmp(current->content, "<<") == 0)
		{
			add_token(&after, current->next->content,
				(int)ft_strlen(current->next->content));
			break ;
		}
		current = current->next;
	}
	return (after);
}

t_list	*last_token(t_list **top)
{
	t_list	*current;
	t_list	*last;

	last = NULL;
	if (top == NULL || *top == NULL)
		return (NULL);
	current = *top;
	while (current != NULL)
	{
		last = current;
		current = current->next;
	}
	return (last);
}
