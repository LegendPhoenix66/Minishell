/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:46:54 by lhopp             #+#    #+#             */
/*   Updated: 2024/09/24 20:46:59 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern char **environ;
void free_split(char **split)
{
    int i = 0;

    if (!split)
        return;

    while (split[i])
    {
        free(split[i]);
        i++;
    }
    free(split);  // Free the array of strings itself
}

// Helper function to join path and command
char *join_path_command(const char *path, const char *cmd)
{
    char *full_path;
    size_t len = strlen(path) + strlen(cmd) + 2;  // +2 for '/' and '\0'

    full_path = malloc(len);
    if (!full_path)
        return NULL;

    snprintf(full_path, len, "%s/%s", path, cmd);
    return full_path;
}

// Search command in PATH and return a full path or NULL
char *find_command_in_path(char *cmd)
{
    char *path_env = getenv("PATH");
    char **paths = ft_split(path_env, ':');  // Use your ft_split to split PATH by ':'
    char *full_path = NULL;
    int i = 0;

    while (paths[i])
    {
        full_path = join_path_command(paths[i], cmd);
        if (access(full_path, X_OK) == 0)  // Check if the command exists and is executable
        {
            free_split(paths);  // Custom function to free memory of ft_split
            return full_path;
        }
        free(full_path);
        i++;
    }

    free_split(paths);
    return NULL;
}

// Function to execute commands (either built-in or external)
void execute_command(char **args)
{
    if (args[0] == NULL)  // No command entered
        return;

    // Built-in 'cd' command
    if (strcmp(args[0], "cd") == 0)
    {
        if (args[1] == NULL || chdir(args[1]) != 0)
            perror("cd error");
    }
    else
    {
        // External command
        char *cmd_path = find_command_in_path(args[0]);
        if (cmd_path == NULL)
        {
            write(2, "Command not found\n", 18);
            return;
        }

        pid_t pid = fork();
        if (pid == 0)  // Child process
        {
            if (execve(cmd_path, args, environ) == -1)  // Use execve to execute the command
            {
                perror("execve error");
                exit(EXIT_FAILURE);
            }
        }
        else if (pid > 0)  // Parent process
        {
            waitpid(pid, NULL, 0);  // Wait for the child process to finish
        }

        free(cmd_path);  // Free the full path allocated
    }
}

// Example parsing input into tokens
void **parse_input(char *input)
{
	char **args;

    args = ft_split(input, ' ');  // Use your ft_split function to split the input by spaces
	execute_command(args);  // Execute the command with the arguments
	free_split(args);  // Custom function to free memory of ft_split
}