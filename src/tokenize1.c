/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 22:08:47 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/20 22:08:47 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	error(const char *msg)
{
	write(2, "Error: ", 7);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(EXIT_FAILURE);
}

t_list	*add_token(t_list **list, const char *token, int length)
{
	t_list	*new_node;

	new_node = ft_lstnew(ft_substr(token, 0, length));
	if (new_node == NULL)
	{
		error("malloc token");
		return (NULL);
	}
	ft_lstadd_back(list, new_node);
	return (new_node);
}

// Skips a quoted section until the closing quote or throws an error
int	skip_quoted_section(const char *input, int start_index, char quote)
{
	int	end_quote;

	end_quote = start_index + 1;
	while (input[end_quote] && input[end_quote] != quote)
		end_quote++;
	if (!input[end_quote])
		error("Unmatched quote");
	return (end_quote + 1);
}

// Function to split input by spaces
t_list	*split_by_spaces(const char *input)
{
	t_list	*parsed_tokens;
	char	single_quote;
	char	double_quote;
	int		index;

	single_quote = '\'';
	double_quote = '"';
	parsed_tokens = NULL;
	index = 0;
	while (input[index])
	{
		if (input[index] == ' ')
		{
			add_token(&parsed_tokens, input, index);
			input += index + 1;
			index = 0;
		}
		else if (input[index] == single_quote || input[index] == double_quote)
			index = skip_quoted_section(input, index, input[index]);
		else
			index++;
	}
	if (index > 0)
		add_token(&parsed_tokens, input, index);
	return (parsed_tokens);
}