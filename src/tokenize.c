/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 15:49:31 by lhopp             #+#    #+#             */
/*   Updated: 2024/11/27 11:31:23 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	add_token(t_node **list, const char *token, int length)
{
	t_node	*new_node;
	t_node	*current;

	new_node = malloc(sizeof(t_node));
	new_node->content = strndup(token, length);
	new_node->next = NULL;
	if (*list == NULL)
	{
		*list = new_node;
	}
	else
	{
		current = *list;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = new_node;
	}
}

void error(const char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
	exit(EXIT_FAILURE);
}

void tokenize_input(const char *input, t_args **args)
{
	char quote;
	int i, j, start;

	i = 0;
	j = 0;
	quote = 0;
	(*args)->tokens = NULL;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
		{
			quote = input[i];
			start = i;
			i++;
			while (input[i] && input[i] != quote)
				i++;
			if (input[i] != quote)
			{
				error("Unmatched quote");
				return;
			}
			add_token(&(*args)->tokens, input + start, i - start + 1);
			i++;
			j = 0;
		}
		else if ((input[i] == ' ' && quote == 0) || input[i] == '|' || input[i] == '<' || input[i] == '>')
		{
			if (j > 0)
			{
				add_token(&(*args)->tokens, input + i - j, j);
				j = 0;
			}
			if (input[i] != ' ')
			{
				add_token(&(*args)->tokens, input + i, 1);
			}
			i++;
		}
		else
		{
			j++;
			i++;
		}
	}
	if (j > 0)
	{
		add_token(&(*args)->tokens, input + i - j, j);
	}
}