/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 23:31:50 by drenquin          #+#    #+#             */
/*   Updated: 2024/03/14 00:26:44 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static size_t	ft_count_argument(char const *str, char c)
{
	size_t	count;

	count = 0;
	if (!*str)
		return (0);
	while (*str)
	{
		while (*str == c)
			str++;
		if (*str)
			count++;
		while (*str != c && *str)
			str++;
	}
	return (count);
}

static char	**allocate_strings_array(char const *str, char s)
{
	char	**tab;
	int		count;

	count = ft_count_argument(str, s);
	tab = (char **)malloc(sizeof(char *) * (count + 1));
	if (tab == (NULL))
		return (NULL);
	return (tab);
}

char	**ft_split(char const *str, char c)
{
	char	**tab;
	size_t	len;
	int		i;

	tab = allocate_strings_array(str, c);
	if (!str || !tab)
		return (0);
	i = 0;
	while (*str)
	{
		while (*str == c && *str)
			str++;
		if (*str)
		{
			if (!ft_strchr(str, c))
				len = ft_strlen(str);
			else
				len = ft_strchr(str, c) - str;
			tab[i++] = ft_substr(str, 0, len);
			str += len;
		}
	}
	tab[i] = NULL;
	return (tab);
}	
