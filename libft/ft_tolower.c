/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 16:38:01 by drenquin          #+#    #+#             */
/*   Updated: 2024/02/27 16:40:19 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_tolower(int c)
{
	while (c >= 65 && c <= 90)
	{
		c += 32;
		return (c);
	}
	return (c);
}
/*
#include<ctype.h>
#include<stdio.h>

int main ()
{
    printf("Test 1: %c\n",ft_tolower('a'));  // Doit afficher 'a'
    printf("Test 2: %c\n", ft_tolower('Z'));  // Doit afficher 'z'
    printf("Test 3: %c\n", ft_tolower('3'));  // Ne doit pas changer

    return (0);
}*/
