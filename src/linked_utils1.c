/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 21:24:32 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/21 10:30:42 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_node	*make_node(const char *env)
{
	t_node	*new_node;

	new_node = (t_node *)malloc(sizeof(t_node));
	if (new_node == NULL)
	{
		perror("malloc error");
		return (NULL);
	}
	new_node->content = ft_strdup(env);
	if (new_node->content == NULL)
	{
		perror("ft_strdup error");
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

void	add_node(t_node **top, const char *env)
{
	t_node	*new_node;
	t_node	*current;

	new_node = make_node(env);
	if (new_node == NULL)
		return ;
	if (*top == NULL)
		*top = new_node;
	else
	{
		current = *top;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
		new_node->next = NULL;
	}
}

t_node	*init_lst(void)
{
	t_node	*lst;
	size_t	i;

	lst = NULL;
	i = 0;
	if (__environ == NULL)
	{
		perror("env error");
		return (NULL);
	}
	while (__environ[i] != NULL)
	{
		add_node(&lst, __environ[i]);
		i++;
	}
	return (lst);
}

void	print_lst(t_node **top)
{
	t_node	*current;

	current = *top;
	while (current != NULL)
	{
		printf("%s\n", current->content);
		current = current->next;
	}
}

void	free_lst(t_node *top)
{
	t_node	*tmp;

	while (top != NULL)
	{
		tmp = top;
		top = top->next;
		free(tmp->content);
		free(tmp);
	}
}
