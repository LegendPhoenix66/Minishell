/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 15:50:15 by lhopp             #+#    #+#             */
/*   Updated: 2024/12/01 15:50:20 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	builtin_echo(t_cmd *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (cmd->args[i] && !strcmp(cmd->args[i], "-n"))
	{
		newline = 0;
		i++;
	}
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

int	builtin_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		return (1);
	}
	ft_putendl_fd(pwd, STDOUT_FILENO);
	free(pwd);
	return (0);
}

int	builtin_cd(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (1);
	if (!cmd->args[1] || !strcmp(cmd->args[1], "~"))
		path = getenv("HOME");
	else
		path = cmd->args[1];
	if (chdir(path) != 0)
	{
		free(old_pwd);
		return (1);
	}
	free(shell->current_directory);
	shell->current_directory = getcwd(NULL, 0);
	free(old_pwd);
	return (0);
}
