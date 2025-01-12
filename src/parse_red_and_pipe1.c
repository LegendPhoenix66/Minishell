/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_red_and_pipe1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 21:23:24 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/10 21:23:26 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_input(t_cmd *cmd, t_list **tokens)
{
	*tokens = (*tokens)->next;
	if (*tokens == NULL)
	{
		perror("Error: Missing filename after <\n");
		return (0);
	}
	cmd->input_file = ft_strdup((*tokens)->content);
	if (!cmd->input_file)
	{
		perror("strdup failed");
		return (0);
	}
	cmd->input_mode = 1;
	return (1);
}

int	handle_output(t_cmd *cmd, t_list **tokens)
{
	*tokens = (*tokens)->next;
	if (*tokens == NULL)
	{
		perror("Error: Missing filename after >\n");
		return (0);
	}
	cmd->output_file = ft_strdup((*tokens)->content);
	if (!cmd->output_file)
	{
		perror("strdup failed");
		return (0);
	}
	cmd->output_mode = 1;
	return (1);
}

int	handle_append(t_cmd *cmd, t_list **tokens)
{
	*tokens = (*tokens)->next;
	if (*tokens == NULL)
	{
		perror("Error: Missing filename after >>\n");
		return (0);
	}
	cmd->output_file = ft_strdup((*tokens)->content);
	if (!cmd->output_file)
	{
		perror("strdup failed");
		return (0);
	}
	cmd->output_mode = 2;
	return (1);
}

int	handle_heredoc1(t_cmd *cmd, t_list **tokens)
{
	*tokens = (*tokens)->next;
	if (*tokens == NULL)
	{
		perror("Error: Missing delimiter after <<\n");
		return (0);
	}
	cmd->input_mode = 2;
	handle_heredoc(cmd, (*tokens)->content);
	return (1);
}
