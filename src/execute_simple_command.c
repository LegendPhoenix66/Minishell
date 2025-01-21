/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 15:54:55 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/21 10:29:43 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

static int	get_filename(t_cmd *cmd, int mode, const char **filename)
{
	if (mode == 1)
		*filename = cmd->input_file;
	else
		*filename = cmd->output_file;
	if (!filename)
	{
		ft_putstr_fd("minishell: No file specified\n", 2);
		return (0);
	}
	return (1);
}

static int	get_flags(t_cmd *cmd, int mode)
{
	if (mode == 1)
		return (O_RDONLY);
	if (cmd->output_mode == 1)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	return (O_WRONLY | O_CREAT | O_APPEND);
}

int	handle_fd_error(t_cmd *cmd, int mode)
{
	int			fd;
	int			flags;
	const char	*filename;

	if (!get_filename(cmd, mode, &filename))
		return (-1);
	flags = get_flags(cmd, mode);
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("file error", 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (-1);
	}
	return (fd);
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
