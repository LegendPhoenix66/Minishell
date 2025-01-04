/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 15:50:15 by lhopp             #+#    #+#             */
/*   Updated: 2024/12/01 15:50:20 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*void builtin_echo(const t_list *tokens)
{
	int newline = 1;
	// Check for the -n flag
	if (tokens && strcmp(tokens->content, "-n") == 0)
	{
		newline = 0;
		tokens = tokens->next; // Move to the next flag if any
	}
	// Print the arguments
	while (tokens)
	{
		printf("%s", (char *)tokens->content);
		if (tokens->next)
			printf(" ");
		tokens = tokens->next;
	}
	if (newline)
		printf("\n");
}*/
int builtin_echo(t_cmd *cmd)
{
    int i = 1;
    int newline = 1;

    // Check for -n option
    if (cmd->args[i] && !strcmp(cmd->args[i], "-n"))
    {
        newline = 0;
        i++;
    }

    // Print arguments
    while (cmd->args[i])
    {
        ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
        if (cmd->args[i + 1])
            ft_putchar_fd(' ', STDOUT_FILENO);
        i++;
    }

    if (newline)
        ft_putchar_fd('\n', STDOUT_FILENO);
    return 0;
}

int builtin_pwd()
{
    char *pwd = getcwd(NULL, 0);
    if (!pwd)
    {
        return 1;
    }
    ft_putendl_fd(pwd, STDOUT_FILENO);
    free(pwd);
    return 0;
}
int builtin_cd(t_shell *shell, t_cmd *cmd)
{
    char *path;
    char *old_pwd;

    // Save current directory
    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
        return 1;

    // Determine target path
    if (!cmd->args[1] || !strcmp(cmd->args[1], "~"))
        path = getenv("HOME");
    else
        path = cmd->args[1];

    // Change directory
    if (chdir(path) != 0)
    {
        free(old_pwd);
        return 1;
    }

    // Update current directory
    free(shell->current_directory);
    shell->current_directory = getcwd(NULL, 0);
    free(old_pwd);
    return 0;
}
