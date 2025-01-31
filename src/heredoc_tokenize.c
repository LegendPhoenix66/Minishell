/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_tokenize.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:36:11 by lhopp             #+#    #+#             */
/*   Updated: 2025/01/31 12:36:32 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	find_var_end(const char *str, int start)
{
	int	i;

	i = start + 1;
	while (str[i] && str[i] != ' ' && str[i] != '$' && str[i] != '\''
		&& str[i] != '"')
		i++;
	return (i);
}

static char	*get_next_token(const char *input, int *i, int len)
{
	int	start;

	start = *i;
	if (input[*i] == '$')
		*i = find_var_end(input, *i);
	else
		while (*i < len && input[*i] != '$')
			(*i)++;
	return (ft_substr(input, start, *i - start));
}

static void	free_tokens(char **tokens, int token_count)
{
	int	j;

	j = 0;
	while (j < token_count)
	{
		free(tokens[j]);
		j++;
	}
	free(tokens);
}

static char	**allocate_and_tokenize(const char *input, int len,
		int *token_count)
{
	char	**tokens;
	int		i;
	char	*token;

	tokens = malloc((len + 1) * sizeof(char *));
	if (!tokens)
		return (NULL);
	i = 0;
	while (i < len)
	{
		token = get_next_token(input, &i, len);
		if (!token)
		{
			free_tokens(tokens, *token_count);
			return (NULL);
		}
		tokens[(*token_count)++] = token;
	}
	return (tokens);
}

char	**tokenize_input_test(const char *input)
{
	int		len;
	int		token_count;
	char	**tokens;

	len = strlen(input);
	token_count = 0;
	tokens = allocate_and_tokenize(input, len, &token_count);
	if (tokens)
	{
		tokens[token_count] = NULL;
	}
	return (tokens);
}
