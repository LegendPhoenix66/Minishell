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

static void	append_to_new_content(char **new_content, int *output_index,
		const char *str)
{
	size_t	len;

	len = strlen(str);
	*new_content = realloc(*new_content, *output_index + len + 1);
	strcpy(&(*new_content)[*output_index], str);
	*output_index += len;
}

// Helper function to append a variable's value
static void	append_variable_value(t_context *ctx, const char *value)
{
	if (value)
	{
		append_to_new_content(ctx->new_content, ctx->output_index, value);
	}
}

static void	process_variable_substitution(const char *content, int *index,
		t_context *ctx)
{
	int		start_index;
	char	status_str[12];
	int		var_length;
	char	*variable_name;
	char	*env_value;

	start_index = (*index)++;
	while (content[*index] && content[*index] != ' ' && content[*index] != '"'
		&& content[*index] != '\'')
		(*index)++;
	var_length = *index - start_index;
	variable_name = malloc(var_length + 1);
	strncpy(variable_name, &content[start_index], var_length);
	variable_name[var_length] = '\0';
	if (strcmp(variable_name, "?") == 0)
	{
		snprintf(status_str, sizeof(status_str), "%d", ctx->last_status);
		append_variable_value(ctx, status_str);
	}
	else
	{
		env_value = getenv(variable_name);
		append_variable_value(ctx, env_value);
	}
	free(variable_name);
}

static void	process_quoted_content(const char *content, int *index,
		t_context *ctx, char quote)
{
	(*index)++;
	while (content[*index] && content[*index] != quote)
	{
		if (content[*index] == '$' && quote == '"')
		{
			process_variable_substitution(content, index, ctx);
		}
		else
		{
			*ctx->new_content = realloc(*ctx->new_content, *ctx->output_index
					+ 2);
			(*ctx->new_content)[(*ctx->output_index)++] = content[(*index)++];
		}
	}
	(*index)++;
}

/**
 * Helper function to process a single character, performing different actions
 * like handling quotes, variable substitution,
	or appending as-is to new_content.
 */
static void	process_character(const char current_char,
		const char *input_content, int *current_char_index, t_context *ctx)
{
	if (current_char == '"' || current_char == '\'')
	{
		process_quoted_content(input_content, current_char_index, ctx,
			current_char);
	}
	else if (current_char == '$')
	{
		process_variable_substitution(input_content, current_char_index, ctx);
	}
	else
	{
		*(ctx->new_content) = realloc(*(ctx->new_content), *(ctx->output_index)
				+ 2);
		(*(ctx->new_content))[(*(ctx->output_index))++] = current_char;
		(*current_char_index)++;
	}
}

void	process_token_content(char *input_content, t_context *ctx)
{
	int		char_index;
	char	char_to_process;

	char_index = 0;
	while (input_content[char_index])
	{
		char_to_process = input_content[char_index];
		process_character(char_to_process, input_content, &char_index, ctx);
	}
	if (*(ctx->new_content))
	{
		(*(ctx->new_content))[*(ctx->output_index)] = '\0';
	}
}

void	remove_quotes_and_substitute_variables(t_list *tokens, int last_status)
{
	t_list		*current;
	t_context	ctx;
	char		*input_content;
	char		*processed_content;
	int			processed_length;

	current = tokens;
	while (current)
	{
		input_content = current->content;
		processed_content = NULL;
		processed_length = 0;
		ctx.last_status = last_status;
		ctx.new_content = &processed_content;
		ctx.output_index = &processed_length;
		process_token_content(input_content, &ctx);
		if (processed_content)
		{
			free(current->content);
			current->content = processed_content;
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
