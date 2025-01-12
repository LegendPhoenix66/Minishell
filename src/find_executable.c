/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_executable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 15:54:55 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/12 15:54:55 by drenquin         ###   ########.fr       */
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
char    *build_path(const char *dir, const char *command)
{
    char    *temp;
    char    *result;

    temp = ft_strjoin(dir, "/");
    if (!temp)
        return (NULL);
    result = ft_strjoin(temp, command);
    free(temp);
    return (result);
}
// Helper to search for a command in PATH
char	*find_executable(const char *command)
{
	char	*path;
	char	*dirs;
	char	*dir;
	char	*full_path;

	path = getenv("PATH");
	if (!path)
	{
		perror("Error: PATH not set\n");
		return (NULL);
	}
	dirs = ft_strdup(path);
	if (!dirs)
	{
		perror("strdup failed");
		return (NULL);
	}
	dir = ft_strtok(dirs, ":");
	while (dir)
	{
        full_path = build_path(dir, command);
		if (access(full_path, X_OK) == 0)
		{
			free(dirs);
			return (full_path);
		}
		free(full_path);
		dir = ft_strtok(NULL, ":");
	}
	free(dirs);
	return (NULL);
}
