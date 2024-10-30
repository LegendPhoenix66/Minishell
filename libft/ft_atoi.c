/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:28:54 by drenquin          #+#    #+#             */
/*   Updated: 2024/03/06 21:18:29 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	ft_convert_int(const char *nptr, int *i)
{
	int	result;

	result = 0;
	while (nptr[*i] >= 48 && nptr[*i] <= 57)
	{
		result = result * 10 + nptr[*i] - 48;
		(*i)++;
	}
	return (result);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	res;
	int	nb_sign;

	nb_sign = 0;
	sign = 1;
	i = 0;
	while ((nptr[i] >= 7 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	while (ft_isdigit(nptr[i]) || (nptr[i] == '-') || (nptr[i] == '+'))
	{
		while ((nptr[i] == '-') || (nptr[i] == '+'))
		{
			if (nb_sign == 1)
				return (0);
			if (nptr[i++] == '-')
				sign = -1;
			nb_sign++;
		}
		res = ft_convert_int (nptr, &i);
		return (sign * res);
	}
	return (0);
}

 /*#include <stdlib.h>
 #include <stdio.h>

 int main ()
 {
    char str3[] = "+42 est la reponse";
    int resultat3 = atoi(str3);
    int res3 = ft_atoi(str3);
    printf("Entree : \"%s\", Sortie : %d\n", str3, resultat3);
    printf("Entree : \"%s\", Sortie : %d\n", str3, res3);

    char str4[] = "Pas un 54 nombre";
    int resultat4 = atoi(str4);
    int res4 = ft_atoi(str4);
    printf("Entree : \"%s\", Sortie : %d\n", str4, resultat4);
    printf("Entree : \"%s\", Sortie : %d\n", str4, res4);

    char str6[] = "-125cc";
    int resultat6 = atoi(str6);
    int res6 = ft_atoi(str6);
    printf("Entree : \"%s\", Sortie : %d\n", str6, resultat6);
    printf("Entree : \"%s\", Sortie : %d\n", str6, res6);

    char str5[] = "";
    int resultat5 = atoi(str5);
    int res5 = ft_atoi(str5);
    printf("Entree : \"%s\", Sortie : %d\n", str5, resultat5);
    printf("Entree : \"%s\", Sortie : %d\n", str5, res5);

    char str7[] = "+-152";
    int resultat7 = atoi(str7);
    int res7 = ft_atoi(str7);
    printf("Entree : \"%s\", Sortie : %d\n", str7, resultat7);
    printf("Entree : \"%s\", Sortie : %d\n", str7, res7);

    return 0;
 }*/
