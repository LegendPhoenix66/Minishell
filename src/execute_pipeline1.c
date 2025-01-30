/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 15:06:36 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/30 14:01:03 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_pipe(t_list **top)
{
	t_list	*current;
	int		nb_pipe;

	nb_pipe = 0;
	current = *top;
	if (top == NULL || *top == NULL)
		return (0);
	while (current != NULL)
	{
		if (ft_strcmp(current->content, "|") == 0)
			nb_pipe++;
		current = current->next;
	}
	return (nb_pipe);
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

int	count_processes(t_list **tokens)
{
	t_list	*current;
	int		process_count;
	int		has_command_after_pipe;

	if (!tokens)
		return (0);
	process_count = 1;
	has_command_after_pipe = 0;
	current = *tokens;
	while (current)
	{
		if (ft_strcmp(current->content, "|") == 0)
		{
			process_count++;
			has_command_after_pipe = 0;
			if (current->next)
				has_command_after_pipe = 1;
		}
		else if (current->content)
			has_command_after_pipe = 1;
		current = current->next;
	}
	if (!has_command_after_pipe)
		process_count--;
	return (process_count);
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

void	free_pid_array(pid_t *pid_array)
{
	if (!pid_array)
		return ;
	free(pid_array);
}

int	last_is_pipe(t_list **top)
{
	t_list	*current;

	current = *top;
	if (top == NULL || *top == NULL)
		return (0);
	while (current != NULL)
	{
		if (ft_strcmp(current->content, "|") == 0 && current->next == NULL)
			return (1);
		current = current->next;
	}
	return (0);
}

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

static void	close_pipes(int **pipes, int nb_pipe)
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

static void	wait_for_children(t_pipeline_ctx *ctx)
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

static void	handle_child(t_pipeline_ctx *ctx, t_cmd *cmd, int i)
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

void	execute_pipeline(t_shell *shell)
{
	t_pipeline_ctx	ctx;
	int				i;
	t_cmd			*cmd;
	const char		*cmd_end;
	t_list			*new_tokens_end;
	t_list			*new_current_end;

	ctx.shell = shell;
	initialize_pipeline_data(&ctx.data);
	ctx.tokens = shell->tokens;
	ctx.nb_pipe = count_pipe(&ctx.tokens);
	ctx.nb_child = count_processes(&ctx.tokens);
	while (ctx.nb_pipe == ctx.nb_child && last_is_pipe(&ctx.tokens))
	{
		cmd_end = get_input1();
		if (cmd_end == NULL)
			return ;
		new_tokens_end = tokenize_input(cmd_end);
		free((char *)cmd_end);
		new_current_end = new_tokens_end;
		while (new_current_end != NULL)
		{
			add_token(&ctx.tokens, new_current_end->content,
				ft_strlen(new_current_end->content));
			new_current_end = new_current_end->next;
		}
		ft_lstclear(&new_tokens_end, free);
		ctx.nb_pipe = count_pipe(&ctx.tokens);
		ctx.nb_child = count_processes(&ctx.tokens);
	}
	ctx.pipes = create_pipe_array(ctx.nb_pipe);
	ctx.pids = create_pid_array(ctx.nb_child);
	if (ctx.pipes == NULL || ctx.pids == NULL)
	{
		free_pipe_array(ctx.pipes, ctx.nb_pipe);
		free_pid_array(ctx.pids);
		return ;
	}
	i = 0;
	while (ctx.tokens != NULL && i < ctx.nb_child)
	{
		cmd = process_command1(&ctx.data, &ctx.tokens, ctx.shell);
		if (cmd == NULL)
			break ;
		ctx.pids[i] = fork();
		if (ctx.pids[i] < 0)
		{
			perror("Fork failed");
			ctx.shell->last_status = 1;
			break ;
		}
		if (ctx.pids[i] == 0)
			handle_child(&ctx, cmd, i);
		free_cmd(cmd);
		i++;
	}
	close_pipes(ctx.pipes, ctx.nb_pipe);
	wait_for_children(&ctx);
	free_pipe_array(ctx.pipes, ctx.nb_pipe);
	free_pid_array(ctx.pids);
}
