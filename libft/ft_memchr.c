/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 12:01:13 by drenquin          #+#    #+#             */
/*   Updated: 2024/03/11 13:04:04 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			j;
	unsigned char	*str;

	str = (unsigned char *)s;
	j = 0;
	if (n == 0)
		return (NULL);
	while (n-- > 0)
	{
		if (str[j] == (unsigned char)c)
			return (str + j);
		j++;
	}
	return (NULL);
}
/*
#include <string.h>
#include <stdio.h>
int main ()
{
	char str [] = "essaie de la fonction";
	char *result = memchr(str,'i',10);
	char *result1 = ft_memchr(str,'i',10);
	printf("resultat avec la fonction native: %s\n",result);
	printf("resultat avec ma fonction: %s\n",result1);
	return(0);
}*/
