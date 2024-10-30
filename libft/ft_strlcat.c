/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:00:36 by drenquin          #+#    #+#             */
/*   Updated: 2024/03/06 19:45:45 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

unsigned int	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t			i;
	size_t			j;
	size_t			len;

	i = 0;
	j = 0;
	len = ft_strlen(dst) + ft_strlen(src);
	if (size <= ft_strlen(dst))
		return (size + ft_strlen(src));
	while (dst[i] != '\0')
	{
		i++;
	}
	while (src[j] != '\0' && i < size - 1)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (len);
}
/*#include<stdio.h>
int main()
{
	char destination[100000]="daniel";
	const char source[]="renquin";
	int nb = ft_strlcat(destination,source,15*sizeof(char));
	printf("%s\n",destination);
	printf("%d\n",nb);
	return(0);
}*/
