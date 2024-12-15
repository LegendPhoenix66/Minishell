/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 18:09:48 by drenquin          #+#    #+#             */
/*   Updated: 2024/12/04 18:09:48 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// compare variable value and remove all the line
//if the function finds this one
int builtin_unset(t_node **env_list, const char *var)
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
        if (strncmp(current->content, var, len) == 0 && current->content[len] == '=')
        {
            if (previous == NULL)
                *env_list = current->next;
            else
                previous->next = current->next;
            free(current->content);
            free(current);
            return (0);
        }
        previous = current;
        current = current->next;
    }
    return (-1);
}
