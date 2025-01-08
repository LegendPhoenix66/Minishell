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
	printf("print my linklist\n");
	while (current != NULL)
	{
		printf("%s\n", current->content);
		current = current->next;
	}
}

void	print_list_debug(t_list **top)
{
	t_list	*current;

	printf("-------list start-------\n");
	current = *top;
	while (current != NULL)
	{
		printf("Content: %s\n", (char *)(current->content));
		printf("adresse %p\n", (char *)(current));
		printf("----------\n");
		current = current->next;
	}
	printf("-------list end-------\n");
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

void	sort_lst(t_node **top)
{
	int		swaped;
	t_node	*current;
	t_node	*prev;
	t_node	*next;

	if (top == NULL || *top == NULL || (*top)->next == NULL)
		return ;
	swaped = 1;
	while (swaped)
	{
		swaped = 0;
		current = *top;
		prev = NULL;
		while (current->next != NULL)
		{
			next = current->next;
			if (strcmp(current->content, next->content) > 0)
			{
				swap_nodes(top, prev);
				swaped = 1;
			}
			if (prev == NULL)
				prev = *top;
			else
				prev = prev->next;
			current = prev->next;
		}
	}
}

// useful when export define a already existing variable
void	remove_if(t_node **top, const char *var_name)
{
	t_node	*current;
	t_node	*previous;
	t_node	*to_remove;

	current = *top;
	previous = NULL;
	if (*top == NULL)
		return ;
	while (current != NULL)
	{
		if (strncmp(current->content, var_name, strchr(var_name, '=')
				- var_name) == 0)
		{
			to_remove = current;
			if (previous == NULL)
				*top = current->next;
			else
				previous->next = current->next;
			current = current->next;
			free(to_remove->content);
			free(to_remove);
		}
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

int	count_node(t_list **top)
{
	int		i;
	t_list	*current;

	i = 0;
	current = *top;
	while (current != NULL)
	{
		current = current->next;
		i++;
	}
	printf("%d\n", i);
	return (i);
}
