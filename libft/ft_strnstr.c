/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:07:08 by drenquin          #+#    #+#             */
/*   Updated: 2024/03/06 20:48:52 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t		i;
	size_t		j;
	char		*b;
	char		*l;

	b = (char *)big;
	l = (char *)little;
	i = 0;
	j = 0;
	if (l[i] == '\0')
		return (b);
	while (b[j] != '\0')
	{
		if (b[j] == l[0])
		{
			while (b[j + i] == l[i] && (i + j) < len)
			{
				if (l[i + 1] == '\0')
					return (&b[j]);
				i++;
			}
		}
		j++;
	}
	return (NULL);
}
/*
#include <string.h>
#include <stdio.h>
int main(void)
{
    const char *big = "aaabcabcd";
    const char *little1 = "cd";
    const char *little2 = "cd";
    const char *little3 = "OpenAI";

    size_t len = 8; // La longueur maximale à considérer

    char *result1 = ft_strnstr(big, little1, len);
    char *result2 = ft_strnstr(big, little2, len);
    char *result3 = ft_strnstr(big, little3, len);

    printf("big: %s\n", big);
    printf("Result 1 (little1): %s\n", result1 ? result1 : "Not found");
    printf("Result 2 (little2): %s\n", result2 ? result2 : "Not found");
    printf("Result 3 (little3): %s\n", result3 ? result3 : "Not found");

    return 0;
}*/
