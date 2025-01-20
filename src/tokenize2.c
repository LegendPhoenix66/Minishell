/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 22:08:55 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/20 22:08:55 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Extracted helper: Process the quoted section
int	process_quoted_section(const char *content, int index, char quote)
{
	index++;
	while (content[index] && content[index] != quote)
		index++;
	if (!content[index])
		error("Unmatched quote");
	return (index + 1);
}

// Extracted helper: Process pipe or redirect
int	process_pipe_or_redirect(t_list **tokens_with_pipes, const char *content,
		int index)
{
	int	length;

	if (content[index + 1] == content[index])
		length = 2;
	else
		length = 1;
	if (index > 0)
		add_token(tokens_with_pipes, content, index);
	add_token(tokens_with_pipes, content + index, length);
	return (index + length);
}

// Extracted function to handle tokens of a single content line
void	process_token_line(const char *token_content,
		t_list **tokens_with_pipes)
{
	int	current_index;
	int	last_special_char_pos;

	current_index = 0;
	last_special_char_pos = -1;
	while (token_content[current_index])
	{
		if (token_content[current_index] == '\"'
			|| token_content[current_index] == '\'')
			current_index = process_quoted_section(token_content, current_index,
					token_content[current_index]);
		else if (token_content[current_index] == '|'
			|| token_content[current_index] == '>'
			|| token_content[current_index] == '<')
		{
			current_index = process_pipe_or_redirect(tokens_with_pipes,
					token_content, current_index);
			last_special_char_pos = current_index - 1;
		}
		else
			current_index++;
	}
	if (current_index > last_special_char_pos + 1)
		add_token(tokens_with_pipes, token_content + last_special_char_pos + 1,
			ft_strlen(token_content) - last_special_char_pos - 1);
}

void	correct_pipes_and_redirects(t_list **parsed_tokens)
{
	t_list	*tokens_with_pipes;
	t_list	*current_token;

	tokens_with_pipes = NULL;
	current_token = *parsed_tokens;
	while (current_token)
	{
		process_token_line(current_token->content, &tokens_with_pipes);
		current_token = current_token->next;
	}
	ft_lstclear(parsed_tokens, free);
	*parsed_tokens = tokens_with_pipes;
}