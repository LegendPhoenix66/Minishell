/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 22:09:03 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/21 10:32:03 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	append_to_new_content(char **new_content, int *output_index,
		const char *str)
{
	size_t	len;

	len = ft_strlen(str);
	*new_content = ft_realloc(*new_content, *output_index, *output_index + len
			+ 1);
	ft_memcpy(&(*new_content)[*output_index], str, len);
	*output_index += len;
	(*new_content)[*output_index] = '\0';
}

// Helper function to append a variable's value
void	append_variable_value(t_context *ctx, const char *value)
{
	if (value)
	{
		append_to_new_content(ctx->new_content, ctx->output_index, value);
	}
}

static void	handle_variable_value(const char *variable_name, int var_length,
		t_context *ctx)
{
	char	*status_str;
	char	*env_value;

	if (ft_strncmp(variable_name, "?", var_length) == 0 && var_length == 1)
	{
		status_str = ft_itoa(ctx->last_status);
		append_variable_value(ctx, status_str);
		free(status_str);
	}
	else
	{
		env_value = getenv(variable_name);
		append_variable_value(ctx, env_value);
	}
}

void	process_variable_substitution(const char *content, int *index,
		t_context *ctx)
{
	int		start_index;
	int		var_length;
	char	*variable_name;

	start_index = (*index)++;
	while (content[*index] && content[*index] != ' ' && content[*index] != '"'
		&& content[*index] != '\'')
		(*index)++;
	var_length = *index - (start_index + 1);
	if (var_length == 0)
	{
		append_to_new_content(ctx->new_content, ctx->output_index, "$");
		return ;
	}
	variable_name = malloc(var_length + 1);
	ft_strlcpy(variable_name, &content[start_index + 1], var_length + 1);
	handle_variable_value(variable_name, var_length, ctx);
	free(variable_name);
}

void	process_quoted_content(const char *content, int *index, t_context *ctx,
		char quote)
{
	(*index)++;
	if (*ctx->new_content == NULL)
	{
		*ctx->new_content = ft_realloc(*ctx->new_content, 0, 1);
		(*ctx->new_content)[0] = '\0';
	}
	while (content[*index] && content[*index] != quote)
	{
		if (content[*index] == '$' && quote == '"')
		{
			process_variable_substitution(content, index, ctx);
		}
		else
		{
			*ctx->new_content = ft_realloc(*ctx->new_content,
					*(ctx->output_index), *ctx->output_index + 2);
			(*ctx->new_content)[(*ctx->output_index)++] = content[(*index)++];
		}
	}
	(*index)++;
}
