/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 18:23:35 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/21 10:29:07 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_cd_path(t_cmd *cmd)
{
	if (!cmd->args[1] || !ft_strcmp(cmd->args[1], "~"))
		return (getenv("HOME"));
	return (cmd->args[1]);
}

int	execute_cd(char *path, char *old_pwd, t_shell *shell)
{
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
	return (0);
}

void	update_directory(t_shell *shell, char *old_pwd)
{
	free(shell->current_directory);
	shell->current_directory = getcwd(NULL, 0);
	free(old_pwd);
	shell->last_status = 0;
}

int	set_error(t_shell *shell, int status)
{
	shell->last_status = status;
	return (status);
}
