/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:34:23 by drenquin          #+#    #+#             */
/*   Updated: 2024/02/27 16:37:50 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 97 && c <= 122)
	{
		c -= 32;
		return (c);
	}
	return (c);
}
/*
#include<ctype.h>
#include<stdio.h>

int main ()
{
    printf("Test 1: %c\n", ft_toupper('a'));  // Doit afficher 'A'
    printf("Test 2: %c\n", ft_toupper('Z'));  // Doit afficher 'Z'
    printf("Test 3: %c\n", ft_toupper('3'));  // Ne doit pas changer
    return (0);
}*/
