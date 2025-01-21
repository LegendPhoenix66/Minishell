/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 12:19:16 by lhopp             #+#    #+#             */
/*   Updated: 2025/01/21 10:30:30 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	read_buffer(int fd, char **buffer)
{
	ssize_t	bytes_read;

	if (*buffer == NULL)
		*buffer = malloc(1025);
	if (*buffer == NULL)
		return (0);
	bytes_read = read(fd, *buffer, 1024);
	if (bytes_read <= 0)
	{
		free(*buffer);
		*buffer = NULL;
		return (0);
	}
	(*buffer)[bytes_read] = '\0';
	return (1);
}

char	*read_line_from_buffer(int fd, char **buffer, char **line)
{
	char const	*nlc;
	int			read_success;

	while (1)
	{
		if (buffer[fd] == NULL || ft_strlen(buffer[fd]) <= 0)
		{
			read_success = read_buffer(fd, &buffer[fd]);
			if (read_success == 0)
				return (*line);
		}
		nlc = ft_strchr(buffer[fd], '\n');
		if (nlc != NULL)
		{
			append_line(line, buffer[fd]);
			ft_memmove(buffer[fd], nlc + 1, ft_strlen(nlc));
			return (*line);
		}
		else
		{
			append_line(line, buffer[fd]);
			free(buffer[fd]);
			buffer[fd] = NULL;
		}
	}
}

char	*get_next_line(int fd)
{
	static char	*buffer[MAX_FD];
	char		*line;

	line = NULL;
	if (fd < 0 || fd >= MAX_FD)
		return (line);
	return (read_line_from_buffer(fd, buffer, &line));
}
