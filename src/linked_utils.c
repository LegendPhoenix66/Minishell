/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 18:44:35 by drenquin          #+#    #+#             */
/*   Updated: 2024/11/27 11:26:19 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	remove_node(t_node **top, t_node **current, t_node *previous,
		t_node *to_remove)
{
	to_remove = *current;
	if (previous == NULL)
		*top = (*current)->next;
	else
		previous->next = (*current)->next;
	*current = (*current)->next;
	free(to_remove->content);
	free(to_remove);
}

void	remove_if(t_node **top, const char *var_name)
{
	t_node	*current;
	t_node	*previous;
	t_node	*to_remove;

	current = *top;
	previous = NULL;
	to_remove = NULL;
	if (*top == NULL)
		return ;
	while (current != NULL)
	{
		if (ft_strncmp(current->content, var_name, ft_strchr(var_name, '=')
				- var_name) == 0)
			remove_node(top, &current, previous, to_remove);
		else
		{
			previous = current;
			current = current->next;
		}
	}
}

t_node	*copy_list(t_node *original)
{
	t_node	*new_node;

	if (original == NULL)
		return (NULL);
	new_node = malloc(sizeof(t_node));
	if (new_node == NULL)
		return (NULL);
	new_node->content = ft_strdup(original->content);
	if (new_node->content == NULL)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = copy_list(original->next);
	return (new_node);
}
