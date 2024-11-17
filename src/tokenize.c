/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 15:49:31 by lhopp             #+#    #+#             */
/*   Updated: 2024/11/17 15:49:35 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void add_token(t_node **list, const char *token, int length)
{
    t_node *new_node = malloc(sizeof(t_node));
    new_node->content = strndup(token, length);
    new_node->next = NULL;

    if (*list == NULL)
    {
        *list = new_node;
    }
    else
    {
        t_node *current = *list;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}

void tokenize_input(const char *input, t_args **args)
{
    int i = 0, j = 0;
    char quote = 0;

    (*args)->tokens = NULL;
    (*args)->flags = NULL;

    while (input[i])
    {
        if (input[i] == '\'' || input[i] == '\"')
        {
            if (quote == 0)
                quote = input[i];
            else if (quote == input[i])
                quote = 0;
        }
        if ((input[i] == ' ' && quote == 0) || input[i] == '|' || input[i] == '<' || input[i] == '>')
        {
            if (j > 0)
            {
                if (input[i - j] == '-')
                    add_token(&(*args)->flags, input + i - j, j);
                else
                    add_token(&(*args)->tokens, input + i - j, j);
                j = 0;
            }
            if (input[i] != ' ')
            {
                add_token(&(*args)->tokens, input + i, 1);
            }
        }
        else
        {
            j++;
        }
        i++;
    }
    if (j > 0)
    {
        if (input[i - j] == '-')
            add_token(&(*args)->flags, input + i - j, j);
        else
            add_token(&(*args)->tokens, input + i - j, j);
    }
}
