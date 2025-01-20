/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:51:30 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/20 13:51:30 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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