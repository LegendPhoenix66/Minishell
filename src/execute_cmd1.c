/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 16:10:33 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/30 21:23:15 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_builtin_type	get_builtin_type(const char *cmd)
{
	if (!cmd)
		return (BUILTIN_NONE);
	if (!ft_strncmp(cmd, "echo", ft_strlen(cmd) + 1))
		return (BUILTIN_ECHO);
	if (!ft_strncmp(cmd, "cd", ft_strlen(cmd) + 1))
		return (BUILTIN_CD);
	if (!ft_strncmp(cmd, "pwd", ft_strlen(cmd) + 1))
		return (BUILTIN_PWD);
	if (!ft_strncmp(cmd, "export", ft_strlen(cmd) + 1))
		return (BUILTIN_EXPORT);
	if (!ft_strncmp(cmd, "unset", ft_strlen(cmd) + 1))
		return (BUILTIN_UNSET);
	if (!ft_strncmp(cmd, "env", ft_strlen(cmd) + 1))
		return (BUILTIN_ENV);
	if (!ft_strncmp(cmd, "exit", ft_strlen(cmd) + 1))
		return (BUILTIN_EXIT);
	return (BUILTIN_NONE);
}

int	builtin_execution(t_cmd *cmd, t_shell *shell)
{
	t_builtin_type	type;
	int				ret;

	type = get_builtin_type(cmd->args[0]);
	ret = 0;
	if (type == BUILTIN_ECHO)
		ret = builtin_echo(cmd);
	else if (type == BUILTIN_CD)
		ret = builtin_cd(shell, cmd);
	else if (type == BUILTIN_PWD)
		ret = builtin_pwd(shell);
	else if (type == BUILTIN_EXPORT)
		ret = builtin_export(shell, cmd->args);
	else if (type == BUILTIN_UNSET)
		ret = builtin_unset(&shell->env, cmd->args[1]);
	else if (type == BUILTIN_ENV)
		print_lst(&shell->env);
	else if (type == BUILTIN_EXIT)
		builtin_exit(shell, cmd);
	else
		ret = 1;
	return (ret);
}

int	opening_mode(t_cmd *cmd)
{
	int	flags;

	if (cmd->output_mode == 1)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	return (flags);
}

int	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	int	ret;
	int	saved_stdout;
	int	fd;
	int	flags;

	ret = 0;
	saved_stdout = -1;
	if (cmd->output_mode)
	{
		saved_stdout = dup(STDOUT_FILENO);
		flags = opening_mode(cmd);
		fd = open(cmd->output_file, flags, 0644);
		if (fd != -1)
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
	}
	ret = builtin_execution(cmd, shell);
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	return (ret);
}

void	handle_io_redirection(t_cmd *cmd)
{
	if (cmd->input_mode == 1 && cmd->input_file)
	{
		if (setup_redirection(cmd, 1, STDIN_FILENO) == -1)
			exit(1);
	}
	if (cmd->output_mode)
	{
		if (setup_redirection(cmd, 2, STDOUT_FILENO) == -1)
			exit(1);
	}
}
