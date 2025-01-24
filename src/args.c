/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 11:27:07 by lhopp             #+#    #+#             */
/*   Updated: 2025/01/24 11:27:09 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		perror("malloc failled");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

void	init_arg(t_cmd *cmd, char *cleaned_arg)
{
	cmd->args = safe_malloc(sizeof(char *) * 2);
	cmd->args[0] = cleaned_arg;
	cmd->args[1] = NULL;
}

void	append_arg(t_cmd *cmd, char *cleaned_arg)
{
	int		i;
	int		j;
	char	**new_args;

	i = 0;
	while (cmd->args[i] != NULL)
		i++;
	new_args = safe_malloc(sizeof(char *) * (i + 2));
	j = 0;
	while (j < i)
	{
		new_args[j] = cmd->args[j];
		j++;
	}
	new_args[i] = cleaned_arg;
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
}

void	add_arg(t_cmd *cmd, char *arg, int last_status)
{
	char	*cleaned_arg;

	cleaned_arg = clean_arg(arg, last_status);
	if (!cleaned_arg)
		return ;
	if (cmd->args == NULL)
	{
		init_arg(cmd, cleaned_arg);
		return ;
	}
	append_arg(cmd, cleaned_arg);
}
