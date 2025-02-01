/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <drenquin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 19:58:52 by drenquin          #+#    #+#             */
/*   Updated: 2025/02/01 08:53:37 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_strcmp(char *str1, char *str2)
{
	int	i;

	i = 0;
	while (str1[i] == str2[i] && str1[i] && str2[i])
		i++;
	return (str1[i] - str2[i]);
}

char    *extract_from_dollar(char *str)
{
    char    *result;
    int     start;
    int     len;
    int     i;

    if (!str)
        return (NULL);
    start = find_index(str);
    len = 0;
    while (str[start + len])
        len++;
    result = (char *)malloc(sizeof(char) * (len + 1));
    if (!result)
        return (NULL);
    i = 0;
    while (str[start + i])
    {
        result[i] = str[start + i];
        i++;
    }
    result[i] = '\0';
    return (result);
}
