/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 12:16:46 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/13 17:08:50 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// check the validity of export arg (first charactere of argument)
int	check_in(const char *var)
{
	if (ft_isalpha(var[0]))
		return (1);
	else if (var[0] == '_')
		return (1);
	else if ((var[0]) == '"' && var[ft_strlen(var) - 1] == '"')
		return (1);
	return (0);
}

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

// Extracted function to handle variables with values
void	handle_value_variable(t_shell *shell, const char *input,
		const char *equal_pos)
{
	char	*var_name;
	char	*var_value;
	char	*full_var;
	char	*complete_var;

	var_name = ft_substr(input, 0, equal_pos - input);
	var_value = ft_strdup(equal_pos + 1);
	if (var_name && var_value)
	{
		full_var = ft_strjoin(var_name, "=");
		if (full_var)
		{
			complete_var = ft_strjoin(full_var, var_value);
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

// Extracted function to handle variables without values
void	handle_no_value_variable(t_shell *shell, const char *input)
{
	char	*no_value;

	no_value = ft_strdup(input);
	if (no_value)
	{
		add_node(&shell->export, no_value);
		free(no_value);
	}
}

void	handle_variable_input(t_shell *shell, char *input)
{
	char	*equal_pos;

	equal_pos = ft_strchr(input, '=');
	if (!equal_pos)
		handle_no_value_variable(shell, input);
	else
		handle_value_variable(shell, input, equal_pos);
}

int	builtin_export(t_shell *shell, char **args)
{
	int		i;
	char	*input;

	if (!args[1])
	{
		print_export(&shell);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		input = args[i];
		if (!check_in(input))
		{
			i++;
			continue ;
		}
		handle_variable_input(shell, input);
		i++;
	}
	return (0);
}
