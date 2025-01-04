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
/*int builtin_export(t_shell *args)
{
    char *input = NULL;
    char *equal_pos = NULL;
    char *var_name = NULL;
    char *var_value = NULL;
    char *full_var = NULL;
	char *no_value = NULL;
	int to_free;

	to_free = 0;
    if(!args || !args->tokens)
        return (1);
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
    if (!check_in(input))
        return (1);
    equal_pos = ft_strchr(input, '=');
    if (equal_pos == NULL)
    {
		no_value = ft_strdup(input);
        add_node(&args->export, no_value);
		free(no_value);
    }
    else
    {
        var_name = ft_substr(input, 0, equal_pos - input);   
        if (*(equal_pos + 1) == '\0')
        {
            full_var = ft_strjoin(var_name, "=");
            remove_if(&args->env, full_var);
            add_node(&args->env, full_var);
            free(var_name);
			free(full_var);
        }
        else
        {
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
}*/
// Modification de builtin_export pour accepter les arguments
int builtin_export(t_shell *shell, char **args)
{
    if (!args[1])  // Si pas d'arguments
    {
        print_export(&shell);
        return (0);
    }

    // Pour chaque argument après "export"
    for (int i = 1; args[i]; i++)
    {
        char *input = args[i];
        if (!check_in(input))
            continue;

        char *equal_pos = ft_strchr(input, '=');
        if (!equal_pos)
        {
            // Cas export sans '='
            char *no_value = ft_strdup(input);
            if (no_value)
            {
                add_node(&shell->export, no_value);
                free(no_value);
            }
        }
        else
        {
            // Cas export avec '='
            size_t name_len = equal_pos - input;
            char *var_name = ft_substr(input, 0, name_len);
            char *var_value = ft_strdup(equal_pos + 1);
            
            if (var_name && var_value)
            {
                char *full_var = ft_strjoin(var_name, "=");
                if (full_var)
                {
                    char *complete_var = ft_strjoin(full_var, var_value);
                    if (complete_var)
                    {
                        remove_if(&shell->env, var_name);
                        add_node(&shell->env, complete_var);
                        free(complete_var);
                    }
                    free(full_var);
                }
            }
            free(var_name);
            free(var_value);
        }
    }
    return (0);
}
