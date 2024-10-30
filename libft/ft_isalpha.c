/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 14:17:50 by drenquin          #+#    #+#             */
/*   Updated: 2024/02/27 14:30:34 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
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
	char  c3 = 's';
	char  c4 = '5';


	printf ("c1 renvoie %d\n",ft_isalpha(c1));
	printf ("c2 renvoie %d\n",ft_isalpha(c2));
	printf ("c3 renvoie %d\n",ft_isalpha(c3));
	printf ("c4 renvoie %d\n",ft_isalpha(c4));
}*/
