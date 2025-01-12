/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:18:30 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/10 16:18:50 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_builtin(const char *token)
{
	const char	*builtins[] = {"exit", "cd", "pwd", "echo", "env", "unset",
			"export"};
	size_t		i;

	i = 0;
	if (!token)
		return (0);
	while (i < sizeof(builtins) / sizeof(builtins[0]))
	{
		if (strcmp(token, builtins[i]) == 0)
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
	cmd->input_fd = 0;
	cmd->output_fd = 1;
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

void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		perror("malloc failled");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

void	add_arg(t_cmd *cmd, char *arg)
{
	int		i;
	int		j;
	char	**new_args;

	i = 0;
	j = 0;
	if (cmd->args == NULL)
	{
		cmd->args = safe_malloc(sizeof(char *) * 2);
		cmd->args[0] = ft_strdup(arg);
		cmd->args[1] = NULL;
		return ;
	}
	while (cmd->args[i] != NULL)
		i++;
	new_args = safe_malloc(sizeof(char *) * (i + 2));
	while (j < i)
	{
		new_args[j] = cmd->args[j];
		j++;
	}
	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
}
