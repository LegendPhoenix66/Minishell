/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_msg_array.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:44:47 by drenquin          #+#    #+#             */
/*   Updated: 2025/02/01 10:42:07 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	**expand_messages_array(char **messages, int size, t_context *ctx,
		char *processed_input)
{
	char	**new_messages;
	int		old_capacity;
	int		new_capacity;
	int		i;

	if (size == 0)
		old_capacity = 10;
	else
		old_capacity = size * 2;
	new_capacity = old_capacity * 2;
	new_messages = malloc(sizeof(char *) * (new_capacity + 1));
	if (!new_messages)
		return (cleanup_and_exit1(messages, ctx, processed_input));
	i = 0;
	while (i < size && messages[i])
	{
		new_messages[i] = messages[i];
		i++;
	}
	while (i <= new_capacity)
		new_messages[i++] = NULL;
	free(messages);
	return (new_messages);
}

char	**short_loop(char **messages, int *size, t_context *ctx,
		char *processed_input)
{
	if (*size >= 10)
	{
		messages = expand_messages_array(messages, *size, ctx, processed_input);
		if (!messages)
			return (NULL);
	}
	return (messages);
}

static char	**process_message_loop(char **messages, int *size, t_context *ctx,
		char *delimiter)
{
	char	*input;
	char	*processed_input;

	while (1)
	{
		input = get_input1();
		if (!input)
			return (cleanup_and_exit(messages, ctx));
		if (ft_strcmp(input, delimiter) == 0)
		{
			free(input);
			return (messages);
		}
		processed_input = process_line(input, ctx);
		if (!processed_input)
			return (cleanup_and_exit(messages, ctx));
		messages = short_loop(messages, size, ctx, processed_input);
		messages[*size] = ft_strjoin(processed_input, "\n");
		free(processed_input);
		if (!messages[*size])
			return (cleanup_and_exit(messages, ctx));
		(*size)++;
		messages[*size] = NULL;
	}
	return (messages);
}

char	**create_message_array(char *delimiter, t_shell *shell)
{
	char		**messages;
	t_context	*ctx;
	int			size;

	messages = NULL;
	size = 0;
	ctx = init_context(shell);
	if (!ctx)
		return (NULL);
	messages = malloc(sizeof(char *) * (11));
	if (!messages)
	{
		free_context(ctx);
		return (NULL);
	}
	messages[0] = NULL;
	messages = process_message_loop(messages, &size, ctx, delimiter);
	free_context(ctx);
	return (messages);
}
