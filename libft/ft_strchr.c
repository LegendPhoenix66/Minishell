/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 09:43:36 by drenquin          #+#    #+#             */
/*   Updated: 2024/02/24 11:45:46 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	if (c > 256)
		c = (c % 256);
	while (s[i] != c && s[i])
	{
		if (s[i] == '\0')
		{
			if (c == '\0')
				return ((char *)(s + i));
			return (NULL);
		}
		i++;
	}
	if (s[i] == c)
		return ((char *)(s + i));
	else
		return (NULL);
}

/*#include <stdio.h>
int main ()
{
	char test [] = "ceci zest un zest de citron";
	char *result;
	result = ft_strchr (test, 'z');
	printf ("%s\n",result);

	return (0);
}*/
