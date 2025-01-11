/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 14:52:52 by drenquin          #+#    #+#             */
/*   Updated: 2024/12/14 14:52:52 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// record the file descritor  pointers
// void	save_std_fds(int *saved_stdin, int *saved_stdout)
// {
// 	*saved_stdin = dup(STDIN_FILENO);
// 	*saved_stdout = dup(STDOUT_FILENO);
// }

// Restore the file descriptor pointeurs
// void	restore_std_fds(int saved_stdin, int saved_stdout)
// {
// 	dup2(saved_stdin, STDIN_FILENO);
// 	dup2(saved_stdout, STDOUT_FILENO);
// 	close(saved_stdin);
// 	close(saved_stdout);
// }

// tell if the content of a node is a redirection
int	is_redir(char *node_content)
{
	if (!node_content)
		return (0);
	if (strcmp(node_content, ">") == 0)
		return (1);
	else if (strcmp(node_content, ">>") == 0)
		return (2);
	else if (strcmp(node_content, "<") == 0)
		return (3);
	else if (strcmp(node_content, "<<") == 0)
		return (4);
	return (0);
}

// this fonction return a pointeur to the redirection token if there is one
// in the token link list
// t_list	*is_a_redirecton(t_shell *args)
// {
// 	t_list	*current;
// 	t_list	*redirection;
//
// 	redirection = NULL;
// 	current = args->tokens;
// 	while (current != NULL)
// 	{
// 		if (is_redir(current->content))
// 		{
// 			redirection = current;
// 		}
// 		current = current->next;
// 	}
// 	return (redirection);
// }

// int	handle_input_redir(char *file)
// {
// 	int	fd;
//
// 	fd = open(file, O_RDONLY);
// 	if (fd == -1)
// 	{
// 		perror("open");
// 		return (-1);
// 	}
// 	if (dup2(fd, STDIN_FILENO) == -1)
// 	{
// 		perror("dup2");
// 		close(fd);
// 		return (-1);
// 	}
// 	close(fd);
// 	return (0);
// }

int	handle_output_redir(char *file, int flags)
{
	int	fd;

	fd = open(file, flags, 0644);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

// int	handle_redirection(t_list *token)
// {
// 	if (strcmp(token->content, ">") == 0)
// 		return (handle_output_redir(token->next->content,
// 				O_WRONLY | O_CREAT | O_TRUNC));
// 	else if (strcmp(token->content, ">>") == 0)
// 		return (handle_output_redir(token->next->content,
// 				O_WRONLY | O_CREAT | O_APPEND));
// 	return (-1);
// }
