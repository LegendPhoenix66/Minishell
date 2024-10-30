/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 13:54:35 by drenquin          #+#    #+#             */
/*   Updated: 2024/02/27 16:23:48 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

unsigned int	ft_strlcpy(char *dst, const char *src, size_t size)
{
	int			i;
	size_t		zero;
	size_t		src_size;

	i = 0;
	zero = 0;
	src_size = ft_strlen(src);
	if (size != 0)
	{
		while (src[i] && (zero < size - 1))
		{
			dst[i] = src[i];
			i++;
			size--;
		}
		dst[i] = '\0';
	}
	return (src_size);
}
/*#include <stdio.h>
int main ()
{
	char src [] = "salut l' ami comment";
	char dest [50];
	int nb_c = ft_strlcpy(dest,src,7);
	printf("la destination est:%s\n",dest);
	printf("le nombre de caracteres est: %d\n",nb_c);
	return (0);
}*/
