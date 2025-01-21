/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 12:23:21 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/21 12:24:47 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	add_double_quotes(char *var)
{
	int	i;

	i = 0;
	write(1, "declare -x ", 11);
	while (var[i] != '\0')
	{
		write(1, &var[i], 1);
		if (var[i] == '=')
			write(1, "\"", 1);
		i++;
	}
	write(1, "\"", 1);
	write(1, "\n", 1);
}

void	print_export(t_shell **args)
{
	t_node	*copy;
	t_node	*head;
	t_node	*copy_exp;
	t_node	*head_exp;

	copy = copy_list((*args)->env);
	sort_lst(&copy);
	head = copy;
	while (copy != NULL)
	{
		add_double_quotes(copy->content);
		copy = copy->next;
	}
	copy_exp = copy_list((*args)->export);
	head_exp = copy_exp;
	while (copy_exp != NULL)
	{
		write(1, "declare -x ", 11);
		printf("%s\n", copy_exp->content);
		copy_exp = copy_exp->next;
	}
	free_lst(head);
	free_lst(head_exp);
}
