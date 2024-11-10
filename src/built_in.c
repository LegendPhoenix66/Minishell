/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 11:46:02 by drenquin          #+#    #+#             */
/*   Updated: 2024/11/09 11:46:02 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// compare variable value and remove all the line
//if the fonction find this one
int ft_unsetenv(t_node **env_list, const char *var)
{
    size_t len;
    t_node *current;
    t_node *previous;

    if (var == NULL || *var == '\0')
    {
        perror("var error");
        return (-1);
    }
    len = ft_strlen(var);
    current = *env_list;
    previous = NULL;
    while (current != NULL)
    {
        if (strncmp(current->env, var, len) == 0 && current->env[len] == '=')
        {
            if (previous == NULL)
                *env_list = current->next;
            else
                previous->next = current->next;
            free(current->env);
            free(current);
            return (0);
        }
        previous = current;
        current = current->next;
    }
    return (-1);
}
