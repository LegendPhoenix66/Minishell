/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 15:49:31 by lhopp             #+#    #+#             */
/*   Updated: 2025/01/14 13:38:11 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		ft_memcpy(new_ptr, ptr, old_size);
		free(ptr);
	}
	return (new_ptr);
}

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
		*(ctx->new_content) = ft_realloc(*(ctx->new_content),
				*(ctx->output_index), *(ctx->output_index) + 2);
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
	correct_pipes_and_redirects(&parsed_tokens);
	remove_quotes_and_substitute_variables(parsed_tokens, last_status);
	return (parsed_tokens);
}
