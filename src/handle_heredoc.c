/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:43:38 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/30 13:04:56 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_list	*cmd_befor_heredoc(t_list **top)
{
	t_list	*current;
	t_list	*befor;

	befor = NULL;
	current = *top;
	if (top == NULL || *top == NULL)
		return (0);
	while (current != NULL)
	{
		if (ft_strcmp(current->content, "<<") == 0)
			break ;
		add_token(&befor, current->content, ft_strlen(current->content));
		current = current->next;
	}
	return (befor);
}

t_list	*cmd_after_heredoc(t_list **top)
{
	t_list	*current;
	t_list	*after;

	after = NULL;
	current = *top;
	if (top == NULL || *top == NULL)
		return (NULL);
	while (current != NULL)
	{
		if (ft_strcmp(current->content, "<<") == 0)
		{
			add_token(&after, current->next->content,
				ft_strlen(current->next->content));
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
	current = *top;
	if (top == NULL || *top == NULL)
		return (NULL);
	while (current != NULL)
	{
		last = current;
		current = current->next;
	}
	return (last);
}

int	find_index(char *input)
{
	int	i;

	i = 0;
	while (input[i] != '$' && input[i] != '\0')
		i++;
	if (input[i] == '$')
		return (i);
	else
		return (-1);
}
