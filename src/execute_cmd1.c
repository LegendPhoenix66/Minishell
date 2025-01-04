/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 16:10:33 by drenquin          #+#    #+#             */
/*   Updated: 2024/12/31 16:10:33 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_builtin_type get_builtin_type(const char *cmd)
{
    if (!cmd)
        return BUILTIN_NONE;
    if (!strcmp(cmd, "echo"))
        return BUILTIN_ECHO;
    if (!strcmp(cmd, "cd"))
        return BUILTIN_CD;
    if (!strcmp(cmd, "pwd"))
        return BUILTIN_PWD;
    if (!strcmp(cmd, "export"))
        return BUILTIN_EXPORT;
    if (!strcmp(cmd, "unset"))
        return BUILTIN_UNSET;
    if (!strcmp(cmd, "env"))
        return BUILTIN_ENV;
    if (!strcmp(cmd, "exit"))
        return BUILTIN_EXIT;
    return BUILTIN_NONE;
}

/*void execute_builtin(t_cmd *cmd, t_shell *shell)
{
    if (strcmp(cmd->args[0], "env") == 0)
    {
        // Gérer les redirections pour env
        if (cmd->output_mode)
        {
            int saved_stdout = dup(STDOUT_FILENO);
            int fd = open(cmd->output_file, cmd->output_mode == 1 ? O_WRONLY | O_CREAT | O_TRUNC : O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd != -1)
            {
                dup2(fd, STDOUT_FILENO);
                print_lst(&shell->env);
                close(fd);
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdout);
            }
        }
        else
            print_lst(&shell->env);
    }
    else if (strcmp(cmd->args[0], "export") == 0)
    {
        // Gérer les redirections pour export
        if (cmd->output_mode)
        {
            int saved_stdout = dup(STDOUT_FILENO);
            int fd = open(cmd->output_file, 
                         cmd->output_mode == 1 ? O_WRONLY | O_CREAT | O_TRUNC : O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd != -1)
            {
                dup2(fd, STDOUT_FILENO);
                builtin_export(shell, cmd->args);
                close(fd);
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdout);
            }
        }
        else
            builtin_export(shell, cmd->args);
    }
    else if (strcmp(cmd->args[0], "unset") == 0 && cmd->args[1])
        builtin_unset(&shell->env, cmd->args[1]);
}*/
int execute_builtin(t_cmd *cmd, t_shell *shell)
{
    t_builtin_type type = get_builtin_type(cmd->args[0]);
    int ret = 0;
    int saved_stdout = -1;
    //int saved_stdin = -1;
    
    // Handle redirections
    if (cmd->output_mode)
    {
        saved_stdout = dup(STDOUT_FILENO);
        int fd = open(cmd->output_file, 
            cmd->output_mode == 1 ? O_WRONLY | O_CREAT | O_TRUNC : O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd != -1)
        {
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
    }

    // Execute appropriate builtin using if-else chain
    if (type == BUILTIN_ECHO)
        ret = builtin_echo(cmd);
    else if (type == BUILTIN_CD)
        ret = builtin_cd(shell, cmd);
    else if (type == BUILTIN_PWD)
        ret = builtin_pwd();
    else if (type == BUILTIN_EXPORT)
        ret = builtin_export(shell, cmd->args);
    else if (type == BUILTIN_UNSET)
        ret = builtin_unset(&shell->env, cmd->args[1]);
    else if (type == BUILTIN_ENV)
        print_lst(&shell->env);
    else if (type == BUILTIN_EXIT)
        builtin_exit(shell, cmd);
    else
        ret = 1;

    // Restore redirections
    if (saved_stdout != -1)
    {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }

    return ret;
}
