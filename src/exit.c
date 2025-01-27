/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:50:16 by lhopp             #+#    #+#             */
/*   Updated: 2024/12/03 21:50:16 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_numeric_argument(const char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

void	print_error_message(char *arg, int fd)
{
	ft_putstr_fd("minishell: exit: ", fd);
	ft_putstr_fd(arg, fd);
	ft_putstr_fd(": numeric argument required\n", fd);
}

int	builtin_exit(t_shell *shell, t_cmd *cmd)
{
	char	*arg;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!cmd->args[1])
	{
		shell->exit = 0;
		return (0);
	}
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	arg = cmd->args[1];
	if (!is_numeric_argument(arg))
	{
		print_error_message(arg, STDERR_FILENO);
		shell->exit = 255;
		return (255);
	}
	shell->exit = ft_atoi(arg) % 256;
	return (shell->exit);
}
