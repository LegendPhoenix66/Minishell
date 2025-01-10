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

int	is_builtin(const char *token)
{
	const char	*builtins[] = {"exit", "cd", "pwd", "echo", "env", "unset",
		"export"};
	size_t		i;

	if (!token)
		return (0);
	for (i = 0; i < sizeof(builtins) / sizeof(builtins[0]); i++)
	{
		if (strcmp(token, builtins[i]) == 0)
			return (1);
	}
	return (0);
}

// Helper function to allocate a t_cmd struct
t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (cmd == NULL)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->input_fd = 0;
	cmd->output_fd = 1;
	cmd->input_mode = 0;
	cmd->output_mode = 0;
	return (cmd);
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd != NULL)
	{
		if (cmd->args != NULL)
		{
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
		}
		if (cmd->input_file)
			free(cmd->input_file);
		if (cmd->output_file)
			free(cmd->output_file);
		free(cmd);
	}
}

// Function to add an argument to the command
/*void	add_arg(t_cmd *cmd, char *arg)
{
	int		i;
	char	**new_args;

	i = 0;
	if (cmd->args == NULL)
	{
		cmd->args = malloc(sizeof(char *) * 2);
		if (cmd->args == NULL)
		{
			perror("malloc failed");
			exit(EXIT_FAILURE);
		}
		cmd->args[0] = ft_strdup(arg);
		cmd->args[1] = NULL;
		return ;
	}
	while (cmd->args[i] != NULL)
		i++;
	new_args = realloc(cmd->args, sizeof(char *) * (i + 2));
	if (new_args == NULL)
	{
		perror("realloc failed");
		exit(EXIT_FAILURE);
	}
	cmd->args = new_args;
	cmd->args[i] = ft_strdup(arg);
	cmd->args[i + 1] = NULL;
}*/
// same fonction but skip realloc
void    add_arg(t_cmd *cmd, char *arg)
{
    int     i;
	int		j;
    char    **new_args;

    i = 0;
	j = 0;
    if (cmd->args == NULL)
    {
        cmd->args = malloc(sizeof(char *) * 2);
        if (cmd->args == NULL)
        {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        cmd->args[0] = ft_strdup(arg);
        cmd->args[1] = NULL;
        return ;
    }
    while (cmd->args[i] != NULL)
        i++;
    new_args = malloc(sizeof(char *) * (i + 2));
    if (new_args == NULL)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    while (j < i)
	{
        new_args[j] = cmd->args[j];
		j++;
	}
    new_args[i] = ft_strdup(arg);
    new_args[i + 1] = NULL;
    free(cmd->args);
    cmd->args = new_args;
}

// Helper function to count args
int	count_args(t_list *tokens)
{
	int		arg_count;
	char	*token;

	arg_count = 0;
	while (tokens != NULL)
	{
		token = tokens->content;
		if (strcmp(token, "|") == 0 || strcmp(token, "<") == 0 || strcmp(token,
				">") == 0 || strcmp(token, "<<") == 0 || strcmp(token,
				">>") == 0)
			break ;
		arg_count++;
		tokens = tokens->next;
	}
	return (arg_count);
}

void	handle_heredoc(t_cmd *cmd, const char *delimiter)
{
	char	*line;
	int		pipe_fd[2];
	pid_t	pid;
	size_t	len;
	int		status;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe failed");
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fd[0]);
		while (1)
		{
			write(1, "> ", 2);
			line = get_next_line(0);
			if (!line)
			{
				close(pipe_fd[1]);
				exit(0);
			}
			len = ft_strlen(line);
			if (len > 0 && line[len - 1] == '\n')
				line[len - 1] = '\0';
			if (strcmp(line, delimiter) == 0)
			{
				free(line);
				break ;
			}
			if (write(pipe_fd[1], line, ft_strlen(line)) == -1)
			{
				perror("write error");
				free(line);
				exit(1);
			}
			write(pipe_fd[1], "\n", 1);
			free(line);
		}
		close(pipe_fd[1]);
		exit(0);
	}
	else if (pid > 0)
	{
		close(pipe_fd[1]);
		cmd->input_fd = pipe_fd[0];
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			fprintf(stderr, "Heredoc process ended with error\n");
		}
	}
}

// Function to extract command
t_cmd	*parse_command(t_list *tokens)
{
	t_cmd	*cmd;
	char	*token;

	if (tokens == NULL)
		return (NULL);
	cmd = init_cmd();
	while (tokens != NULL)
	{
		token = tokens->content;
		if (strcmp(token, "|") == 0)
			break ;
		if (strcmp(token, "<") == 0)
		{
			tokens = tokens->next;
			if (tokens == NULL)
			{
				fprintf(stderr, "Error: Missing filename after <\n");
				free_cmd(cmd);
				return (NULL);
			}
			cmd->input_file = strdup(tokens->content);
			if (cmd->input_file == NULL)
			{
				perror("strdup failed");
				free_cmd(cmd);
				return (NULL);
			}
			cmd->input_mode = 1;
		}
		else if (strcmp(token, ">") == 0)
		{
			tokens = tokens->next;
			if (tokens == NULL)
			{
				fprintf(stderr, "Error: Missing filename after >\n");
				free_cmd(cmd);
				return (NULL);
			}
			cmd->output_file = strdup(tokens->content);
			if (cmd->output_file == NULL)
			{
				perror("strdup failed");
				free_cmd(cmd);
				return (NULL);
			}
			cmd->output_mode = 1;
		}
		else if (strcmp(token, ">>") == 0)
		{
			tokens = tokens->next;
			if (tokens == NULL)
			{
				fprintf(stderr, "Error: Missing filename after >>\n");
				free_cmd(cmd);
				return (NULL);
			}
			cmd->output_file = strdup(tokens->content);
			if (cmd->output_file == NULL)
			{
				perror("strdup failed");
				free_cmd(cmd);
				return (NULL);
			}
			cmd->output_mode = 2;
		}
		else if (strcmp(token, "<<") == 0)
		{
			tokens = tokens->next;
			if (tokens == NULL)
			{
				fprintf(stderr, "Error: Missing delimiter after <<\n");
				free_cmd(cmd);
				return (NULL);
			}
			cmd->input_mode = 2;
			handle_heredoc(cmd, tokens->content);
		}
		else
			add_arg(cmd, token);
		tokens = tokens->next;
	}
	return (cmd);
}

// Helper to search for a command in PATH
char	*find_executable(const char *command)
{
	char	*path;
	char	*dirs;
	char	*dir;
	size_t	full_path_len;
	char	*full_path;

	path = getenv("PATH");
	if (!path)
	{
		fprintf(stderr, "Error: PATH not set\n");
		return (NULL);
	}
	dirs = strdup(path);
	if (!dirs)
	{
		perror("strdup failed");
		return (NULL);
	}
	dir = strtok(dirs, ":");
	while (dir)
	{
		full_path_len = strlen(dir) + strlen(command) + 2;
		full_path = malloc(full_path_len);
		if (!full_path)
		{
			perror("malloc failed");
			free(dirs);
			return (NULL);
		}
		snprintf(full_path, full_path_len, "%s/%s", dir, command);
		if (access(full_path, X_OK) == 0)
		{
			free(dirs);
			return (full_path);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(dirs);
	return (NULL);
}

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

/*void	execute_pipeline(t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;
	int		input_fd;
	int		is_last_command;
	t_list	*tokens;
	t_cmd	*cmd;
	int		saved_stdin;
	int		fd;
	char	*exec_path;

	input_fd = STDIN_FILENO;
	is_last_command = 0;
	tokens = shell->tokens;
	while (tokens)
	{
		cmd = parse_command(tokens);
		if (cmd == NULL)
			break ;
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
			break ;
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
			if (cmd->output_mode == 2)
			{
				fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND,
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
			exec_path = find_executable(cmd->args[0]);
			if (!exec_path)
			{
				fprintf(stderr, "minishell: command not found: %s\n",
					cmd->args[0]);
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
			break ;
	}
	if (input_fd != STDIN_FILENO)
		close(input_fd);
}*/
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
