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

void	ft_realloc(void **ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (new_size == 0)
	{
		if (*ptr)
            free(*ptr);
		ptr = NULL;
		return ;
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return ;
	if (*ptr)
	{
		ft_memcpy(new_ptr, *ptr, old_size);
		free(*ptr);
	}
	*ptr = new_ptr;
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
		ft_realloc((void **)ctx->new_content, *(ctx->output_index), *(ctx->output_index) + 2);
		if (!*(ctx->new_content))
	        return ;
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

void	clean_arg(char **token, int last_status)
{
	t_context	ctx;
	char		*processed_content;
	int			processed_length;

	processed_content = NULL;
	processed_length = 0;
	if (!token || !*token)
		return ;
	ctx.last_status = last_status;
	ctx.new_content = &processed_content;
	ctx.output_index = &processed_length;
	process_token_content(*token, &ctx);
	if (processed_content)
	{
		if (*token)
			free(*token);
		*token = processed_content;
	}
}

t_list	*tokenize_input(const char *input)
{
	t_list	*parsed_tokens;

	parsed_tokens = split_by_spaces(input);
	if (!parsed_tokens)
		return (NULL);
	correct_pipes_and_redirects(&parsed_tokens);
	return (parsed_tokens);
}
