/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 19:55:46 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/06 19:55:46 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void handle_sigint(int sig)
{
	(void)sig; // Suppress unused parameter warning
	write(1, "\n", 1); // Print a newline
	rl_on_new_line();  // Tell readline to move to a new line
	rl_replace_line("", 0); // Clear the current line buffer
	rl_redisplay();    // Redisplay the prompt
}
void set_upsignals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
void init_args(t_shell *args)
{
	args->env = init_lst();
	args->exit = -1;
	args->current_directory = getcwd(NULL, 0);
	args->tokens = NULL;
	args->export = NULL;
	args->tmp_tokens = NULL;
	args->tokens1 = NULL;
	args->redirections = NULL;
	args->last_status = 0;
}
t_shell *initialize_shell(char **env)
{
	t_shell *args;

	args = malloc(sizeof(t_shell));
	if(!args)
		return (NULL);
	init_args(args);
	args->environ = env;
	return (args);
}
