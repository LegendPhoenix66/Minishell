/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:46:54 by lhopp             #+#    #+#             */
/*   Updated: 2024/10/09 15:34:18 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern char	**environ;
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
	free(split); // Free the array of strings itself
}

// Helper function to join path and command
char	*join_path_command(const char *path, const char *cmd)
{
	char	*full_path;

	size_t len = ft_strlen(path) + ft_strlen(cmd) + 2; // +2 for '/' and '\0'
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

	path_env = getenv("PATH");
	char **paths = ft_split(path_env, ':');
		// Use your ft_split to split PATH by ':'
	full_path = NULL;
	i = 0;
	while (paths[i])
	{
		full_path = join_path_command(paths[i], cmd);
		if (access(full_path, X_OK) == 0)
			// Check if the command exists and is executable
		{
			free_split(paths); // Custom function to free memory of ft_split
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}

void	execute_external_command(char **args)
{
	char	*cmd_path;
	pid_t	pid;
	int		pipe_fd[2];
	char	buffer[1024];
	ssize_t	bytes_read;

	cmd_path = find_command_in_path(args[0]);
	if (cmd_path == NULL)
	{
		write(2, "Command not found\n", 18);
		return ;
	}

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe error");
		return ;
	}

	pid = fork();
	if (pid == 0) // Child process
	{
		close(pipe_fd[0]); // Close read end of the pipe
		dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to the write end of the pipe
		close(pipe_fd[1]); // Close the write end of the pipe after duplicating
		if (execve(cmd_path, args, environ) == -1)
		{
			perror("execve error");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid > 0) // Parent process
	{
		close(pipe_fd[1]); // Close write end of the pipe
		while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytes_read] = '\0';
			write(STDOUT_FILENO, buffer, bytes_read); // Write the output to stdout
		}
		close(pipe_fd[0]); // Close read end of the pipe
		waitpid(pid, NULL, 0); // Wait for the child process to finish
	}
	free(cmd_path); // Free the full path allocated
}
// Function to execute commands (either built-in or external)
void	execute_command(char **args, t_node **env_lst)
{
		const char *dir = (args[1] == NULL) ? getenv("HOME") : args[1];
		//char **env;
		//char **new;

	if (args[0] == NULL) // No command entered
		return ;
	// Built-in 'cd' command
	if (strcmp(args[0], "cd") == 0)
	{
		if (dir == NULL || chdir(dir) != 0)
			perror("cd error");
	}
	else if (strcmp(args[0], "env") == 0)
		print_lst(env_lst);
	else if (strcmp(args[0], "unset") == 0 && args[1] != NULL)
		ft_unsetenv(env_lst, args[1]);
	else
	{
		execute_external_command(args);
	}
}

// Example parsing input into tokens
void	parse_input(char *input, t_args **args)
{
	char	**command;
	t_node **env_lst = &(*args)->env;

	//print_lst(env_lst);
	command = ft_split(input, ' ');
		// Use your ft_split function to split the input by spaces
	execute_command(command, env_lst);       // Execute the command with the arguments
	free_split(command);            // Custom function to free memory of ft_split
}
