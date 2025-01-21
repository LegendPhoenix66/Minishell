/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:26:56 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/21 10:31:32 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	wait_for_child(pid_t pid, int *status, t_shell *shell)
{
	waitpid(pid, status, 0);
	if (WIFEXITED(*status))
		shell->last_status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
		shell->last_status = 128 + WTERMSIG(*status);
	else
		shell->last_status = 1;
}

static int	setup_redirection(t_cmd *cmd, int mode, int target_fd)
{
	int	fd;

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

void	handle_io_redirection(t_cmd *cmd)
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
