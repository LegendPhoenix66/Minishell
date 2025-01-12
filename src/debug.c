/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:20:13 by drenquin          #+#    #+#             */
/*   Updated: 2024/11/28 17:20:13 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Fonction de debug pour un seul nœud
void	debug_node(t_node *node)
{
	if (!node)
	{
		printf("Node is NULL.\n");
		return ;
	}
	printf("Node content: %s\n", node->content ? node->content : "(null)");
	printf("is_cmd: %d\n", node->is_cmd);
	if (node->in_single)
	{
		printf("Quotes: Single quotes ('...')\n");
	}
	else if (node->in_double)
	{
		printf("Quotes: Double quotes (\"...\")\n");
	}
	else if (node->no_quotes)
	{
		printf("Quotes: No quotes\n");
	}
	else
	{
		printf("Quotes: Undefined or mixed state\n");
	}
	printf("Redirection type: ");
	switch (node->type)
	{
	case STDOUT:
		printf("STDOUT (>)\n");
		break ;
	case APPEND:
		printf("APPEND (>>)\n");
		break ;
	case STDIN:
		printf("STDIN (<)\n");
		break ;
	case HEREDOC:
		printf("HEREDOC (<<)\n");
		break ;
	case NO_DIR:
		printf("NO_DIR (not a redirection)\n");
		break ;
	default:
		printf("Unknown type\n");
		break ;
	}
	if (node->next)
		printf("Next node exists.\n");
	else
		printf("No next node.\n");
}

// Fonction de debug pour parcourir toute une liste chaînée de t_node
void	debug_list(t_node **head)
{
	t_node	*current;
	int		index;

	current = *head;
	index = 0;
	while (current)
	{
		printf("\033[1;31m=== Node %d ===\033[0m\n", index++);
		debug_node(current);
		current = current->next;
	}
	if (index == 0)
	{
		printf("The list is empty.\n");
	}
}

// work with one argument no try yet with more
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