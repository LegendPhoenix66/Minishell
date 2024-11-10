/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 18:44:35 by drenquin          #+#    #+#             */
/*   Updated: 2024/11/10 18:44:35 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_node  *make_node(const char *env)
{
    t_node *new_node;

    new_node = (t_node *)malloc(sizeof(t_node));
    if(new_node == NULL)
    {
        perror("malloc error");
        return (NULL);
    }
    new_node->env = ft_strdup(env);
    if(new_node->env == NULL)
    {
        perror("ft_strdup error");
        free(new_node);
        return (NULL);
    }
    new_node->next = NULL;
    return (new_node);
}
void    add_node(t_node **top, const char *env)
{
    t_node *new_node = make_node(env);
    t_node *current;

    if(new_node == NULL)
        return;
    if(*top == NULL)
        *top = new_node;
    else
    {
        current = *top;
        while (current->next != NULL)
            current = current->next;
        current->next = new_node;
    }
}
t_node *init_lst(void)
{
    t_node *lst = NULL;
    size_t i;

    i = 0;
    if(__environ == NULL)
    {
        perror("env error");
        return (NULL);
    }
    while (__environ[i] != NULL)
    {
        add_node(&lst, __environ[i]);
        i++;
    }
    return (lst);
}
void print_lst(t_node **top)
{
    t_node *current = *top;

    while (current != NULL)
    {
        printf("%s\n", current->env);
        current = current->next;
    }
}
void free_lst(t_node *top)
{
    t_node *tmp;

    while (top != NULL)
    {
        tmp = top;
        top = top->next;
        free(tmp->env);
        free(tmp);
    }
}