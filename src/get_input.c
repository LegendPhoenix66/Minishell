/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 18:53:29 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/06 18:53:29 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	char	*elements[4];
	size_t	size;
	int		i;

	elements[0] = COLOR_GREEN;
	elements[1] = "minishell> ";
	elements[2] = COLOR_RESET;
	elements[3] = NULL;
	size = calculate_size(elements);
	prompt = malloc(size);
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

char	*get_input(void)
{
	char	*line;
	char	*prompt;
	char	*trimmed_line;

	if (isatty(STDIN_FILENO))
	{
		prompt = get_prompt();
		line = readline(prompt);
		free(prompt);
	}
	else
	{
		line = get_next_line(STDIN_FILENO);
		trimmed_line = ft_strtrim(line, "\n");
		free(line);
		line = trimmed_line;
	}
	if (line == NULL)
		return (NULL);
	trim_and_remove_whitespace(line);
	if (*line)
		add_history(line);
	return (line);
}
