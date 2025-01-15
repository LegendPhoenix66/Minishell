/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 19:33:28 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/14 19:33:49 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    handle_builtin(t_cmd *cmd, t_shell *shell, t_p *data)
{
    if (data->input_fd != STDIN_FILENO)
    {
        data->saved_stdin = dup(STDIN_FILENO);
        dup2(data->input_fd, STDIN_FILENO);
        execute_builtin(cmd, shell);
        dup2(data->saved_stdin, STDIN_FILENO);
        close(data->saved_stdin);
        close(data->input_fd);
    }
    else
        execute_builtin(cmd, shell);
}

void    handle_pipeline_execution(t_p *data, t_cmd *cmd, t_shell *shell)
{
    data->pid = fork();
    if (data->pid == 0)
        handle_child_process(data, cmd, shell);
    else if (data->pid > 0)
        handle_parent_process(data, shell);
    else
    {
        perror("fork failed");
        free_cmd(cmd);
        exit(EXIT_FAILURE);
    }
}

t_cmd   *process_command1(t_p *data, t_list **tokens, t_shell *shell)
{
    t_cmd   *cmd;

    cmd = parse_command(*tokens);
    if (!cmd)
        return (NULL);
    process_tokens1(tokens, data);
    if (data->is_last_command && is_builtin(cmd->args[0]))
    {
        handle_builtin(cmd, shell, data);
        free_cmd(cmd);
        return (NULL);
    }
    return (cmd);
}

void    execute_pipeline(t_shell *shell)
{
    t_p     data;
    t_list  *tokens;
    t_cmd   *cmd;

    initialize_pipeline_data(&data);
    tokens = shell->tokens;
    while (tokens)
    {
        cmd = process_command1(&data, &tokens, shell);
        if (cmd == NULL)
            break;
        if (!create_pipe(&data, cmd))
            continue;
        handle_pipeline_execution(&data, cmd, shell);
        free_cmd(cmd);
        if (data.is_last_command)
            break;
    }
    if (data.input_fd != STDIN_FILENO)
        close(data.input_fd);
}
