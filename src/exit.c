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

int	ft_isspace(int c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r')
		return (1);
	return (0);
}

long long	ft_atoll(const char *str)
{
	long long	num;
	int			sign;

	num = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		num = num * 10 + *str - '0';
		str++;
	}
	return (num * sign);
}

int	is_out_of_range(const char *arg)
{
	long long	num;

	num = ft_atoll(arg);
	if (num > LONG_MAX || num < LONG_MIN)
		return (1);
	return (0);
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
	if (!is_numeric_argument(arg) || is_out_of_range(arg))
	{
		print_error_message(arg, STDERR_FILENO);
		shell->exit = 255;
		return (255);
	}
	shell->exit = ft_atoll(arg) % 256;
	return (shell->exit);
}
