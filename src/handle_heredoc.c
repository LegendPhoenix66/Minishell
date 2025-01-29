/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:43:38 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/29 10:58:38 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_heredoc_parent(t_cmd *cmd, int pipe_fd[2], pid_t pid)
{
	int	status;

	close(pipe_fd[1]);
	cmd->input_fd = pipe_fd[0];
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		perror("Heredoc process ended with error\n");
}

int	process_heredoc_line(char *line, char *delimiter, int fd)
{
	size_t	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	write_heredoc_line(fd, line);
	free(line);
	return (0);
}

void	handle_heredoc_child(int pipe_fd[2], char *delimiter)
{
	close(pipe_fd[0]);
	process_heredoc_input(pipe_fd[1], delimiter);
	close(pipe_fd[1]);
	exit(0);
}

void	handle_heredoc(t_cmd *cmd, char *delimiter)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (setup_heredoc_pipe(pipe_fd) == -1)
		return ;
	pid = fork();
	if (pid == 0)
		handle_heredoc_child(pipe_fd, delimiter);
	else if (pid > 0)
		handle_heredoc_parent(cmd, pipe_fd, pid);
}

t_list	*cmd_befor_heredoc(t_list **top)
{
	t_list	*current;
	t_list	*befor;

	befor = NULL;
	current = *top;
	if (top == NULL || *top == NULL)
		return (0);
	while (current != NULL)
	{
		if (ft_strcmp(current->content, "<<") == 0)
			break ;
		add_token(&befor, current->content, ft_strlen(current->content));
		current = current->next;
	}
	return (befor);
}

t_list	*cmd_after_heredoc(t_list **top)
{
	t_list	*current;
	t_list	*after;

	after = NULL;
	current = *top;
	if (top == NULL || *top == NULL)
		return (0);
	while (current != NULL)
	{
		if (ft_strcmp(current->content, "<<") == 0 && current->next)
		{
			add_token(&after, current->next->content,
				ft_strlen(current->next->content));
			break ;
		}
		current = current->next;
	}
	return (after);
}
