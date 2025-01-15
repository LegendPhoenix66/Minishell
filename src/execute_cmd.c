/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 23:17:58 by lhopp             #+#    #+#             */
/*   Updated: 2024/12/14 23:18:04 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	execute_command1(t_shell *shell)
{
	t_list	*tokens;
	int		has_pipe;
	char	*token;
	t_cmd	*cmd;

	tokens = shell->tokens;
	has_pipe = 0;
	if (tokens == NULL)
		return ;
	while (tokens)
	{
		token = tokens->content;
		if (strcmp(token, "|") == 0)
		{
			has_pipe = 1;
			break ;
		}
		tokens = tokens->next;
	}
	if (has_pipe)
		execute_pipeline(shell);
	else
	{
		cmd = parse_command(shell->tokens);
		if (cmd)
		{
			execute_simple_command(cmd, shell);
			free_cmd(cmd);
		}
	}
}
