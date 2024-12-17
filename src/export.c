/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 12:16:46 by drenquin          #+#    #+#             */
/*   Updated: 2024/12/06 12:16:46 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int     find_equal(const char *var)
{
        int     i;

        i = 0;
        while (var[i] != '=' && var[i])
                i++;
        if (var[i] == '=')
        {
                printf("i vaut %d\n", i);
                return (i);
        }
        else
                return (-1);
}
// check the validity of export arg (first charactere of argument)
int     check_in(const char *var)
{
        if (ft_isalpha(var[0]))
                return (1);
        else if (var[0] == '_')
                return (1);
        else if ((var[0]) == '"' && var[ft_strlen(var) - 1] == '"')
                return (1);
        return (0);
}
void    add_double_quotes(char *var)
{
        int     i;

        i = 0;
		write(1,"declare -x ",11);
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
void    print_export(t_shell **args)
{
        t_node  *copy;
        t_node  *head;
		t_node  *copy_exp;
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
			write(1,"declare -x ",11);
			printf("%s\n", copy_exp->content);
			copy_exp = copy_exp->next;
		}
        free_lst(head);
		free_lst(head_exp);
}
//fonctionne avec l' affichage et le trieen
/*int		builtin_export(t_shell *args)
{
	t_list *current;
	t_list *prev;
	char *new_var;
	char *var_value;
	int egale;

	egale = 0;
	current = args->tokens;
	prev = NULL;
	//export cmd is alone
	if(current->next == NULL)
	{
		print_export(&args);
	}
	else if(check_in(current->next->content))
	{
		while (current != NULL)
		{
			//in both cases the variable have to be add at env variable
			if(strcmp(current->content, "=") == 0)
			{
				//no value after equal
				if(current->next == NULL)
				{
					printf("pas de valeur donner a la variable\n");
					new_var = ft_strjoin(prev->content,current->content);
					remove_if(&args->env, new_var);
					add_node(&args->env, new_var);
					free(new_var);
					return(1);
				}
				//variable had a value after equal
				else if (current->next != NULL)
				{
					printf("presence d' une valeur apres le egale\n");
					new_var = ft_strjoin(prev->content,current->content);
					var_value = ft_strjoin(new_var, current->next->content);
					remove_if(&args->env, new_var);
					add_node(&args->env, var_value);
					free(new_var);
					free(var_value);
					return(1);
				}
				egale++;
			}
			else if(current->next == NULL && egale == 0)
			{
				printf("pas de egale\n");
				add_node(&args->export, current->content);
				return(1);
			}
			prev = current;
			current = current->next;
		}
	}
	return (0);
}*/
char *concat_list_content(t_list *list)
{
    t_list *current;
    char *result;
    char *temp;
    
    if (!list)
        return (NULL);
    result = ft_strdup(list->content);
    if (!result)
        return (NULL);
    current = list->next;
    while (current)
    {
        temp = result;
        result = ft_strjoin(result, current->content);
        free(temp);
        if (!result)
            return (NULL);
        current = current->next;
    }
	printf("use concatenation fonction");
    return (result);
}

int builtin_export(t_shell *args)
{
    char *input;
    char *equal_pos;
    char *var_name;
    char *var_value;
    char *full_var;
	char *no_value;
	int to_free;

    // Si pas d'argument après export
	to_free = 0;
    if (args->tokens->next == NULL)
    {
        print_export(&args);
        return (0);
    }

    if(count_node(&args->tokens) < 3)
    	input = args->tokens->next->content;
	else
	{
		input = concat_list_content(args->tokens->next);
		to_free++;
	}
    // Vérifie si l'input est valide
    if (!check_in(input))
        return (1);

    // Cherche le signe égal
    equal_pos = ft_strchr(input, '=');

    if (equal_pos == NULL)
    {
        // Pas de signe égal, ajouter à export
        printf("pas de egale\n");
		no_value = ft_strdup(input);
        add_node(&args->export, no_value);
		free(no_value);
    }
    else
    {
        // Extrait le nom de variable
        var_name = ft_substr(input, 0, equal_pos - input);
        
        if (*(equal_pos + 1) == '\0')
        {
            // Égal sans valeur
            printf("pas de valeur donner a la variable\n");
            full_var = ft_strjoin(var_name, "=");
            remove_if(&args->env, full_var);
            add_node(&args->env, full_var);
            free(var_name);
			free(full_var);
        }
        else
        {
            // Égal avec valeur
            printf("presence d'une valeur apres le egale\n");
            var_value = ft_strdup(equal_pos + 1);
            full_var = ft_strjoin(var_name, "=");
			free(var_name);
            var_name = full_var;
            full_var = ft_strjoin(var_name, var_value);
            remove_if(&args->env, var_name);
            add_node(&args->env, full_var);
            free(var_name);
            free(var_value);
			free(full_var);
        }
    }
	if(to_free != 0)
		free(input);
    return (0);
}
