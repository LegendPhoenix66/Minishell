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

void builtin_exit(t_shell *args)
{
	t_list *command_arguments = args->tokens->next;

	// if multiple arguments are passed, print exit and an error message without changing the exit code to exit
	if (command_arguments && command_arguments->next) {
		printf("exit\n");
		printf("minishell: exit: too many arguments\n");
		return ;
	}
	// if only one argument is passed, check if it's a number (numbers above max long also count as not a numeric argument)
	if (command_arguments) {
		char *arg = command_arguments->content;
		int i = 0;
		while (arg[i]) {
			if (!ft_isdigit(arg[i])) {
				printf("exit\n");
				printf("minishell: exit: %s: numeric argument required\n", arg);
				args->exit = 255;
				return ;
			}
			i++;
		}
		args->exit = ft_atoi(arg) % 256;
	}
	else {
		args->exit = 0;
	}
}