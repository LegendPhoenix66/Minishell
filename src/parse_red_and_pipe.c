/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_red_and_pipe.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:36:15 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/24 11:10:30 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_token(t_cmd *cmd, t_list **tokens, int *status,
		int last_status)
{
	char	*token;

	*status = 1;
	token = (*tokens)->content;
	if (ft_strcmp(token, "<") == 0)
		*status = handle_input(cmd, tokens);
	else if (ft_strcmp(token, ">") == 0)
		*status = handle_output(cmd, tokens);
	else if (ft_strcmp(token, ">>") == 0)
		*status = handle_append(cmd, tokens);
	else if (ft_strcmp(token, "<<") == 0)
		*status = handle_heredoc1(cmd, tokens);
	else
	{
		add_arg(cmd, token, last_status);
		(*tokens)->content = token;
	}
	return (1);
}

static int	process_tokens(t_cmd *cmd, t_list **tokens, int last_status)
{
	char	*token;
	int		status;

	while (*tokens)
	{
		token = (*tokens)->content;
		if (ft_strcmp(token, "|") == 0)
			break ;
		if (!handle_token(cmd, tokens, &status, last_status))
			return (0);
		if (!status)
			return (0);
		*tokens = (*tokens)->next;
	}
	return (1);
}

t_cmd	*parse_command(t_list *tokens, int last_status)
{
	t_cmd	*cmd;

	if (!tokens)
		return (NULL);
	cmd = init_cmd();
	if (!cmd)
		return (NULL);
	if (!process_tokens(cmd, &tokens, last_status))
	{
		free_cmd(cmd);
		return (NULL);
	}
	return (cmd);
}
