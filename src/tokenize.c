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

// Function to split input by spaces
t_list	*split_by_spaces(const char *input)
{
	t_list	*parsed_tokens;
	int		i;
	char	quote;
	int		j;

	parsed_tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (input[i] == ' ')
		{
			add_token(&parsed_tokens, input, i);
			input += i + 1;
			i = 0;
		}
		else if (input[i] == '\"' || input[i] == '\'')
		{
			quote = input[i];
			j = i + 1;
			while (input[j] && input[j] != quote)
				j++;
			if (!input[j])
				error("unmatched quote");
			i = j + 1;
		}
		else
			i++;
	}
	add_token(&parsed_tokens, input, i);
	return (parsed_tokens);
}

// Function to correct pipes and redirects
void	correct_pipes_and_redirects(t_list *parsed_tokens)
{
	t_list	*tokens_with_pipes;
	t_list	*current;
	int		i;
	int		pipe_pos;
	char	*content;
	char	quote;
	int		length;

	tokens_with_pipes = NULL;
	current = parsed_tokens;
	while (current)
	{
		i = 0;
		pipe_pos = -1;
		content = current->content;
		while (content[i])
		{
			if (content[i] == '\"' || content[i] == '\'')
			{
				quote = content[i];
				i++;
				while (content[i] != quote)
				{
					i++;
				}
				i++;
			}
			if (content[i] == '|' || content[i] == '<' || content[i] == '>')
			{
				length = (content[i + 1] == content[i]) ? 2 : 1;
				add_token(&tokens_with_pipes, current->content, i);
				add_token(&tokens_with_pipes, current->content + i, length);
				pipe_pos = i + length - 1;
				i += length;
			}
			else
			{
				i++;
			}
		}
		add_token(&tokens_with_pipes, content + pipe_pos + 1, strlen(content)
			- pipe_pos - 1);
		current = current->next;
	}
	parsed_tokens = tokens_with_pipes;
	ft_lstclear(&tokens_with_pipes, free);
}

// Function to remove quotes and substitute variables
void	remove_quotes_and_substitute_variables(t_list *tokens, int last_status)
{
	t_list	*current;
	char	*new_content;
	int		i;
	int		j;
	char	*content;
	char	quote;
	int		var_start;
	char	*var_name;
	char	status_str[12];
	char	*var_value;

	current = tokens;
	while (current)
	{
		new_content = NULL;
		i = 0;
		j = 0;
		content = current->content;
		while (content[i])
		{
			if (content[i] == '\"' || content[i] == '\'')
			{
				quote = content[i++];
				while (content[i] && content[i] != quote)
				{
					if (content[i] == '$' && quote == '\"')
					{
						var_start = ++i;
						while (content[i] && content[i] != ' '
							&& content[i] != quote)
						{
							i++;
						}
						var_name = malloc(i - var_start + 1);
						strncpy(var_name, &content[var_start], i - var_start);
						var_name[i - var_start] = '\0';
						if (strcmp(var_name, "?") == 0)
						{
							snprintf(status_str, sizeof(status_str), "%d",
								last_status);
							new_content = realloc(new_content, j
									+ strlen(status_str) + 1);
							strcpy(&new_content[j], status_str);
							j += strlen(status_str);
						}
						else
						{
							var_value = getenv(var_name);
							if (var_value)
							{
								new_content = realloc(new_content, j
										+ strlen(var_value) + 1);
								strcpy(&new_content[j], var_value);
								j += strlen(var_value);
							}
						}
						free(var_name);
					}
					else
					{
						new_content = realloc(new_content, j + 2);
						new_content[j++] = content[i++];
					}
				}
				i++;
			}
			else if (content[i] == '$')
			{
				var_start = ++i;
				while (content[i] && content[i] != ' ' && content[i] != '\"'
					&& content[i] != '\'')
				{
					i++;
				}
				var_name = malloc(i - var_start + 1);
				strncpy(var_name, &content[var_start], i - var_start);
				var_name[i - var_start] = '\0';
				if (strcmp(var_name, "?") == 0)
				{
					snprintf(status_str, sizeof(status_str), "%d", last_status);
					new_content = realloc(new_content, j + strlen(status_str)
							+ 1);
					strcpy(&new_content[j], status_str);
					j += strlen(status_str);
				}
				else
				{
					var_value = getenv(var_name);
					if (var_value)
					{
						new_content = realloc(new_content, j + strlen(var_value)
								+ 1);
						strcpy(&new_content[j], var_value);
						j += strlen(var_value);
					}
				}
				free(var_name);
			}
			else
			{
				new_content = realloc(new_content, j + 2);
				new_content[j++] = content[i++];
			}
		}
		if (new_content)
		{
			new_content[j] = '\0';
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
