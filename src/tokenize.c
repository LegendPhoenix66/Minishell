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

void	error(const char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
	exit(EXIT_FAILURE);
}

t_list	*add_token(t_list **list, const char *token, int length)
{
	t_list	*new_node;

	new_node = ft_lstnew(strndup(token, length));
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
	add_token(tokens_with_pipes, content, index);
	add_token(tokens_with_pipes, content + index, length);
	return (index + length);
}

void	correct_pipes_and_redirects(t_list *parsed_tokens)
{
	t_list	*tokens_with_pipes;
	t_list	*current_token;
	int		index;
	int		last_pipe_pos;
	char	*token_content;

	tokens_with_pipes = NULL;
	current_token = parsed_tokens;
	while (current_token)
	{
		index = 0;
		last_pipe_pos = -1;
		token_content = current_token->content;
		while (token_content[index])
		{
			if (token_content[index] == '\"' || token_content[index] == '\'')
				index = process_quoted_section(token_content, index,
						token_content[index]);
			else if (token_content[index] == '|' || token_content[index] == '>'
				|| token_content[index] == '<')
			{
				index = process_pipe_or_redirect(&tokens_with_pipes,
						token_content, index);
				last_pipe_pos = index - 1;
			}
			else
				index++;
		}
		add_token(&tokens_with_pipes, token_content + last_pipe_pos + 1,
			strlen(token_content) - last_pipe_pos - 1);
		current_token = current_token->next;
	}
	parsed_tokens = tokens_with_pipes;
	ft_lstclear(&tokens_with_pipes, free);
}

// Helper to copy remaining characters
static char	*append_char(char *buffer, int *pos, char c)
{
	buffer = realloc(buffer, *pos + 2);
	buffer[(*pos)++] = c;
	buffer[*pos] = '\0';
	return (buffer);
}

// Helper function to process and expand a variable
static char	*expand_variable(const char *content, int *index, int last_status,
		char *new_content, int *new_index)
{
	int		var_start;
	char	*var_value;
	char	status_str[12];
	char	*var_name;
	size_t	value_length;

	var_start = *index;
	var_value = NULL;
	while (content[*index] && content[*index] != ' ' && content[*index] != '"'
		&& content[*index] != '\'')
	{
		(*index)++;
	}
	var_name = strndup(&content[var_start], *index - var_start);
	if (strcmp(var_name, "?") == 0)
	{
		snprintf(status_str, sizeof(status_str), "%d", last_status);
		var_value = status_str;
	}
	else
	{
		var_value = getenv(var_name);
	}
	if (var_value)
	{
		value_length = strlen(var_value);
		new_content = realloc(new_content, *new_index + value_length + 1);
		strcpy(&new_content[*new_index], var_value);
		*new_index += value_length;
	}
	free(var_name);
	return (new_content);
}

// Helper function to process quoted sections
static char	*process_string_with_quotes(const char *content, int *index,
		char quote, int last_status, char *new_content, int *new_index)
{
	(*index)++;
	while (content[*index] && content[*index] != quote)
	{
		if (content[*index] == '$' && quote == '"')
		{
			(*index)++;
			new_content = expand_variable(content, index, last_status,
					new_content, new_index);
		}
		else
		{
			new_content = append_char(new_content, new_index,
					content[(*index)++]);
		}
	}
	if (content[*index] == quote)
	{
		(*index)++;
	}
	return (new_content);
}

// Refactored main function
void	remove_quotes_and_substitute_variables(t_list *tokens, int last_status)
{
	t_list	*current;
	char	*content;
	char	*new_content;
	int		index;
	int		new_index;

	current = tokens;
	while (current)
	{
		content = current->content;
		new_content = NULL;
		index = 0;
		new_index = 0;
		while (content[index])
		{
			if (content[index] == '"' || content[index] == '\'')
			{
				new_content = process_string_with_quotes(content, &index,
						content[index], last_status, new_content, &new_index);
			}
			else if (content[index] == '$')
			{
				index++;
				new_content = expand_variable(content, &index, last_status,
						new_content, &new_index);
			}
			else
			{
				new_content = append_char(new_content, &new_index,
						content[index++]);
			}
		}
		if (new_content)
		{
			if (new_index > 0)
				new_content[new_index] = '\0';
			free(current->content);
			current->content = new_content;
		}
		current = current->next;
	}
}

t_list	*tokenize_input(const char *input, int last_status)
{
	t_list	*parsed_tokens;

	parsed_tokens = split_by_spaces(input);
	if (!parsed_tokens)
		return (NULL);
	correct_pipes_and_redirects(parsed_tokens);
	remove_quotes_and_substitute_variables(parsed_tokens, last_status);
	return (parsed_tokens);
}
