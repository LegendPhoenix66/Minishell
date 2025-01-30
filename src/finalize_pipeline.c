/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalize_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:38:34 by lhopp             #+#    #+#             */
/*   Updated: 2025/01/30 20:56:42 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	close_pipes(int **pipes, int nb_pipe)
{
	int	i;

	i = 0;
	while (i < nb_pipe)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	wait_for_children(t_pipeline_ctx *ctx)
{
	int	i;
	int	status;

	i = 0;
	while (i < ctx->nb_child)
	{
		waitpid(ctx->pids[i], &status, 0);
		if (i == ctx->nb_child - 1)
		{
			if (WIFEXITED(status))
				ctx->shell->last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				ctx->shell->last_status = 128 + WTERMSIG(status);
			else
				ctx->shell->last_status = 1;
		}
		i++;
	}
}

void	free_pipe_array(int **pipe_array, int pipe_count)
{
	int	i;

	if (!pipe_array)
		return ;
	i = 0;
	while (i < pipe_count)
	{
		if (pipe_array[i])
		{
			close(pipe_array[i][0]);
			close(pipe_array[i][1]);
			free(pipe_array[i]);
		}
		i++;
	}
	free(pipe_array);
}

void	free_pid_array(pid_t *pid_array)
{
	if (!pid_array)
		return ;
	free(pid_array);
}

void	finalize_pipeline(t_pipeline_ctx *ctx)
{
	close_pipes(ctx->pipes, ctx->nb_pipe);
	wait_for_children(ctx);
	free_pipe_array(ctx->pipes, ctx->nb_pipe);
	free_pid_array(ctx->pids);
}
