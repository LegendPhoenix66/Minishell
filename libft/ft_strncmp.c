/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 07:35:56 by drenquin          #+#    #+#             */
/*   Updated: 2024/03/05 15:02:03 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{	
	size_t				i;
	size_t				j;
	unsigned char		*str1;
	unsigned char		*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	j = 0;
	while (str1[i] != '\0' && str2[j] != '\0' && j < n)
	{	
		if (str1[i] != str2[j])
		{
			return (str1[i] - str2[j]);
		}
		i++;
		j++;
	}
	if (j == n || (str1[i] == '\0' && str2[j] == '\0'))
	{
		return (0);
	}
	return (str1[i] - str2[j]);
}
/*
#include <stdio.h>
#include <string.h>

int ft_strncmp(char *s1, char *s2, unsigned int n);

int main() {
    char str1[] = "world!";
    char str2[] = "ellojdfkdkdkfkfk";
    unsigned int n = 7;

    int result_std = strncmp(str1, str2, n);
    printf("strncmp - Résultat : %d\n", result_std);

    int result_custom = ft_strncmp(str1, str2, n);
    printf("ft_strncmp - Résultat : %d\n", result_custom);

    if (result_std == result_custom) {
        printf("Les résultats sont identiques.\n");
    } else {
        printf("Les résultats sont différents.\n");
    }

    return 0;
}*/
