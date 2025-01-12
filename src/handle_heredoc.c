/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:43:38 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/10 16:44:17 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void handle_heredoc_parent(t_cmd *cmd, int pipe_fd[2], pid_t pid)
{
    int status;

    close(pipe_fd[1]);
    cmd->input_fd = pipe_fd[0];
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        perror("Heredoc process ended with error\n");
}
int process_heredoc_line(char *line, char *delimiter, int fd)
{
    size_t len;

    len = ft_strlen(line);
    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';
    if (ft_strcmp(line, delimiter) == 0)
    {
        free(line);
        return (1);
    }
    write_heredoc_line(fd, line);
    free(line);
    return (0);
}

void handle_heredoc_child(int pipe_fd[2], char *delimiter)
{
    close(pipe_fd[0]);
    process_heredoc_input(pipe_fd[1], delimiter);
    close(pipe_fd[1]);
    exit(0);
}
void    handle_heredoc(t_cmd *cmd, char *delimiter)
{
    int     pipe_fd[2];
    pid_t   pid;

    if (setup_heredoc_pipe(pipe_fd) == -1)
        return;

    pid = fork();
    if (pid == 0)
        handle_heredoc_child(pipe_fd, delimiter);
    else if (pid > 0)
        handle_heredoc_parent(cmd, pipe_fd, pid);
}
