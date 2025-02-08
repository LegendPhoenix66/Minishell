/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:05:28 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/20 14:05:28 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	*ft_strcpy(char *dst, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*create_new_line(char **line, int length)
{
	char	*new_line;

	new_line = malloc(sizeof(char) * (length + 1));
	if (new_line == NULL)
		return (NULL);
	if (*line != NULL)
	{
		ft_strcpy(new_line, *line);
		free(*line);
	}
	return (new_line);
}

void	append_line(char **line, const char *buffer)
{
	int		i;
	int		j;
	int		old_length;
	char	*new_line;

	old_length = 0;
	i = 0;
	j = 0;
	while (buffer[i] != '\n' && buffer[i] != '\0')
		i++;
	if (buffer[i] == '\n')
		i++;
	if (*line != NULL)
		old_length = (int)ft_strlen(*line);
	new_line = create_new_line(line, old_length + i);
	if (new_line == NULL)
		return ;
	while (j < i)
	{
		new_line[old_length + j] = buffer[j];
		j++;
	}
	new_line[old_length + j] = '\0';
	*line = new_line;
}
