/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 15:50:15 by lhopp             #+#    #+#             */
/*   Updated: 2025/01/21 10:29:25 by lhopp            ###   ########.fr       */
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

static int	save_old_pwd(char **old_pwd)
{
	*old_pwd = getcwd(NULL, 0);
	return (*old_pwd != NULL);
}

static int	check_cd_args(t_cmd *cmd, char *old_pwd, t_shell *shell)
{
	if (cmd->args[2])
	{
		ft_putendl_fd("cd: too many arguments", STDERR_FILENO);
		free(old_pwd);
		shell->last_status = 1;
		return (1);
	}
	return (0);
}

int	builtin_cd(t_shell *shell, t_cmd *cmd)
{
	char	*old_pwd;
	char	*path;

	if (!save_old_pwd(&old_pwd))
		return (set_error(shell, 1));
	if (check_cd_args(cmd, old_pwd, shell))
		return (1);
	path = get_cd_path(cmd);
	if (execute_cd(path, old_pwd, shell))
		return (1);
	update_directory(shell, old_pwd);
	return (0);
}
