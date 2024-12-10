/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:20:13 by drenquin          #+#    #+#             */
/*   Updated: 2024/11/28 17:20:13 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Debug function for a single node
void debug_node(t_node *node) 
{
    if (!node) 
    {
        printf("Node is NULL.\n");
        return;
    }

    // Affichage des informations basiques du nœud
    printf("Node content: %s\n", node->content ? node->content : "(null)");
    printf("is_cmd: %d\n", node->is_cmd);

    // Déterminer le type de quotes
    if (node->in_single)
    {
        printf("Quotes: Single quotes ('...')\n");
    } 
    else if (node->in_double) 
    {
        printf("Quotes: Double quotes (\"...\")\n");
    } 
    else if (node->no_quotes) 
    {
        printf("Quotes: No quotes\n");
    } 
    else 
    {
        printf("Quotes: Undefined or mixed state\n");
    }

    // Vérification du type de redirection
    printf("Redirection type: ");
    switch (node->type) 
    {
        case STDOUT:
            printf("STDOUT (>)\n");
            break;
        case APPEND:
            printf("APPEND (>>)\n");
            break;
        case STDIN:
            printf("STDIN (<)\n");
            break;
        case HEREDOC:
            printf("HEREDOC (<<)\n");
            break;
        case NO_DIR:
            printf("NO_DIR (not a redirection)\n");
            break;
        default:
            printf("Unknown type\n");
            break;
    }

    // Suivi de la chaîne des nœuds si elle existe
    if (node->next)
    {
        printf("Next node exists.\n");
    } 
    else 
    {
        printf("No next node.\n");
    }
}

// Debug function to traverse an entire t_node linked list
void debug_list(t_node **head) 
{
    t_node *current = *head;
    int index = 0;

    while (current) 
    {
        printf("\033[1;31m=== Node %d ===\033[0m\n", index++);
        debug_node(current);
        current = current->next;
    }
    if (index == 0)
    {
        printf("The list is empty.\n");
    }
}

//work with one argument no try yet with more
char *ft_strtok(char *str, char *separtors)
{
    static char *item;
    char *start_item;

    if(str != NULL)
        item = str;
    if (item == NULL || *item == '\0')
        return (NULL);
    start_item = item;
    while (*item != '\0')
    {
        if (ft_strchr(separtors, *item) != NULL)
        {
            *item = '\0';
            item++;
            break;
        }
        item++;
    }
    return (start_item);
}
