/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 11:22:20 by drenquin          #+#    #+#             */
/*   Updated: 2024/02/24 11:53:36 by drenquin         ###   ########.fr       */
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

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen (s);
	if (c > 256)
		c = (c % 256);
	while (i >= 0)
	{
		if (s[i] == c)
		{
			return ((char *)(s + i));
		}
		i--;
	}
	return (0);
}
/*
#include <stdio.h>

int main ()
{
        char test [] = "ceci zest un zest de citron";
        char *result;
        result = ft_strrchr (test, 'z');
        printf ("%s\n",result);

        return (0);
}*/
