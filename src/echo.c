/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 15:50:15 by lhopp             #+#    #+#             */
/*   Updated: 2025/01/16 14:38:58 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	builtin_echo(t_cmd *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (cmd->args[i] && !ft_strncmp(cmd->args[i], "-n", ft_strlen("-n") + 1))
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
	{
		shell->last_status = 1;
		return (1);
	}
	if (cmd->args[2])
	{
		ft_putendl_fd("cd: too many arguments", STDERR_FILENO);
		free(old_pwd);
		shell->last_status = 1;
		return (1);
	}
	if (!cmd->args[1] || !strcmp(cmd->args[1], "~"))
		path = getenv("HOME");
	else
		path = cmd->args[1];
	if (access(path, F_OK) != 0)
	{
		ft_putstr_fd("cd: no such file or directory", STDERR_FILENO);
		free(old_pwd);
		shell->last_status = 1;
		return (1);
	}
	if (chdir(path) != 0)
	{
		free(old_pwd);
		shell->last_status = 1;
		return (1);
	}
	free(shell->current_directory);
	shell->current_directory = getcwd(NULL, 0);
	free(old_pwd);
	shell->last_status = 0;
	return (0);
}
