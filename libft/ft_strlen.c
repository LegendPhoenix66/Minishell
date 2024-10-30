/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 09:38:06 by drenquin          #+#    #+#             */
/*   Updated: 2024/02/21 10:54:23 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

unsigned int	ft_strlen(const char *str)
{
	unsigned int	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}
/*
#include<stdio.h>
int main ()
{
	char str [] = "salut";
	char str1 [] = "dan iel ";

	printf("str contient %d caracteres\n",ft_strlen(str));
	printf("str1 contient %d caracteres\n",ft_strlen(str1));
	
	return (0);
}*/
