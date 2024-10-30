/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:38:02 by drenquin          #+#    #+#             */
/*   Updated: 2024/02/27 14:33:01 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	else
		return (0);
}
/*
#include <stdio.h>
int main ()
{
	char c1 = 'A';
	char c2 = 'd';
	char c3 = '0';
	char c4 = '9';
	char c5 = '5';

	printf ("c1 renvoie %d\n",ft_isdigit(c1));
	printf ("c2 renvoie %d\n",ft_isdigit(c2));
	printf ("c3 renvoie %d\n",ft_isdigit(c3));
	printf ("c4 renvoie %d\n",ft_isdigit(c4));
	printf ("c5 renvoie %d\n",ft_isdigit(c5));

	return(0);
}*/
