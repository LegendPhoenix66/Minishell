/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:58:36 by drenquin          #+#    #+#             */
/*   Updated: 2024/11/11 10:58:36 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void print_export(t_args **args);
void add_or_update(const char *var, t_node **env_list);

int ft_export(const char *var, t_args **args)
{
    int i;

    i = 1;
    if (var == NULL || *var == '\0')
    {
        print_export(args);
        return (0);
    }
    printf("%s\n", var);
    if (ft_isalpha(var[0]) || var[0] == '_' || ((var[0]) == '"' && var[ft_strlen(var) - 1] == '"'))
    {
        while (ft_isalnum(var[i]) || var[i] == '=' || var[i] == '_')
        {
            if (var[i] == '=')
            {
                add_or_update(var, &(*args)->env);
                //checker la validiter de ce qu' il y a apres le egale
                //creation de la nouvelle variable qui ce repercute
                //au processus enfant et existe dans le processus parent
                //si ""=value test" la variable s' appelera just "value"
                return (0);
            }
            i++;
        }
        if(var[i] == '\0')
        {
            printf("variable facultative");
            //todo : facultative var
            return (0);
        }
        printf("invalide");
        return (-1);
    }
    printf("premier caractere invalide");
    return (-1);
}

void add_double_quotes(char *var)
{
    int i;

    i = 0;
    while (var[i] != '\0')
    {
        write(1, &var[i], 1);
        if(var[i] == '=')
            write(1, "\"", 1);
        i++;
    }
    write(1, "\"", 1);
    write(1, "\n", 1);
}
void print_export(t_args **args)
{
    t_node *copy;

    copy = copy_list((*args)->env);
    sort_lst(&copy);
    while (copy != NULL)
    {
        add_double_quotes(copy->env);
        copy = copy->next;
    }
    free_lst(copy);
}
void add_or_update(const char *var, t_node **env_list)
{
    char *name;
    char *equal;
    int  i;

    name = ft_strdup(var);
    equal = ft_strchr(name, '=');
    i = 1;
    printf("equal %s\n", equal);
    printf("name %s\n", name);
    if(equal != NULL)
    {
        //the variable name is in double quotes
        if(equal[i] == '"' && equal[ft_strlen(equal) - 1] == '"')
        {
            equal[ft_strlen(equal)] = '\0';
            i++;
            printf("condition variable name in double quotes\n");
        }
        // all after the export fonction is in doubles quotes
        else if(name[0] == '"' && name[ft_strlen(name) - 1] == '"')
        {
            printf("all name in double quotes\n");
        }
        //no variable name after =
        else if(ft_strlen(equal) == 1 && equal[0] == '=')
        {
            add_node(env_list, name);
        }
    }
}




















































/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Fonction pour ajouter ou modifier une variable dans la liste chaînée
void add_or_update_var(const char *var, t_node **env_list) {
    // Extraire le nom de la variable et sa valeur
    char *name = strdup(var); // Copie de `var` pour modification
    char *equal_pos = strchr(name, '=');
    
    if (equal_pos) {
        // Séparer le nom de la valeur
        *equal_pos = '\0'; // Coupure à `=`
        char *value = equal_pos + 1;

        // Vérifier si la valeur est entourée de guillemets et les retirer
        if (*value == '"' && value[strlen(value) - 1] == '"') {
            value[strlen(value) - 1] = '\0'; // Retirer le guillemet de fin
            value++; // Avancer le pointeur pour ignorer le premier guillemet
        }

        // Créer une nouvelle chaîne avec la forme "name=value"
        size_t new_var_len = strlen(name) + strlen(value) + 2; // +2 pour `=` et `\0`
        char *new_var = malloc(new_var_len);
        if (!new_var) {
            free(name);
            return; // Gérer l'échec de l'allocation
        }
        snprintf(new_var, new_var_len, "%s=%s", name, value);

        // Ajouter ou mettre à jour la variable dans la liste chaînée
        t_node *current = *env_list;
        while (current) {
            if (strncmp(current->env, name, equal_pos - name) == 0 && current->env[equal_pos - name] == '=') {
                // Mettre à jour la variable existante
                free(current->env);
                current->env = new_var;
                free(name);
                return;
            }
            current = current->next;
        }

        // Ajouter la nouvelle variable si elle n'existe pas
        t_node *new_node = malloc(sizeof(t_node));
        if (!new_node) {
            free(name);
            free(new_var);
            return; // Gérer l'échec de l'allocation
        }
        new_node->env = new_var;
        new_node->next = *env_list;
        *env_list = new_node;

        free(name);
    }
}

// Mise à jour de la partie `if (var[i] == '=')` dans `ft_export`
int ft_export(const char *var, t_args **args) {
    int i = 1;
    if (var == NULL || *var == '\0') {
        print_export(args);
        return (0);
    }
    printf("%s\n", var);
    if (ft_isalpha(var[0]) || var[0] == '_') {
        while (ft_isalnum(var[i]) || var[i] == '=' || var[i] == '_') {
            if (var[i] == '=') {
                // Appeler la fonction pour ajouter ou mettre à jour la variable
                add_or_update_var(var, &(*args)->env);
                return (0);
            }
            i++;
        }
        if (var[i] == '\0') {
            printf("variable facultative");
            return (0);
        }
        printf("invalide");
        return (-1);
    }
    printf("premier caractere invalide");
    return (-1);
}*/