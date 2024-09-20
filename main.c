/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:30:27 by lhopp             #+#    #+#             */
/*   Updated: 2024/09/16 19:33:15 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void write_prompt(void)
{
    write(1, "\033[0;32m", 7); // Set text color to green
    write(1, "minishell$ ", 11); // Print prompt
    write(1, "\033[0m", 4); // Reset text color to default
}


char *get_input(int fd)
{
	char *line;

	write_prompt();
	line = get_next_line(fd);
	// trim line
	// add line to history
	return (line);
}

int	main(int argc, char *argv[])
{
	char		*input;

	input = get_input(1);
	while (input && ft_strcmp(input, "exit") != 0) {
		// parse input
		free(input);
		input = get_next_line(1);
	}
	free(input);
	return (0);
}