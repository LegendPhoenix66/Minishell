/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:44:07 by drenquin          #+#    #+#             */
/*   Updated: 2024/02/27 14:34:46 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	else
		return (0);
}
/*
#include <stdio.h>
int main ()
{
        char c1 = '.' ;
        char c2 = 'A';
        char c3 = 'z';
        char c4 = '7';
        int c5 = 187;

        printf ("c1 renvoie %d\n",ft_isascii(c1));
        printf ("c2 renvoie %d\n",ft_isascii(c2));
        printf ("c3 renvoie %d\n",ft_isascii(c3));
        printf ("c4 renvoie %d\n",ft_isascii(c4));
        printf ("c5 renvoie %d\n",ft_isascii(c5));

        return(0);
}
*/
