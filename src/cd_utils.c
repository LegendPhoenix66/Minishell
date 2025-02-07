/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 18:23:35 by drenquin          #+#    #+#             */
/*   Updated: 2025/02/07 20:21:06 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_cd_path(t_shell *shell, t_cmd *cmd)
{
	if (!cmd->args[1] || !ft_strcmp(cmd->args[1], "~"))
		return (get_node(&shell->env, "HOME"));
	return (cmd->args[1]);
}

int	execute_cd(char *path, t_shell *shell)
{
	if (access(path, F_OK) != 0)
	{
		ft_putendl_fd("cd: no such file or directory", STDERR_FILENO);
		shell->last_status = 1;
		return (1);
	}
	if (chdir(path) != 0)
	{
		shell->last_status = 1;
		return (1);
	}
	return (0);
}

void	update_directory(t_shell *shell)
{
	char	*oldpwd;
	char	*pwd;

	remove_if(&shell->env, "OLDPWD");
	oldpwd = ft_strjoin("OLDPWD=", shell->current_directory);
	add_node(&shell->env, oldpwd);
	free(oldpwd);
	free(shell->current_directory);
	shell->current_directory = getcwd(NULL, 0);
	remove_if(&shell->env, "PWD");
	pwd = ft_strjoin("PWD=", shell->current_directory);
	add_node(&shell->env, pwd);
	free(pwd);
	shell->last_status = 0;
}
