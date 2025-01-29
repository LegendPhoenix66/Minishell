/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_red_and_pipe.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:36:15 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/29 11:00:41 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_token(t_cmd *cmd, t_list **tokens, int *status,
		t_shell *shell)
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
	{
		printf("handle token go to handle heredoc\n");
		*status = handle_heredoc1(cmd, tokens);
	}
	else
	{
		add_arg(cmd, token, shell);
		(*tokens)->content = token;
	}
	return (1);
}

static int	process_tokens(t_cmd *cmd, t_list **tokens, t_shell *shell)
{
	char	*token;
	int		status;

	while (*tokens)
	{
		token = (*tokens)->content;
		if (ft_strcmp(token, "|") == 0)
			break ;
		if (ft_strcmp(token, "<<") == 0)
			break ;
		if (!handle_token(cmd, tokens, &status, shell))
			return (0);
		if (!status)
			return (0);
		*tokens = (*tokens)->next;
	}
	return (1);
}

t_cmd	*parse_command(t_shell *shell, t_list *tokens)
{
	t_cmd	*cmd;

	if (!tokens)
		return (NULL);
	cmd = init_cmd();
	if (!cmd)
		return (NULL);
	if (!process_tokens(cmd, &tokens, shell))
	{
		free_cmd(cmd);
		return (NULL);
	}
	return (cmd);
}
