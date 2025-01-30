/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:36:10 by lhopp             #+#    #+#             */
/*   Updated: 2025/01/30 20:56:42 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_pipe_prompt(void)
{
	char	*prompt;
	char	*elements[4];
	size_t	size;
	int		i;

	elements[0] = COLOR_GREEN;
	elements[1] = ">";
	elements[2] = COLOR_RESET;
	elements[3] = NULL;
	size = calculate_size(elements);
	prompt = malloc(size);
	if (!prompt)
		return (NULL);
	i = 1;
	ft_strlcpy(prompt, elements[0], size);
	while (elements[i])
	{
		ft_strlcat(prompt, elements[i], size);
		i++;
	}
	return (prompt);
}

char	*get_input1(void)
{
	char	*line;
	char	*prompt;
	char	*trimmed_line;

	if (isatty(STDIN_FILENO))
	{
		prompt = get_pipe_prompt();
		line = readline(prompt);
		free(prompt);
	}
	else
	{
		line = get_next_line(STDIN_FILENO);
		trimmed_line = ft_strtrim(line, "\n");
		free(line);
		line = trimmed_line;
	}
	if (line == NULL)
		return (NULL);
	trim_and_remove_whitespace(line);
	if (*line)
		add_history(line);
	return (line);
}

void	handle_child(t_pipeline_ctx *ctx, t_cmd *cmd, int i)
{
	if (i == 0 && ctx->nb_pipe > 0)
		dup2(ctx->pipes[0][1], STDOUT_FILENO);
	else if (i == ctx->nb_child - 1)
		dup2(ctx->pipes[i - 1][0], STDIN_FILENO);
	else
	{
		dup2(ctx->pipes[i - 1][0], STDIN_FILENO);
		dup2(ctx->pipes[i][1], STDOUT_FILENO);
	}
	close_pipes(ctx->pipes, ctx->nb_pipe);
	handle_child_process(&ctx->data, cmd, ctx->shell);
	exit(ctx->shell->last_status);
}

int	process_child(t_pipeline_ctx *ctx, int i)
{
	t_cmd	*cmd;

	cmd = process_command1(&ctx->data, &ctx->tokens, ctx->shell);
	if (cmd == NULL)
		return (0);
	ctx->pids[i] = fork();
	if (ctx->pids[i] < 0)
	{
		perror("Fork failed");
		ctx->shell->last_status = 1;
		free_cmd(cmd);
		return (0);
	}
	if (ctx->pids[i] == 0)
		handle_child(ctx, cmd, i);
	free_cmd(cmd);
	return (1);
}

void	initialize_pipeline_data(t_p *data)
{
	data->input_fd = STDIN_FILENO;
	data->is_last_command = 0;
	data->saved_stdin = -1;
	data->fd = -1;
	data->exec_path = NULL;
	data->ret = 0;
	data->status = 0;
}
