/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 19:58:52 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/10 19:58:55 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_strcmp(char *str1, char *str2)
{
	int	i;

	i = 0;
	while (str1[i] == str2[i] && str1[i] && str2[i])
		i++;
	return (str1[i] - str2[i]);
}

int	setup_heredoc_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe failed");
		return (-1);
	}
	return (0);
}

void	process_heredoc_input(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		write(1, ">", 2);
		line = get_next_line(0);
		if (!line)
		{
			close(fd);
			exit(0);
		}
		if (process_heredoc_line(line, delimiter, fd))
			break ;
	}
}

void	write_heredoc_line(int fd, const char *line)
{
	if (write(fd, line, ft_strlen(line)) == -1)
	{
		perror("write error");
		exit(1);
	}
	write(fd, "\n", 1);
}
