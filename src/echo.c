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

void builtin_echo(const t_list *tokens)
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
}
