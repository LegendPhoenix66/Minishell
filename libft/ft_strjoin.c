/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:17:34 by drenquin          #+#    #+#             */
/*   Updated: 2024/03/12 10:09:21 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t				lenght;
	char				*join;
	unsigned int		i;
	unsigned int		j;

	j = 0;
	i = 0;
	lenght = ft_strlen(s1) + ft_strlen(s2);
	join = malloc((lenght + 1) * (sizeof(char)));
	if (join == NULL)
		return (NULL);
	while (ft_strlen(s1) > i)
	{
		join[i] = s1[i];
		i++;
	}
	while (s2[j])
		join[i++] = s2[j++];
	join[i] = '\0';
	return (join);
}

/*#include<stdio.h>

int main(void)
{
    const char *str1 = "Hello,";
    const char *str2 = "world!";
    char *result = ft_strjoin(str1, str2);
    if (result)
    {
        printf("Concatenated string: %s\n", result);
        free(result); // Don't forget to free the allocated memory!
    }
    else
    {
        printf("Memory allocation failed.\n");
    }
    return 0;
}*/
