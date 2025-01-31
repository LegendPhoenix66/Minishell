/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_msg_array.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <drenquin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:44:47 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/31 16:46:12 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**create_message_array(char *delimiter, t_shell *shell)
{
	char		**messages;
	int			capacity;
	int			size;
	t_context	*ctx;
	char		*input;
	char		*processed_input;
	char		**temp;

	messages = NULL;
	capacity = 10;
	size = 0;
	ctx = init_context(shell);
	if (!ctx)
		return (NULL);
	messages = malloc(sizeof(char *) * (capacity + 1));
	if (!messages)
	{
		free_context(ctx);
		return (NULL);
	}
	messages[0] = NULL;
	while (1)
	{
		input = get_input1();
		if (!input)
		{
			free_message_array(messages);
			free_context(ctx);
			return (NULL);
		}
		if (ft_strcmp(input, delimiter) == 0)
		{
			free(input);
			break ;
		}
		processed_input = process_line(input, ctx);
		if (!processed_input)
		{
			free_message_array(messages);
			free_context(ctx);
			return (NULL);
		}
		if (size >= capacity)
		{
			capacity *= 2;
			temp = realloc(messages, sizeof(char *) * (capacity + 1));
			if (!temp)
			{
				free(processed_input);
				free_message_array(messages);
				free_context(ctx);
				return (NULL);
			}
			messages = temp;
		}
		messages[size] = ft_strjoin(processed_input, "\n");
		free(processed_input);
		if (!messages[size])
		{
			free_message_array(messages);
			free_context(ctx);
			return (NULL);
		}
		size++;
		messages[size] = NULL;
	}
	free_context(ctx);
	return (messages);
}
