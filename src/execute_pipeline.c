/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 19:33:28 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/25 22:51:56 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_builtin(t_cmd *cmd, t_shell *shell, t_p *data)
{
	if (data->input_fd != STDIN_FILENO)
	{
		data->saved_stdin = dup(STDIN_FILENO);
		dup2(data->input_fd, STDIN_FILENO);
		execute_builtin(cmd, shell);
		dup2(data->saved_stdin, STDIN_FILENO);
		close(data->saved_stdin);
		close(data->input_fd);
	}
	else
		execute_builtin(cmd, shell);
}

t_cmd	*process_command1(t_p *data, t_list **tokens, t_shell *shell)
{
	t_cmd	*cmd;

	cmd = parse_command(shell, *tokens);
	if (!cmd)
		return (NULL);
	process_tokens1(tokens, data);
	if (data->is_last_command && is_builtin(cmd->args[0]))
	{
		handle_builtin(cmd, shell, data);
		free_cmd(cmd);
		return (NULL);
	}
	return (cmd);
}
