/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:29:46 by lhopp             #+#    #+#             */
/*   Updated: 2024/02/27 13:57:37 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int			word_count;
	const char	*word_start = s;

	word_count = 0;
	while (*s)
	{
		if (*s == c)
		{
			if (s != word_start)
			{
				word_count++;
			}
			word_start = s + 1;
		}
		s++;
	}
	if (*word_start)
	{
		word_count++;
	}
	return (word_count);
}

static char	**allocate_words(int word_count)
{
	char	**words;

	words = malloc((word_count + 1) * sizeof(char *));
	if (!words)
	{
		return (NULL);
	}
	return (words);
}

static void	create_substring(char **wordsarray, size_t *word_index,
	const char **word_start, size_t word_length)
{
	if (word_length > 0)
	{
		wordsarray[*word_index] = ft_substr(*word_start, 0, word_length);
		(*word_index)++;
	}
}

static void	fill_words(char **wordsarray, char const *string, char c)
{
	size_t		word_index;
	const char	*word_start = string;
	size_t		word_length;

	word_index = 0;
	word_length = 0;
	while (*string)
	{
		if (*string == c)
		{
			create_substring(wordsarray, &word_index, &word_start, word_length);
			word_start = string + 1;
			word_length = 0;
		}
		else
		{
			word_length++;
		}
		string++;
	}
	create_substring(wordsarray, &word_index, &word_start, word_length);
	wordsarray[word_index] = NULL;
}

char	**ft_split(char const *string, char c)
{
	int		word_count;
	char	**words;

	if (!string)
	{
		return (NULL);
	}
	word_count = count_words(string, c);
	words = allocate_words(word_count);
	if (!words)
	{
		return (NULL);
	}
	fill_words(words, string, c);
	return (words);
}
