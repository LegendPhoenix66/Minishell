/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_child_process.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:41:41 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/29 11:14:50 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_input_redirection(t_p *data, t_cmd *cmd)
{
	if (cmd->input_mode == 1)
	{
		data->fd = open(cmd->input_file, O_RDONLY);
		if (data->fd == -1)
		{
			perror("Error opening input file");
			free_cmd(cmd);
			exit(EXIT_FAILURE);
		}
		dup2(data->fd, STDIN_FILENO);
		close(data->fd);
	}
}

void	handle_output_redirection(t_p *data, t_cmd *cmd)
{
	int	flags;

	if (cmd->output_mode)
	{
		if (cmd->output_mode == 1)
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		else
			flags = O_WRONLY | O_CREAT | O_APPEND;
		data->fd = open(cmd->output_file, flags, 0644);
		if (data->fd == -1)
		{
			perror("Error opening output file");
			free_cmd(cmd);
			exit(EXIT_FAILURE);
		}
		dup2(data->fd, STDOUT_FILENO);
		close(data->fd);
	}
}

void	execute_external_command(t_p *data, t_cmd *cmd, t_shell *shell)
{
	data->exec_path = find_executable(shell, cmd->args[0]);
	if (!data->exec_path)
	{
		free_cmd(cmd);
		exit(127);
	}
	execve(data->exec_path, cmd->args, shell->environ);
	perror("execve failed");
	free(data->exec_path);
	free_cmd(cmd);
	exit(EXIT_FAILURE);
}

void	handle_child_process(t_p *data, t_cmd *cmd, t_shell *shell)
{
	handle_input_redirection(data, cmd);
	handle_output_redirection(data, cmd);
	if (is_builtin(cmd->args[0]))
	{
		data->ret = execute_builtin(cmd, shell);
		free_cmd(cmd);
		exit(data->ret);
	}
	execute_external_command(data, cmd, shell);
}
