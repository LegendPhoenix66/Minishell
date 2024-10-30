/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:10:14 by drenquin          #+#    #+#             */
/*   Updated: 2024/02/21 10:33:28 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_isalnum(int val)
{
	if ((val >= 65 && val <= 90) || (val >= 97 && val <= 122))
		return (1);
	else if (val >= 48 && val <= 57)
		return (1);
	else
		return (0);
}
/*
#include <stdio.h>
int main ()
{
	char c1 = ' ';
	char c2 = 'A';
	char c3 = '+';
	char c4 = '7';
	int c5 = 187;

	printf ("c1 renvoie %d\n",ft_isalnum(c1));
	printf ("c2 renvoie %d\n",ft_isalnum(c2));
	printf ("c3 renvoie %d\n",ft_isalnum(c3));
	printf ("c4 renvoie %d\n",ft_isalnum(c4));
	printf ("c5 renvoie %d\n",ft_isalnum(c5));

	return(0);
}*/
