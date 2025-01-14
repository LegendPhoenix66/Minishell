/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 15:54:55 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/14 14:58:02 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	wait_for_child(pid_t pid, int *status, t_shell *shell)
{
	waitpid(pid, status, 0);
	if (WIFEXITED(*status))
		shell->last_status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
		shell->last_status = 128 + WTERMSIG(*status);
	else
		shell->last_status = 1;
}

static void	execute_command(t_cmd *cmd, t_shell *shell)
{
	char	*exec_path;

	exec_path = find_executable(cmd->args[0]);
	if (!exec_path)
	{
		fprintf(stderr, "minishell: command not found: %s\n", cmd->args[0]);
		free_cmd(cmd);
		exit(127);
	}
	execve(exec_path, cmd->args, shell->environ);
	perror("execve failed");
	free(exec_path);
	free_cmd(cmd);
	exit(EXIT_FAILURE);
}

static int	handle_fd_error(t_cmd *cmd)
{
	int	fd;

	fd = open(cmd->input_file, O_WRONLY);
	if (fd == -1)
	{
		perror("Error opening input file");
		free_cmd(cmd);
		exit(EXIT_FAILURE);
	}
	return (fd);
}

static void	handle_io_redirection(t_cmd *cmd)
{
	int	fd;

	if (cmd->input_mode == 1)
	{
		fd = handle_fd_error(cmd);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (cmd->input_mode == 2 && cmd->input_fd > 0)
	{
		dup2(cmd->input_fd, STDIN_FILENO);
		close(cmd->input_fd);
	}
	if (cmd->output_mode == 1)
	{
		fd = handle_fd_error(cmd);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (cmd->output_mode == 2)
	{
		fd = handle_fd_error(cmd);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	execute_simple_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, shell);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		handle_io_redirection(cmd);
		execute_command(cmd, shell);
	}
	else if (pid > 0)
		wait_for_child(pid, &status, shell);
	else
	{
		perror("fork failed");
		shell->last_status = 1;
		free_cmd(cmd);
		exit(EXIT_FAILURE);
	}
}
