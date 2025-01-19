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

static int handle_fd_error(t_cmd *cmd, int mode)
{
    int fd;
    int flags;

    if (mode == 1)
        flags = O_RDONLY;
    else
    {
        if (cmd->output_mode == 1)
            flags = O_WRONLY | O_CREAT | O_TRUNC;
        else
            flags = O_WRONLY | O_CREAT | O_APPEND;
    }
    fd = open(cmd->input_file, flags, 0644);
    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->input_file, 2);
        ft_putstr_fd(": ", 2);
        ft_putstr_fd("error", 2);
        ft_putstr_fd("\n", 2);
        return (-1);
    }
    return (fd);
}

/*static void handle_io_redirection(t_cmd *cmd)
{
    int fd;

    if (cmd->input_mode == 1 && cmd->input_file)
    {
        fd = handle_fd_error(cmd, 1);
        if (fd == -1)
            exit(1);
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            close(fd);
            perror("minishell: dup2");
            exit(1);
        }
        close(fd);
    }
    if (cmd->output_mode)
    {
        fd = handle_fd_error(cmd, 2);
        if (fd == -1)
            exit(1);
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            close(fd);
            perror("minishell: dup2");
            exit(1);
        }
        close(fd);
    }
}*/
static int setup_redirection(t_cmd *cmd, int mode, int target_fd)
{
    int fd;

    fd = handle_fd_error(cmd, mode);
    if (fd == -1)
        return (-1);
    
    if (dup2(fd, target_fd) == -1)
    {
        close(fd);
        perror("minishell: dup2");
        return (-1);
    }
    close(fd);
    return (0);
}

static void handle_io_redirection(t_cmd *cmd)
{
    if (cmd->input_mode == 1 && cmd->input_file)
    {
        if (setup_redirection(cmd, 1, STDIN_FILENO) == -1)
            exit(1);
    }
    if (cmd->output_mode)
    {
        if (setup_redirection(cmd, 2, STDOUT_FILENO) == -1)
            exit(1);
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
