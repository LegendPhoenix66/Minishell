/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_executable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 15:54:55 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/12 20:27:07 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_strtok(char *str, char *separtors)
{
	static char	*item;
	char		*start_item;

	if (str != NULL)
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
			break ;
		}
		item++;
	}
	return (start_item);
}

char	*build_path(const char *dir, const char *command)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, command);
	free(temp);
	return (result);
}

char	*get_path_dirs(void)
{
	char	*path;
	char	*path_dirs;

	path = getenv("PATH");
	if (!path)
	{
		perror("Error: PATH not set\n");
		return (NULL);
	}
	path_dirs = ft_strdup(path);
	if (!path_dirs)
	{
		perror("Error: memory allocation failed\n");
	}
	return (path_dirs);
}

char	*find_executable_in_path(const char *path_dirs, const char *command)
{
	char	*current_dir;
	char	*full_path;

	current_dir = strtok((char *)path_dirs, ":");
	while (current_dir)
	{
		full_path = build_path(current_dir, command);
		if (access(full_path, X_OK) == 0)
		{
			return (full_path);
		}
		free(full_path);
		current_dir = strtok(NULL, ":");
	}
	return (NULL);
}

char	*find_executable(const char *command)
{
	char	*path_dirs;
	char	*executable_path;

	path_dirs = get_path_dirs();
	if (!path_dirs)
	{
		return (NULL);
	}
	executable_path = find_executable_in_path(path_dirs, command);
	free(path_dirs);
	return (executable_path);
}
