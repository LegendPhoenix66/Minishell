/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 22:46:08 by drenquin          #+#    #+#             */
/*   Updated: 2024/12/14 22:46:08 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// this fonction is for executing when there is
// a redirection in the prompte
// is not so far away from the other one but
// she exclude the pipe stuff to fit with the redirection fonction
// can be nice if in the future this fonction have common fonction
// and common structure with the other one
// void	execute_external_command1(t_shell *args)
// {
// 	char	*cmd_path;
// 	pid_t	pid;
// 	char	*argv[3];
// 	t_list	*token_node;
//
// 	token_node = args->tokens;
// 	argv[0] = (char *)token_node->content;
// 	if (token_node->next == NULL || is_redir(token_node->next->content) != 0)
// 		argv[1] = NULL;
// 	else
// 		argv[1] = (char *)token_node->next->content;
// 	argv[2] = NULL;
// 	if (((char *)token_node->content)[0] == '/'
// 		|| ((char *)token_node->content)[0] == '.')
// 	{
// 		if (access(token_node->content, X_OK) == 0)
// 			cmd_path = strdup(token_node->content);
// 		else
// 		{
// 			write(2, "Command not found\n", 18);
// 			return ;
// 		}
// 	}
// 	else
// 	{
// 		cmd_path = find_command_in_path(token_node->content);
// 		if (cmd_path == NULL)
// 		{
// 			write(2, "Command not found\n", 18);
// 			return ;
// 		}
// 	}
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		if (execve(cmd_path, argv, args->environ) == -1)
// 		{
// 			perror("execve error");
// 			exit(EXIT_FAILURE);
// 		}
// 	}
// 	else if (pid > 0)
// 	{
// 		waitpid(pid, NULL, 0);
// 	}
// 	free(cmd_path);
// }
