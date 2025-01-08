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

int	builtin_exit(t_shell *shell, t_cmd *cmd)
{
	char	*arg;
	int		i;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!cmd->args[1])
	{
		shell->exit = 0;
		return (0);
	}
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	arg = cmd->args[1];
	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			shell->exit = 255;
			return (255);
		}
		i++;
	}
	shell->exit = ft_atoi(arg) % 256;
	return (shell->exit);
}
