/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_msg_array.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:44:47 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/31 17:25:30 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

typedef struct s_input_data
{
	char		**message_array;
	int			size;
	int			capacity;
	t_context	*context;
}				t_input_data;

static char	**resize_message_array(char **messages, int *capacity)
{
	char	**new_messages;

	*capacity *= 2;
	new_messages = realloc(messages, sizeof(char *) * (*capacity + 1));
	return (new_messages);
}

int	resize_if_needed(char ***message_array, int size, int *capacity)
{
	char	**resized_array;

	if (size >= *capacity)
	{
		*capacity += 10;
		resized_array = resize_message_array(*message_array, capacity);
		if (!resized_array)
		{
			return (0);
		}
		*message_array = resized_array;
	}
	return (1);
}

static int	handle_heredoc_input(t_input_data *data, char *user_input)
{
	char	*processed_input;

	processed_input = process_line(user_input, data->context);
	if (!processed_input)
	{
		return (0);
	}
	if (!resize_if_needed(&data->message_array, data->size, &data->capacity))
	{
		free(processed_input);
		return (0);
	}
	data->message_array[data->size] = ft_strjoin(processed_input, "\n");
	free(processed_input);
	if (!data->message_array[data->size])
	{
		return (0);
	}
	data->size++;
	data->message_array[data->size] = NULL;
	return (1);
}

static int	process_input_loop(t_input_data *data, char *delimiter)
{
	char	*user_input;
	int		result;

	while (1)
	{
		user_input = get_input1();
		if (!user_input)
		{
			return (0);
		}
		if (ft_strcmp(user_input, delimiter) == 0)
		{
			free(user_input);
			return (2);
		}
		result = handle_heredoc_input(data, user_input);
		if (result == 0)
		{
			return (0);
		}
	}
}

char	**create_message_array(char *delimiter, t_shell *shell)
{
	t_input_data	data;
	int				result;

	data.context = init_context(shell);
	if (!data.context)
	{
		return (NULL);
	}
	data.capacity = 10;
	data.size = 0;
	data.message_array = malloc(sizeof(char *) * (data.capacity + 1));
	if (!data.message_array)
	{
		free_context(data.context);
		return (NULL);
	}
	data.message_array[0] = NULL;
	result = process_input_loop(&data, delimiter);
	free_context(data.context);
	if (result == 0)
	{
		free_message_array(data.message_array);
		return (NULL);
	}
	return (data.message_array);
}
