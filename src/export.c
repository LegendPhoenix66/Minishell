/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:58:36 by drenquin          #+#    #+#             */
/*   Updated: 2024/11/27 12:09:42 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_export(t_args **args);
void	add_or_update(const char *var, t_node **env_list);
int		check_in(const char *var);
int		quotes(const char *var);
char	*remove_quotes(const char *var);
int		find_equal(const char *var);

int	ft_export(const char *var, t_args **args)
{
	int	i;

	i = 1;
	if (var == NULL || *var == '\0')
	{
		print_export(args);
		return (0);
	}
//	 remove quotes if variable name is in
	if (var[0] == '"' && var[find_equal(var) - 1] == '"')
		var = remove_quotes(var); // attention cette fonction malloc
	if (check_in(var))
	{
		while (ft_isalnum(var[i]) || var[i] == '=' || var[i] == '_')
		{
			if (var[i] == '=')
			{
				add_or_update(var, &(*args)->env);
				return (0);
			}
			i++;
		}
		if (var[i] == '\0')
		{
			printf("variable facultative");
			// todo : facultative var
			return (0);
		}
		printf("invalide");
		return (-1);
	}
	printf("premier caractere invalide");
	return (-1);
}
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
// add double quotes for eexport printing
void	add_double_quotes(char *var)
{
	int	i;

	i = 0;
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
// printing for export fonction
void	print_export(t_args **args)
{
	t_node	*copy;
	t_node	*head;

	copy = copy_list((*args)->env);
	sort_lst(&copy);
	head = copy;
	while (copy != NULL)
	{
		add_double_quotes(copy->content);
		copy = copy->next;
	}
	free_lst(head);
}
void	add_or_update(const char *var, t_node **env_list)
{
	char	*name;
	char	*equal;
	int		i;

	name = ft_strdup(var);
	equal = ft_strchr(name, '=');
	i = 1;
	printf("equal %s\n", equal);
	printf("name %s\n", name);
	if (equal != NULL)
	{
		// the variable name is in double quotes
		if (equal[i] == '"' && equal[ft_strlen(equal) - 1] == '"')
		{
			name = remove_quotes(name);
			add_node(env_list, name);
			free(name);
		}
		// all after the export fonction is in doubles quotes
		else if (name[0] == '"' && name[ft_strlen(name) - 1] == '"')
		{
			printf("all name in double quotes\n");
		}
		// no variable name after = or varible name after =
		else if ((ft_strlen(equal) == 1 && equal[0] == '=') || quotes(var) == 0)
		{
			// to replace if variable already in list
			remove_if(env_list, name);
			add_node(env_list, name);
			free(name);
		}
	}
}
int	find_equal(const char *var)
{
	int	i;

	i = 0;
	while (var[i] != '=')
		i++;
	if (var[i] == '=')
	{
		printf("i vaut %d\n", i);
		return (i);
	}
	else
		return (-1);
}
// to know if doubles quotes are around variable name or variable value
// et if opened double quotes are also close
int	quotes(const char *var)
{
	int	i;
	int	nb_quotes;

	i = 0;
	nb_quotes = 0;
	while (var[i])
	{
		if (var[i] == '"')
			nb_quotes++;
		i++;
	}
	if (nb_quotes % 2 != 0)
		return (0);
	else if (var[0] == '"' && var[find_equal(var) - 1] == '"')
	{
		printf("first argument in double quotes\n");
		return (1);
	}
	else if (var[find_equal(var) + 1] == '"' && var[ft_strlen(var) - 1] == '"')
	{
		printf("second argument in double quotes\n");
		return (2);
	}
	return (0);
}
// for ignore quotes for export fonction
char	*remove_quotes(const char *var)
{
	int i;
	int j;
	char *new;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * ft_strlen(var) - 1);
	while (var[i])
	{
		if (var[i] == '"')
			i++;
		new[j++] = var[i++];
	}
	new[j] = '\0';
	// printf("new: %s\n", new);
	return (new);
}