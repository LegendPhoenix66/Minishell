/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 23:17:58 by lhopp             #+#    #+#             */
/*   Updated: 2025/01/25 22:51:03 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_for_heredoc(t_list *tokens)
{
	char	*token;

	while (tokens)
	{
		token = tokens->content;
		if (ft_strcmp(token, "<<") == 0)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}

int	check_for_pipe(t_list *tokens)
{
	char	*token;

	while (tokens)
	{
		token = tokens->content;
		if (ft_strcmp(token, "|") == 0)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}

void	execute_command1(t_shell *shell)
{
	t_cmd	*cmd;
	int		has_pipe;

	if (shell->tokens == NULL)
		return ;
	has_pipe = check_for_pipe(shell->tokens);
	if (has_pipe)
		execute_pipeline(shell);
	else
	{
		cmd = parse_command(shell, shell->tokens);
		if (cmd)
		{
			execute_simple_command(cmd, shell);
			free_cmd(cmd);
		}
	}
}
