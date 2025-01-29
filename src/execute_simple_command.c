/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 15:54:55 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/29 11:00:49 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_executable(const char *path)
{
	struct stat	s;

	if (!path)
		return (0);
	if (access(path, F_OK) == 0)
	{
		if (access(path, X_OK) == 0)
		{
			if (stat(path, &s) == 0)
			{
				if (S_ISREG(s.st_mode))
					return (1);
			}
		}
	}
	return (0);
}

char	*resolve_command_path(t_shell *shell, const char *cmd)
{
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (is_executable(cmd))
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (find_executable(shell, cmd));
}

void	execute_command(t_cmd *cmd, t_shell *shell)
{
	char	*exec_path;

	if (!cmd || !cmd->args || !cmd->args[0])
	{
		ft_putendl_fd("minishell: empty command", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	exec_path = resolve_command_path(shell, cmd->args[0]);
	if (!exec_path)
	{
		if (access(cmd->args[0], F_OK) == 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd->args[0], STDERR_FILENO);
			ft_putendl_fd(": Permission denied", STDERR_FILENO);
		}
		else
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd->args[0], STDERR_FILENO);
            ft_putendl_fd(": command not found", STDERR_FILENO);
		}
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
	pid_t		pid;
	int			status;
	t_heredoc	*data;

	data = NULL;
	if (check_for_heredoc(shell->tokens))
	{
		execute_heredoc(data, cmd, shell);
		return ;
	}
	if (is_builtin(cmd->args[0]))
	{
		shell->last_status = execute_builtin(cmd, shell);
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
