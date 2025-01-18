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

static int	skip_leading_whitespace(const char *str)
{
	int	index;

	index = 0;
	while (str[index] == ' ' || str[index] == '\t')
		index++;
	return (index);
}

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

void	trim_and_remove_whitespace(char *str)
{
	int		read_pos;
	int		write_pos;
	char	quote;

	read_pos = skip_leading_whitespace(str);
	write_pos = 0;
	quote = '\0';
	while (str[read_pos])
	{
		if (str[read_pos] == '\'' || str[read_pos] == '\"')
		{
			if (quote == '\0')
				quote = str[read_pos];
			else if (quote == str[read_pos])
				quote = '\0';
		}
		if (quote || !is_whitespace(str[read_pos]) || (write_pos > 0
				&& !is_whitespace(str[write_pos - 1])))
			str[write_pos++] = str[read_pos];
		read_pos++;
	}
	str[write_pos] = '\0';
	write_pos--;
	while (write_pos >= 0 && is_whitespace(str[write_pos]))
		str[write_pos--] = '\0';
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
