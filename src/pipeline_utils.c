/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:47:26 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/15 10:47:26 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    initialize_pipeline_data(t_p *data)
{
    data->input_fd = STDIN_FILENO;
    data->is_last_command = 0;
    data->saved_stdin = -1;
    data->fd = -1;
    data->exec_path = NULL;
    data->ret = 0;
    data->status = 0;
}

void    process_tokens1(t_list **tokens, t_p *data)
{
    while (*tokens && ft_strcmp((*tokens)->content, "|") != 0)
        *tokens = (*tokens)->next;
    data->is_last_command = (*tokens == NULL);
    if (!data->is_last_command)
        *tokens = (*tokens)->next;
}

int    create_pipe(t_p *data, t_cmd *cmd)
{
    if (pipe(data->pipefd) == -1)
    {
        perror("pipe failed");
        free_cmd(cmd);
        exit(EXIT_FAILURE);
        return (0);
    }
    return (1);
}

void    handle_parent_process(t_p *data, t_shell *shell)
{
    close(data->pipefd[1]);
    if (data->input_fd != STDIN_FILENO)
        close(data->input_fd);
    data->input_fd = data->pipefd[0];
    waitpid(data->pid, &data->status, 0);
    if (data->is_last_command)
    {
        if (WIFEXITED(data->status))
            shell->last_status = WEXITSTATUS(data->status);
        else if (WIFSIGNALED(data->status))
            shell->last_status = 128 + WTERMSIG(data->status);
        else
            shell->last_status = 1;
    }
}