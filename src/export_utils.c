/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 18:58:12 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/19 18:58:12 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int is_valid_var_char(char c)
{
    return (ft_isalnum(c) || c == '_');
}

static int check_var_name(char *var, size_t len)
{
    size_t i;

    i = 1;
    while (i < len)
    {
        if (!is_valid_var_char(var[i]))
            return (0);
        i++;
    }
    return (1);
}

static void print_identifier_error(char *var)
{
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(var, 2);
    ft_putstr_fd(": not a valid identifier\n", 2);
}

static int validate_basic_input(char *var, t_shell *shell)
{
    if (!var || !*var)
    {
        print_identifier_error("");
        shell->last_status = 1;
        return (0);
    }
    if (var[0] == '=')
    {
        print_identifier_error(var);
        shell->last_status = 1;
        return (0);
    }
    return (1);
}

int check_in(char *var, t_shell *shell)
{
    char    *equal_pos;
    size_t  len;

    if (!validate_basic_input(var, shell))
        return (0);
    if (!ft_isalpha(var[0]) && var[0] != '_')
    {
        print_identifier_error(var);
        shell->last_status = 1;
        return (0);
    }
    equal_pos = ft_strchr(var, '=');
    if (equal_pos)
        len = (size_t)(equal_pos - var);
    else
        len = ft_strlen(var);
    if (!check_var_name(var, len))
    {
        print_identifier_error(var);
        shell->last_status = 1;
        return (0);
    }
    return (1);
}
