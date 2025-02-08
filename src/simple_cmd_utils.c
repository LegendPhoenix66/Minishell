/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:26:56 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/30 21:23:15 by lhopp            ###   ########.fr       */
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

static int	get_filename(t_cmd *cmd, int mode, const char **filename)
{
	if (mode == 1)
		*filename = cmd->input_file;
	else
		*filename = cmd->output_file;
	if (*filename == NULL)
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

int	setup_redirection(t_cmd *cmd, int mode, int target_fd)
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
