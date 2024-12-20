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

void	ft_strtrim1(char *str)
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

	if (isatty(fileno(stdin)))
	{
		prompt = get_prompt();
		line = readline(prompt);
		free(prompt);
	}
	else
	{
		line = get_next_line(fileno(stdin));
		char *trimmed_line = ft_strtrim(line, "\n");
		free(line);
		line = trimmed_line;
	}
	if (line == NULL)
	{
		return (NULL);
	}
	ft_strtrim1(line);
	if (*line)
		add_history(line);
	return (line);
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
}

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_shell  *args;

	(void)argc;
	(void)argv;
	args = malloc(sizeof(t_shell));
	if (!args)
		return (1);
	init_args(args);
	args->environ = env;
	input = get_input();
	while (input && args->exit == -1)
	{
		parse_input(input, args);
		ft_lstclear(&args->tmp_tokens, free);
		ft_lstclear(&args->tokens, free);
		ft_lstclear(&args->tokens1, free);
		free(input);
		if (args->exit != -1)
			break ;
		input = get_input();
	}
	if (args->exit == -1)
		args->exit = 0;
	free_lst(args->env);
	ft_lstclear(&args->tokens, free);
	write(1, "exit\n", 5);
	return (args->exit);
}
