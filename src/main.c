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

#include "../include/minishell.h"

char *get_prompt(void)
{
	char *prompt;

	prompt = malloc(20); // Allocate memory for the prompt
	if (!prompt)
		return (NULL);
	ft_strlcpy(prompt, "\033[0;32mminishell> \033[0m", 20); // Copy the prompt to the allocated memory
	return (prompt);
}

void ft_strtrim(char *str)
{
	int i = 0;
	int j = 0;

	// Skip leading whitespaces
	while (str[i] == ' ' || str[i] == '\t')
		i++;

	// Remove extra whitespaces in between words
	while (str[i]) {
		if (!(str[i] == ' ' || str[i] == '\t' || str[i] == '\n') ||
			(j > 0 && !(str[j - 1] == ' ' || str[j - 1] == '\t' || str[i] == '\n'))) {
			str[j++] = str[i];
		}
		i++;
	}
	str[j] = '\0';

	// Remove trailing whitespaces
	j--;
	while (j >= 0 && (str[j] == ' ' || str[j] == '\t')) {
		str[j] = '\0';
		j--;
	}
}

char *get_input()
{
	char *line;
	char *prompt;

	prompt = get_prompt();
	line = readline(prompt);
	free(prompt);
	if (line == NULL) // Handle ctrl-D (EOF)
	{
		write(1, "exit\n", 5); // Display "exit" on ctrl-D like bash
		return (NULL);
	}
	// trim line
	ft_strtrim(line);
	// add line to history (if it's not an empty line)
	if (*line)
		add_history(line);
	return (line);
}

int main(int argc, char *argv[])
{
	char *input;

	input = get_input();
	while (input && ft_strncmp(input, "exit", 4) != 0) {
		// parse input
		free(input);
		input = get_input();
	}
	free(input);
	return (0);
}