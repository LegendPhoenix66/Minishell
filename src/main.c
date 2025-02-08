/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:30:27 by lhopp             #+#    #+#             */
/*   Updated: 2025/01/27 14:31:14 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	process_command(char *input, t_shell *shell)
{
	parse_input(input, shell);
	if (shell->tokens)
		ft_lstclear(&shell->tokens, free);
	free(input);
}

void	cleanup_shell(t_shell *args)
{
	free_lst(args->env);
	ft_lstclear(&args->tokens, free);
	free(args->current_directory);
	if (args->export != NULL)
		free_lst(args->export);
	free(args);
	rl_clear_history();
	write(1, "exit\n", 5);
}

int	main(int argc, char **argv, char **env)
{
	t_shell	*shell;
	int		exit_status;
	char	*input;

	(void)argc;
	(void)argv;
	set_upsignals();
	shell = initialize_shell(env);
	if (!shell)
		return (1);
	input = get_input();
	while (input && shell->exit == -1)
	{
		process_command(input, shell);
		if (shell->exit != -1)
			break ;
		input = get_input();
	}
	if (shell->exit == -1)
		shell->exit = shell->last_status;
	exit_status = shell->exit;
	cleanup_shell(shell);
	return (exit_status);
}
