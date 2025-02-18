/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 21:50:38 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/25 23:00:16 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	swap_nodes(t_node **top, t_node *prev)
{
	t_node	*first;
	t_node	*second;

	if (top == NULL || *top == NULL || (*top)->next == NULL)
		return ;
	if (prev == NULL)
	{
		first = *top;
		second = first->next;
		first->next = second->next;
		second->next = first;
		*top = second;
	}
	else
	{
		first = prev->next;
		if (first == NULL || first->next == NULL)
			return ;
		second = first->next;
		first->next = second->next;
		second->next = first;
		prev->next = second;
	}
}

void	advance_to_next_node(t_node **top, t_node **current_node,
		t_node **prev_node)
{
	if (*prev_node == NULL)
		*prev_node = *top;
	else
		*prev_node = (*prev_node)->next;
	*current_node = (*prev_node)->next;
}

void	sort_lst(t_node **top)
{
	int		swapped;
	t_node	*current_node;
	t_node	*prev_node;

	if (top == NULL || *top == NULL || (*top)->next == NULL)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current_node = *top;
		prev_node = NULL;
		while (current_node->next != NULL)
		{
			if (ft_strcmp(current_node->content,
					current_node->next->content) > 0)
			{
				swap_nodes(top, prev_node);
				swapped = 1;
			}
			advance_to_next_node(top, &current_node, &prev_node);
		}
	}
}

char	*get_node(t_node **top, const char *var_name)
{
	t_node	*current;
	char	*equal_sign;
	int		size;

	current = *top;
	while (current != NULL)
	{
		equal_sign = ft_strchr(current->content, '=');
		if (equal_sign != NULL)
		{
			size = equal_sign - current->content;
			if (ft_strncmp(current->content, var_name, size) == 0
				&& var_name[size] == '\0')
				return (equal_sign + 1);
		}
		current = current->next;
	}
	return (NULL);
}
