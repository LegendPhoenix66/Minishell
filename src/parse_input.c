/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:46:54 by lhopp             #+#    #+#             */
/*   Updated: 2024/11/27 11:26:07 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

// Helper function to join path and command
char	*join_path_command(const char *path, const char *cmd)
{
	char	*full_path;
	size_t	len;

	len = ft_strlen(path) + ft_strlen(cmd) + 2;
	full_path = malloc(len);
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, path, len);
	ft_strlcat(full_path, "/", len);
	ft_strlcat(full_path, cmd, len);
	return (full_path);
}

// Search command in PATH and return a full path or NULL
char	*find_command_in_path(char *cmd)
{
	char	*path_env;
	char	*full_path;
	int		i;
	char	**paths;

	path_env = getenv("PATH");
	paths = ft_split(path_env, ':');
	full_path = NULL;
	i = 0;
	while (paths[i])
	{
		full_path = join_path_command(paths[i], cmd);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}

void	execute_external_command(t_shell *args)
{
	char	*cmd_path;
	pid_t	pid;
	int		pipe_fd[2];
	char	buffer[1024];
	ssize_t	bytes_read;
	char	*argv[3];
	t_list	*token_node;

	token_node = args->tokens;
	argv[0] = (char *)token_node->content;
	if (token_node->next == NULL)
		argv[1] = NULL;
	else
		argv[1] = (char *)token_node->next->content;
	argv[2] = NULL;
	if (((char *)token_node->content)[0] == '/'
		|| ((char *)token_node->content)[0] == '.')
	{
		printf("content %s\n", (char *)(token_node->content));
		if (access(token_node->content, X_OK) == 0)
			cmd_path = strdup(token_node->content);
		else
		{
			write(2, "Command not found\n", 18);
			return ;
		}
	}
	else
	{
		cmd_path = find_command_in_path(token_node->content);
		if (cmd_path == NULL)
		{
			write(2, "Command not found\n", 18);
			return ;
		}
	}
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe error");
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		if (execve(cmd_path, argv, args->environ) == -1)
		{
			perror("execve error");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid > 0)
	{
		close(pipe_fd[1]);
		while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytes_read] = '\0';
			write(STDOUT_FILENO, buffer, bytes_read);
		}
		close(pipe_fd[0]);
		waitpid(pid, NULL, 0);
	}
	free(cmd_path);
}

// Example parsing input into tokens
void	parse_input(char *input, t_shell *shell)
{
	shell->tokens = tokenize_input(input, shell->last_status);
	execute_command1(shell);
}
