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

char *get_prompt(void)
{
    char *prompt;

    prompt = malloc(20); // Allocate memory for the prompt
    if (!prompt)
        return (NULL);
    strcpy(prompt, "\033[0;32mminishell$ \033[0m"); // Construct the prompt
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
    while (str[i])
    {
        if (!(str[i] == ' ' || str[i] == '\t' || str[i] == '\n') || (j > 0 && !(str[j - 1] == ' ' || str[j - 1] == '\t' || str[i] == '\n')))
        {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0';

    // Remove trailing whitespaces
    j--;
    while (j >= 0 && (str[j] == ' ' || str[j] == '\t'))
    {
        str[j] = '\0';
        j--;
    }
}

char *get_input(int fd)
{
	char *line;

	line = readline(get_prompt());
	// trim line
	ft_strtrim(line);
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