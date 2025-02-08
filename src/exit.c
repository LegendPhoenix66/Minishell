/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:50:16 by lhopp             #+#    #+#             */
/*   Updated: 2025/02/07 20:15:32 by lhopp            ###   ########.fr       */
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
	const char	*long_max_str = "9223372036854775807";
	const char	*long_min_str = "9223372036854775808";
	int			sign;
	size_t		input_len;
	const char	*long_str;

	sign = 1;
	if (*arg == '-')
	{
		sign = -1;
		arg++;
	}
	else if (*arg == '+')
		arg++;
	input_len = ft_strlen(arg);
	if (sign == 1)
		long_str = long_max_str;
	else
		long_str = long_min_str;
	if (input_len > ft_strlen(long_str))
		return (1);
	if (input_len < ft_strlen(long_str))
		return (0);
	return (ft_strncmp(arg, long_str, input_len) > 0);
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
		shell->exit = 2;
		return (2);
	}
	shell->exit = (int)(ft_atoll(arg) % 256);
	return (shell->exit);
}
