/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 15:06:36 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/30 13:05:05 by lhopp            ###   ########.fr       */
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

int	**create_pipe_array(int pipe_count)
{
	int	**pipe_array;
	int	i;

	if (pipe_count <= 0)
		return (NULL);
	pipe_array = (int **)malloc(sizeof(int *) * pipe_count);
	if (!pipe_array)
		return (NULL);
	i = 0;
	while (i < pipe_count)
	{
		pipe_array[i] = (int *)malloc(sizeof(int) * 2);
		if (!pipe_array[i])
		{
			while (--i >= 0)
				free(pipe_array[i]);
			free(pipe_array);
			return (NULL);
		}
		if (pipe(pipe_array[i]) == -1)
		{
			while (i >= 0)
				free(pipe_array[i]);
			free(pipe_array);
			return (NULL);
		}
		i++;
	}
	return (pipe_array);
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

void	execute_pipeline(t_shell *shell)
{
	t_p			data;
	t_list		*tokens;
	t_cmd		*cmd;
	int			nb_pipe;
	int			nb_child;
	int			**pipes;
	pid_t		*pids;
	int			i;
	int			status;
	const char	*cmd_end;
	t_list		*new_tokens_end;
	t_list		*new_current_end;
	int			j;

	initialize_pipeline_data(&data);
	tokens = shell->tokens;
	nb_pipe = count_pipe(&shell->tokens);
	nb_child = count_processes(&shell->tokens);
	i = 0;
	while (nb_pipe == nb_child && last_is_pipe(&shell->tokens))
	{
		cmd_end = get_input1();
		if (cmd_end == NULL)
			return ;
		new_tokens_end = tokenize_input(cmd_end);
		free((char *)cmd_end);
		new_current_end = new_tokens_end;
		while (new_current_end)
		{
			add_token(&shell->tokens, new_current_end->content,
				ft_strlen(new_current_end->content));
			new_current_end = new_current_end->next;
		}
		ft_lstclear(&new_tokens_end, free);
		nb_pipe = count_pipe(&shell->tokens);
		nb_child = count_processes(&shell->tokens);
	}
	pipes = create_pipe_array(nb_pipe);
	pids = create_pid_array(nb_child);
	if (!pipes || !pids)
	{
		free_pipe_array(pipes, nb_pipe);
		free_pid_array(pids);
		return ;
	}
	while (tokens && i < nb_child)
	{
		cmd = process_command1(&data, &tokens, shell);
		if (cmd == NULL)
			break ;
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("Fork failed");
			shell->last_status = 1;
			break ;
		}
		if (pids[i] == 0)
		{
			if (i == 0)
			{
				if (nb_pipe > 0)
					dup2(pipes[0][1], STDOUT_FILENO);
			}
			else if (i == nb_child - 1)
				dup2(pipes[i - 1][0], STDIN_FILENO);
			else
			{
				dup2(pipes[i - 1][0], STDIN_FILENO);
				dup2(pipes[i][1], STDOUT_FILENO);
			}
			j = 0;
			while (j < nb_pipe)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
				j++;
			}
			handle_child_process(&data, cmd, shell);
		}
		free_cmd(cmd);
		i++;
	}
	i = 0;
	while (i < nb_pipe)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
	i = 0;
	while (i < nb_child)
	{
		waitpid(pids[i], &status, 0);
		if (i == nb_child - 1)
		{
			if (WIFEXITED(status))
				shell->last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->last_status = 128 + WTERMSIG(status);
			else
				shell->last_status = 1;
		}
		i++;
	}
	free_pipe_array(pipes, nb_pipe);
	free_pid_array(pids);
}
