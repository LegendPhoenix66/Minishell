/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 15:40:35 by drenquin          #+#    #+#             */
/*   Updated: 2024/02/22 09:20:47 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*str;

	i = 0;
	str = (char *)s;
	while (i < n)
	{
		str[i] = '\0';
		i++;
	}
}
/*
#include<stdio.h>

int main()
{
	char str [50] = "salut l ami comment va tu";
	printf ("avant bzero:%s\n",str);
	ft_bzero(str + 4,2*sizeof(char));
	printf ("apres bzero:%s\n",str);
	return(0);
}
*/
