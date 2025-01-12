/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 23:17:58 by lhopp             #+#    #+#             */
/*   Updated: 2024/12/14 23:18:04 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	execute_simple_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;
	int		fd;
	char	*exec_path;

	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, shell);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		if (cmd->input_mode == 1)
		{
			fd = open(cmd->input_file, O_RDONLY);
			if (fd == -1)
			{
				perror("Error opening input file");
				free_cmd(cmd);
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (cmd->input_mode == 2)
		{
			if (cmd->input_fd > 0)
			{
				dup2(cmd->input_fd, STDIN_FILENO);
				close(cmd->input_fd);
			}
		}
		if (cmd->output_mode == 1)
		{
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror("Error opening output file");
				free_cmd(cmd);
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (cmd->output_mode == 2)
		{
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror("Error opening output file");
				free_cmd(cmd);
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		exec_path = find_executable(cmd->args[0]);
		if (!exec_path)
		{
			fprintf(stderr, "minishell: command not found: %s\n", cmd->args[0]);
			free_cmd(cmd);
			status = 127;
		}
		execve(exec_path, cmd->args, shell->environ);
		perror("execve failed");
		free(exec_path);
		free_cmd(cmd);
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			shell->last_status = WEXITSTATUS(status);
		}
		else if (WIFSIGNALED(status))
		{
			shell->last_status = 128 + WTERMSIG(status);
		}
		else
		{
			shell->last_status = 1;
		}
	}
	else
	{
		perror("fork failed");
		shell->last_status = 1;
		free_cmd(cmd);
		exit(EXIT_FAILURE);
	}
}

void execute_pipeline(t_shell *shell)
{
    int     pipefd[2];
    pid_t   pid;
    int     status;
    int     input_fd;
    int     is_last_command;
    t_list  *tokens;
    t_cmd   *cmd;
    int     saved_stdin;
    int     fd;
    char    *exec_path;

    input_fd = STDIN_FILENO;
    is_last_command = 0;
    tokens = shell->tokens;

    while (tokens)
    {
        cmd = parse_command(tokens);
        if (cmd == NULL)
            break;
        while (tokens && strcmp(tokens->content, "|") != 0)
            tokens = tokens->next;
        is_last_command = (tokens == NULL);
        if (!is_last_command)
            tokens = tokens->next;
        if (is_last_command && is_builtin(cmd->args[0]))
        {
            if (input_fd != STDIN_FILENO)
            {
                saved_stdin = dup(STDIN_FILENO);
                dup2(input_fd, STDIN_FILENO);
                execute_builtin(cmd, shell);
                dup2(saved_stdin, STDIN_FILENO);
                close(saved_stdin);
                close(input_fd);
            }
            else
                execute_builtin(cmd, shell);
            free_cmd(cmd);
            break;
        }

        if (pipe(pipefd) == -1)
        {
            perror("pipe failed");
            free_cmd(cmd);
            exit(EXIT_FAILURE);
        }
        pid = fork();
        if (pid == 0)
        {
            dup2(input_fd, STDIN_FILENO);
            if (!is_last_command)
                dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
            if (cmd->input_mode == 1)
            {
                fd = open(cmd->input_file, O_RDONLY);
                if (fd == -1)
                {
                    perror("Error opening input file");
                    free_cmd(cmd);
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            if (cmd->output_mode)
            {
                fd = open(cmd->output_file, 
                         cmd->output_mode == 1 ? O_WRONLY | O_CREAT | O_TRUNC 
                                             : O_WRONLY | O_CREAT | O_APPEND,
                         0644);
                if (fd == -1)
                {
                    perror("Error opening output file");
                    free_cmd(cmd);
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
			if (is_builtin(cmd->args[0]))
            {
                int ret = execute_builtin(cmd, shell);
                free_cmd(cmd);
                exit(ret);
            }
            exec_path = find_executable(cmd->args[0]);
            if (!exec_path)
            {
                free_cmd(cmd);
                exit(127);
            }
            execve(exec_path, cmd->args, shell->environ);
            perror("execve failed");
            free(exec_path);
            free_cmd(cmd);
            exit(EXIT_FAILURE);
        }
        else if (pid > 0)
        {
            close(pipefd[1]);
            if (input_fd != STDIN_FILENO)
                close(input_fd);
            input_fd = pipefd[0];

            waitpid(pid, &status, 0);
            if (is_last_command)
            {
                if (WIFEXITED(status))
                    shell->last_status = WEXITSTATUS(status);
                else if (WIFSIGNALED(status))
                    shell->last_status = 128 + WTERMSIG(status);
                else
                    shell->last_status = 1;
            }
        }
        else
        {
            perror("fork failed");
            free_cmd(cmd);
            exit(EXIT_FAILURE);
        }
        free_cmd(cmd);
        if (is_last_command)
            break;
    }
    if (input_fd != STDIN_FILENO)
    {
        close(input_fd);
    }
}

void	execute_command1(t_shell *shell)
{
	t_list	*tokens;
	int		has_pipe;
	char	*token;
	t_cmd	*cmd;

	tokens = shell->tokens;
	has_pipe = 0;
	if (tokens == NULL)
		return ;
		
	while (tokens)
	{
		token = tokens->content;
		if (strcmp(token, "|") == 0)
		{
			has_pipe = 1;
			break ;
		}
		tokens = tokens->next;
	}
	if (has_pipe)
		execute_pipeline(shell);
	else
	{
		cmd = parse_command(shell->tokens);
		if (cmd)
		{
			execute_simple_command(cmd, shell);
			free_cmd(cmd);
		}
	}
}
