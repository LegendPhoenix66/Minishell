/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:45:46 by drenquin          #+#    #+#             */
/*   Updated: 2024/02/27 18:11:21 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

/*unsigned int	ft_strlen(const char *str)
{
	unsigned int	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}*/

char	*ft_strdup(const char *s)
{
	int		i;
	int		len_s;
	char	*new_s;

	i = 0;
	len_s = ft_strlen(s);
	new_s = (char *) malloc (sizeof(char) * (len_s + 1));
	if (new_s == NULL)
	{
		return (NULL);
	}
	while (*s)
	{
		new_s[i] = *s;
		i++;
		s++;
	}
	new_s[i] = '\0';
	return (new_s);
}
/*
#include <stdio.h>
#include <string.h>
int main(int ac,char **av)
{
	char *ma_fonction;
	char *fonction_nat;

	if (ac == 2)
	{
		ma_fonction = ft_strdup(av[1]);
		fonction_nat = strdup (av[1]);
		printf ("mafonction renvoie :%s: strdup renvoie :%s:",//
		free(ma_fonction);
		free(fonction_nat);
	}
	return (0);
}*/
