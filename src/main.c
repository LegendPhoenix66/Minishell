/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:30:27 by lhopp             #+#    #+#             */
/*   Updated: 2024/10/09 15:34:09 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	process_command(char *input, t_shell *shell)
{
	parse_input(input, shell);
	ft_lstclear(&shell->tmp_tokens, free);
	ft_lstclear(&shell->tokens, free);
	ft_lstclear(&shell->tokens1, free);
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
	write(1, "exit\n", 5);
}

int	main(int argc, char **argv, char **env)
{
	t_shell	*args;
	int		exit_status;
	char	*input;

	(void)argc;
	(void)argv;
	set_upsignals();
	if (!(args = initialize_shell(env)))
		return (1);
	input = get_input();
	while (input && args->exit == -1)
	{
		process_command(input, args);
		if (args->exit != -1)
			break ;
		input = get_input();
	}
	if (args->exit == -1)
		args->exit = 0;
	exit_status = args->exit;
	cleanup_shell(args);
	return (exit_status);
}
