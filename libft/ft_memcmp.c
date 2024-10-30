/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 16:33:26 by drenquin          #+#    #+#             */
/*   Updated: 2024/03/11 13:06:42 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t						i;
	unsigned char				*str1;
	unsigned char				*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	if (n == 0)
		return (0);
	while (i < n)
	{
		if (str1[i] != str2[i])
			return ((int)(str1[i] - str2[i]));
		i++;
	}
	return (0);
}

/*#include <string.h>
#include <stdio.h>
int main ()
{
	char str1 [] = "salu";
	char str2 [] = "salu";
	int result1 = ft_memcmp(str1,str2,4);
	int result2 = memcmp(str1,str2,4);
	printf ("ma fonction renvoie: %d\n",result1);
	printf ("la fonction native renvoie: %d\n",result2);
	return (0);
}*/
