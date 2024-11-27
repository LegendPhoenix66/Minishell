/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 16:51:27 by drenquin          #+#    #+#             */
/*   Updated: 2024/11/27 12:42:45 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// don't work with env fonction directly
void	redirections(const char *file_path, const char *cmd, const char *flags)
{
	int		pid;
	int		err;
	int		file;
	char	*argv[3];

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	// Processus enfant
	if (pid == 0)
	{
		// Ouvre le fichier pour écrire
		file = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (file == -1)
		{
			perror("open");
			exit(2);
		}
		// Redirige stdout vers le fichier
		if (dup2(file, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(file);
			exit(2);
		}
		close(file);
		// Prépare les arguments pour execve
		argv[0] = (char *)cmd;
		argv[1] = (char *)flags;
		argv[2] = NULL;
		// Exécute la commande
		err = execve(cmd, argv, NULL); // NULL pour hériter de l'environnement
		if (err == -1)
		{
			perror("execve");
			exit(2);
		}
	}
	else
	{
		// Processus parent : attend la fin du processus enfant
		wait(NULL);
	}
}
void	redir_linked(const char *file_path, t_node **top)
{
	int	pid;
	int	file;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		// Ouvre le fichier pour écrire
		file = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (file == -1)
		{
			perror("open");
			exit(2);
		}
		// Redirige stdout vers le fichier
		if (dup2(file, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(file);
			exit(2);
		}
		close(file);
		print_lst(top);
		exit(0);
	}
	else
		wait(NULL);
}
