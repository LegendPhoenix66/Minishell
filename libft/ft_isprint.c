/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 09:03:00 by drenquin          #+#    #+#             */
/*   Updated: 2024/02/20 09:36:22 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	else
		return (0);
}
/*
#include <stdio.h>
int main ()
{
	char a = '4';
	char b = '0';
	char c = ' ';
	char d = '~';
	char e = '\t';
	char f = '\0';
	int g = 127;

	printf ("a renvoie %d\n",ft_isprint(a));
	printf ("b renvoie %d\n",ft_isprint(b));
 	printf ("c renvoie %d\n",ft_isprint(c));
	printf ("d renvoie %d\n",ft_isprint(d));
	printf ("e renvoie %d\n",ft_isprint(e));
	printf ("f renvoie %d\n",ft_isprint(f));
	printf ("g renvoie %d\n",ft_isprint(g));

	return(0);
}
*/
