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

#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[0;32m"

size_t	calculate_size(char **elements)
{
	int		i;
	size_t	size;

	i = 0;
	size = 0;
	while (elements[i])
	{
		size += ft_strlen(elements[i]);
		i++;
	}
	return ((size) + 1);
}

char	*get_prompt(void)
{
	char	*prompt;
	char	*elements[] = {COLOR_GREEN, "minishell> ", COLOR_RESET, NULL};
	size_t	size;
	int		i;

	size = calculate_size(elements);
	prompt = malloc(size);
		// Allocate enough memory for the prompt including escape sequences
	if (!prompt)
		return (NULL);
	i = 1;
	ft_strlcpy(prompt, elements[0], size);
	while (elements[i])
	{
		ft_strlcat(prompt, elements[i], size);
		i++;
	}
	return (prompt);
}

void	ft_strtrim(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	// Skip leading whitespaces
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	// Remove extra whitespaces in between words
	while (str[i])
	{
		if (!(str[i] == ' ' || str[i] == '\t' || str[i] == '\n') || (j > 0
				&& !(str[j - 1] == ' ' || str[j - 1] == '\t'
					|| str[i] == '\n')))
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

char	*get_input(void)
{
	char	*line;
	char	*prompt;

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

int	main(void)
{
	char	*input;

	input = get_input();
	while (input && ft_strncmp(input, "exit", 4) != 0)
	{
		parse_input(input);
		free(input);
		input = get_input();
	}
	free(input);
	return (0);
}
