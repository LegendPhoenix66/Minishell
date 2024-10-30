/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:33:39 by drenquin          #+#    #+#             */
/*   Updated: 2024/02/22 14:33:34 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char			*destination;
	const char		*source;

	destination = (char *)dest;
	source = (const char *)src;
	while (n--)
	{
		destination[n] = source[n];
	}
	return (destination);
}
/*
--n donne geeksgeeksfor
n-- donne geeksgeeksgeek
meme reponse qu en incrementant un index comparer a n
#include <stdio.h>
int main ()
{
       char src [] = "Geeksfor";
      // char dest [50];
       memcpy(src + 5,src,strlen(src)+1);
       printf ("la src est: %s\n",src);
       return (0);
}*/
