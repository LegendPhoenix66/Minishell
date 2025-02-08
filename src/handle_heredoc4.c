/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:03:02 by lhopp             #+#    #+#             */
/*   Updated: 2025/02/01 11:03:07 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_tokens(char **tokens)
{
	int	j;

	if (tokens)
	{
		j = 0;
		while (tokens[j] != NULL)
		{
			free(tokens[j]);
			j++;
		}
		free(tokens);
	}
}

void	free_message_array(char **messages)
{
	int	i;

	if (!messages)
		return ;
	i = 0;
	while (messages[i])
	{
		free(messages[i]);
		i++;
	}
	free(messages);
	messages = NULL;
}

void	free_context(t_context *ctx)
{
	if (!ctx)
		return ;
	if (ctx->new_content)
		free(ctx->new_content);
	if (ctx->output_index)
		free(ctx->output_index);
	free(ctx);
}

char	*concat_strings(char *s1, char *s2)
{
	char	*result;
	char	*temp;

	if (!s1)
		return (ft_strdup(s2));
	temp = s1;
	result = ft_strjoin(s1, s2);
	free(temp);
	return (result);
}

char	*create_string_from_array(char **messages)
{
	char	*result;
	int		i;

	result = NULL;
	i = 0;
	if (!messages)
		return (NULL);
	while (messages[i])
	{
		result = concat_strings(result, messages[i]);
		if (!result)
			return (NULL);
		i++;
	}
	messages[i] = NULL;
	return (result);
}
