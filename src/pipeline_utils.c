/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:47:26 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/30 21:27:24 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	process_tokens1(t_list **tokens, t_p *data)
{
	while (*tokens && ft_strcmp((*tokens)->content, "|") != 0)
		*tokens = (*tokens)->next;
	data->is_last_command = (*tokens == NULL);
	if (!data->is_last_command)
		*tokens = (*tokens)->next;
}

void	free_partial_pipe_array(int **pipes, int pipe_idx)
{
	while (--pipe_idx >= 0)
	{
		free(pipes[pipe_idx]);
	}
	free(pipes);
}

int	**create_pipe_array(int num_pipes)
{
	int	**pipes;
	int	pipe_idx;

	if (num_pipes <= 0)
		return (NULL);
	pipes = (int **)malloc(sizeof(int *) * num_pipes);
	if (!pipes)
		return (NULL);
	pipe_idx = 0;
	while (pipe_idx < num_pipes)
	{
		pipes[pipe_idx] = (int *)malloc(sizeof(int) * 2);
		if (!pipes[pipe_idx])
		{
			free_partial_pipe_array(pipes, pipe_idx);
			return (NULL);
		}
		if (pipe(pipes[pipe_idx]) == -1)
		{
			free_partial_pipe_array(pipes, pipe_idx + 1);
			return (NULL);
		}
		pipe_idx++;
	}
	return (pipes);
}

pid_t	*create_pid_array(int process_count)
{
	pid_t	*pid_array;
	int		i;

	if (process_count <= 0)
		return (NULL);
	pid_array = (pid_t *)malloc(sizeof(pid_t) * process_count);
	if (!pid_array)
		return (NULL);
	i = 0;
	while (i < process_count)
	{
		pid_array[i] = 0;
		i++;
	}
	return (pid_array);
}

int	initialize_pipes_and_pids(t_pipeline_ctx *ctx)
{
	ctx->pipes = create_pipe_array(ctx->nb_pipe);
	ctx->pids = create_pid_array(ctx->nb_child);
	if (ctx->pipes == NULL || ctx->pids == NULL)
	{
		free_pipe_array(ctx->pipes, ctx->nb_pipe);
		free_pid_array(ctx->pids);
		return (0);
	}
	return (1);
}
