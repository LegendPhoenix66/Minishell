/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:18:30 by drenquin          #+#    #+#             */
/*   Updated: 2025/02/07 20:21:02 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_builtin(char *token)
{
	char	*builtins[7];
	size_t	i;

	i = 0;
	builtins[0] = "exit";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "echo";
	builtins[4] = "env";
	builtins[5] = "unset";
	builtins[6] = "export";
	if (!token)
		return (0);
	while (i < sizeof(builtins) / sizeof(builtins[0]))
	{
		if (ft_strcmp(token, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (cmd == NULL)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->input_mode = 0;
	cmd->output_mode = 0;
	return (cmd);
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd != NULL)
	{
		if (cmd->args != NULL)
		{
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
		}
		if (cmd->input_file)
			free(cmd->input_file);
		if (cmd->output_file)
			free(cmd->output_file);
		free(cmd);
	}
}
