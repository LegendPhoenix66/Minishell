/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:02:51 by lhopp             #+#    #+#             */
/*   Updated: 2025/02/07 20:02:46 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_heredoc	*init_pipe_data(void)
{
	t_heredoc	*data;

	data = malloc(sizeof(t_heredoc));
	if (!data)
		return (NULL);
	if (pipe(data->pipe_fd) == -1)
	{
		free(data);
		return (NULL);
	}
	return (data);
}

int	start_process(t_heredoc *data, t_cmd *cmd, t_shell *shell)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	data->process_pid = fork();
	if (data->process_pid == -1)
	{
		perror("fork failed");
		return (1);
	}
	if (data->process_pid == 0)
	{
		close(data->pipe_fd[1]);
		dup2(data->pipe_fd[0], STDIN_FILENO);
		close(data->pipe_fd[0]);
		execute_command(cmd, shell);
		perror("process failed");
		exit(1);
	}
	else
		close(data->pipe_fd[0]);
	return (0);
}

void	write_messages(t_heredoc *data, char **messages)
{
	int	i;

	i = 0;
	while (messages[i])
	{
		write(data->pipe_fd[1], messages[i], ft_strlen(messages[i]));
		i++;
	}
	free_message_array(messages);
	close(data->pipe_fd[1]);
}

void	write_and_wait(t_heredoc *data, t_shell *shell)
{
	char	*delimiter;
	char	**messages;
	t_list	*process;
	t_list	*check_last;

	check_last = last_token(&shell->tokens);
	if (ft_strcmp(check_last->content, "<<") == 0)
	{
		perror("bash: syntax error near unexpected token newline\n");
		shell->last_status = 2;
		return ;
	}
	process = cmd_after_heredoc(&shell->tokens);
	delimiter = process->content;
	messages = create_message_array(delimiter, shell);
	if (messages == NULL)
		return ;
	write_messages(data, messages);
	ft_lstclear(&process, free);
	waitpid(data->process_pid, NULL, 0);
}

void	write_nothing(t_shell *shell)
{
	char	*delimiter;
	t_list	*process;
	char	*input;

	process = cmd_after_heredoc(&shell->tokens);
	delimiter = process->content;
	while (1)
	{
		input = get_input1();
		if (ft_strcmp(input, delimiter) == 0)
		{
			free(input);
			break ;
		}
		free(input);
	}
	ft_lstclear(&process, free);
}
