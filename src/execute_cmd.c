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


int is_builtin(const char *token)
{
    const char *builtins[] = {"exit", "cd", "pwd", "echo", "env", "unset", "export"};
    size_t i;

    if (!token)
        return 0;

    for (i = 0; i < sizeof(builtins) / sizeof(builtins[0]); i++)
    {
        if (strcmp(token, builtins[i]) == 0)
            return 1;
    }
    return 0;
}
// Helper function to allocate a t_cmd struct
t_cmd *init_cmd() 
{
	t_cmd *cmd = malloc(sizeof(t_cmd));
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
    return cmd;
}

void free_cmd(t_cmd *cmd) 
{
    int i = 0;
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
void add_arg(t_cmd *cmd, char *arg) 
{
    int i = 0;
    if (cmd->args == NULL)
    {
        cmd->args = malloc(sizeof(char *) * 2);
        if (cmd->args == NULL)
        {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        cmd->args[0] = strdup(arg);
        cmd->args[1] = NULL;
        return ;
    }
    while (cmd->args[i] != NULL)
        i++;
    char **new_args = realloc(cmd->args, sizeof(char *) * (i + 2));
    if (new_args == NULL)
    {
            perror("realloc failed");
            exit(EXIT_FAILURE);
    }
    cmd->args = new_args;
    cmd->args[i] = ft_strdup(arg);
    cmd->args[i+1] = NULL;
}

// Helper function to count args
int count_args(t_list *tokens) 
{
    int arg_count = 0;
    while (tokens != NULL)
    {
        char *token = tokens->content;
        if (strcmp(token, "|") == 0
            || strcmp(token, "<") == 0
            || strcmp(token, ">") == 0
            || strcmp(token, "<<") == 0
            || strcmp(token, ">>") == 0)
            break;
        arg_count++;
        tokens = tokens->next;
    }
    return arg_count;
}

/*void handle_heredoc(t_cmd *cmd, const char *delimiter)
{
    char *line;
    int pipe_fd[2];
    
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe failed");
        return;
    }

    // Créer un processus fils pour gérer l'entrée
    pid_t pid = fork();
    if (pid == 0)
    {
        close(pipe_fd[0]);  // Fermer le côté lecture dans le fils
        
        while (1)
        {
            write(1, "> ", 2);  // Prompt pour heredoc
            line = get_next_line(0);
            
            if (!line)
                break;
                
            // Supprimer le newline à la fin
            size_t len = ft_strlen(line);
            if (len > 0 && line[len - 1] == '\n')
                line[len - 1] = '\0';
                
            // Vérifier si c'est le délimiteur
            if (strcmp(line, delimiter) == 0)
            {
                free(line);
                break;
            }
            
            // Écrire la ligne dans le pipe
            write(pipe_fd[1], line, ft_strlen(line));
            write(pipe_fd[1], "\n", 1);
            free(line);
        }
        
        close(pipe_fd[1]);
        exit(0);
    }
    else if (pid > 0)
    {
        int status;
        close(pipe_fd[1]);  // Fermer le côté écriture dans le parent
        cmd->input_fd = pipe_fd[0];  // Sauvegarder le fd de lecture
        waitpid(pid, &status, 0);
    }
}*/

void handle_heredoc(t_cmd *cmd, const char *delimiter)
{
    char *line;
    int pipe_fd[2];
    
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe failed");
        return;
    }

    pid_t pid = fork();
    if (pid == 0)
    {
        close(pipe_fd[0]);  
        
        while (1)
        {
            write(1, "> ", 2);
            line = get_next_line(0);
            
            // Ajout de la gestion explicite de Ctrl+D
            if (!line)
            {
                close(pipe_fd[1]);
                exit(0);  // Sortie propre en cas de Ctrl+D
            }
            
            // Nettoyage de la ligne
            size_t len = ft_strlen(line);
            if (len > 0 && line[len - 1] == '\n')
                line[len - 1] = '\0';
                
            // Vérification du délimiteur
            if (strcmp(line, delimiter) == 0)
            {
                free(line);
                break;
            }
            
            // Écriture dans le pipe avec vérification
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
        int status;
        close(pipe_fd[1]);
        cmd->input_fd = pipe_fd[0];
        
        // Attente explicite du processus fils
        waitpid(pid, &status, 0);
        
        // Vérification du statut
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            // Gestion de l'erreur si nécessaire
            fprintf(stderr, "Heredoc process ended with error\n");
        }
    }
}

// Function to extract command
t_cmd *parse_command(t_list **tokens) 
{
    t_cmd *cmd = init_cmd();
    if (*tokens == NULL)
		return NULL;
    while (*tokens != NULL) 
    {
        char *token = (*tokens)->content;
		if (strcmp(token, "|") == 0)
			break ;
        if (strcmp(token, "<") == 0)
        {
            *tokens = (*tokens)->next;
            if (*tokens == NULL) 
            {
                fprintf(stderr, "Error: Missing filename after <\n");
                free_cmd(cmd);
                return NULL;
            }
            cmd->input_file = strdup((*tokens)->content);
            cmd->input_mode = 1;
        }
        else if (strcmp(token, ">") == 0)
        {
            *tokens = (*tokens)->next;
            if (*tokens == NULL) 
            {
                fprintf(stderr, "Error: Missing filename after >\n");
                free_cmd(cmd);
                return NULL;
            }
            cmd->output_file = strdup((*tokens)->content);
            cmd->output_mode = 1;
        }
        else if (strcmp(token, ">>") == 0)
        {
            *tokens = (*tokens)->next;
            if (*tokens == NULL) 
            {
                fprintf(stderr, "Error: Missing filename after >>\n");
                free_cmd(cmd);
                return NULL;
            }
            cmd->output_file = strdup((*tokens)->content);
            cmd->output_mode = 2;
        }
        else if (strcmp(token, "<<") == 0)
        {
            *tokens = (*tokens)->next;
            if (*tokens == NULL) 
            {
                fprintf(stderr, "Error: Missing delimiter after <<\n");
                free_cmd(cmd);
                return NULL;
            }
            //cmd->input_file = strdup((*tokens)->content);
            cmd->input_mode = 2;
            handle_heredoc(cmd, (*tokens)->content);
        }
        else
            add_arg(cmd, token);
        *tokens = (*tokens)->next;
    }
	return cmd;
}

void execute_simple_command(t_cmd *cmd, t_shell *shell) 
{
    if(is_builtin(cmd->args[0]))
    {
        execute_builtin(cmd, shell);
        return ;
    }
    pid_t pid = fork();
    if (pid == 0) 
    { // Child process

        // Handle input redirection
		if (cmd->input_mode == 1)
        {
			int fd = open(cmd->input_file, O_RDONLY);
            if (fd == -1)
            {
                perror("Error opening input file");
				free_cmd(cmd);
                exit(EXIT_FAILURE);
            }
			dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if(cmd->input_mode == 2) 
        {
            // handle here_doc
            if (cmd->input_fd > 0)
            {
                dup2(cmd->input_fd, STDIN_FILENO);
                close(cmd->input_fd);
            }
        }

        // Handle output redirection
		if(cmd->output_mode == 1)
		{
            int fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
            int fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
             if (fd == -1)
            {
                perror("Error opening output file");
				free_cmd(cmd);
                exit(EXIT_FAILURE);
            }
			dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        if (cmd->args != NULL && cmd->args[0] != NULL) 
        {
            execvp(cmd->args[0], cmd->args);
            perror("execvp failed"); // Only reached if execvp fails
            free_cmd(cmd);
            exit(EXIT_FAILURE);
        } 
        else 
        {
            fprintf(stderr, "Error: No command specified\n");
			free_cmd(cmd);
            exit(EXIT_FAILURE);
        }
    } 
    else if (pid > 0)
    { // Parent process
        int status;
        waitpid(pid, &status, 0);
    } 
    else 
    {
        perror("fork failed");
		free_cmd(cmd);
        exit(EXIT_FAILURE);
    }
}

void execute_pipeline(t_list **tokens, t_shell *shell)
{
    int pipefd[2];
    pid_t pid;
    int input_fd = STDIN_FILENO; // Initial input source

    while (1)
    {
        t_cmd *cmd = parse_command(tokens);
        if (cmd == NULL)
            break;
        // Gestion spéciale pour le dernier builtin dans un pipeline
        if (is_builtin(cmd->args[0]) && *tokens == NULL)
        {
            if (input_fd != STDIN_FILENO)
            {
                // Sauvegarder stdin
                int saved_stdin = dup(STDIN_FILENO);
                // Rediriger l'entrée du pipe
                dup2(input_fd, STDIN_FILENO);
                
                execute_builtin(cmd, shell);
                
                // Restaurer stdin
                dup2(saved_stdin, STDIN_FILENO);
                close(saved_stdin);
                close(input_fd);
            }
            else
                execute_builtin(cmd, shell);
            
            free_cmd(cmd);
            break;
        }
        if (*tokens != NULL && strcmp((*tokens)->content, "|") == 0)
            *tokens = (*tokens)->next;
		if (pipe(pipefd) == -1)
        {
            perror("pipe failed");
			free_cmd(cmd);
            exit(EXIT_FAILURE);
        }
        pid = fork();

		if (pid == 0) // Child process
		{
			dup2(input_fd, STDIN_FILENO);
			if (*tokens != NULL)
				dup2(pipefd[1], STDOUT_FILENO); // output of this process goes to pipe

            close(pipefd[0]);
            close(pipefd[1]);
            if (strcmp(cmd->args[0], "env") == 0)
            {
                print_lst(&shell->env);
                exit(EXIT_SUCCESS);
            }
            if (cmd->input_mode == 1)
            {
                int fd = open(cmd->input_file, O_RDONLY);
                if (fd == -1)
                {
                    perror("Error opening input file");
					free_cmd(cmd);
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            if(cmd->output_mode == 1)
            {
                int fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
                int fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd == -1)
                {
                    perror("Error opening output file");
					free_cmd(cmd);
                    exit(EXIT_FAILURE);
                }
				dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            if(cmd->args != NULL && cmd->args[0] != NULL)
            {
                execvp(cmd->args[0], cmd->args);
                perror("execvp failed");
                free_cmd(cmd);
                exit(EXIT_FAILURE);
            }
            else
            {
                fprintf(stderr, "Error: No command specified\n");
                free_cmd(cmd);
                exit(EXIT_FAILURE);
            }
        }
		else if (pid > 0) //Parent process
		{
			close(pipefd[1]); // Close the write end of the current pipe
            if (input_fd != STDIN_FILENO)
                close(input_fd); // Close the previous input fd, only if it wasn't stdin
			input_fd = pipefd[0]; // Set the read end of the current pipe for next iteration
            int status;
            waitpid(pid, &status, 0);
        }
		else
		{
			perror("fork failed");
            free_cmd(cmd);
            exit(EXIT_FAILURE);
        }
        free_cmd(cmd);

		if (*tokens == NULL)
			break;
    }
	if (input_fd != STDIN_FILENO)
		close(input_fd);
}


void execute_command1(t_shell *shell) 
{
    t_list *tokens = shell->tokens;
    if (tokens == NULL)
        return;
    t_list *temp_tokens = shell->tokens;
	int has_pipe = 0;
	while (temp_tokens) 
    {
		char *token = temp_tokens->content;
		if (strcmp(token, "|") == 0)
		{
			has_pipe = 1;
			break;
		}
		temp_tokens = temp_tokens->next;
	}
	if (has_pipe)
		execute_pipeline(&shell->tokens, shell);
    else
    { 
        t_cmd *cmd = parse_command(&shell->tokens);
        if (cmd)
        {
            execute_simple_command(cmd, shell);
            free_cmd(cmd);
        }
    }
}
