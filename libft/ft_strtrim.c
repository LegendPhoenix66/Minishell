/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 14:24:23 by drenquin          #+#    #+#             */
/*   Updated: 2024/03/11 15:36:43 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	ft_trim(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	char	*result;
	int		i;

	end = ft_strlen(s1);
	start = 0;
	i = 0;
	if (!s1 || !set)
		return (0);
	while (s1[start] != '\0' && ft_trim(s1[start], set))
		start++;
	while (end > start && ft_trim(s1[end - 1], set))
		end--;
	result = (char *) malloc (sizeof (char) * (end - start + 1));
	if (result == NULL)
		return (NULL);
	while (start < end)
		result[i++] = s1[start++];
	result[i] = '\0';
	return (result);
}
/*#include <stdio.h>
int main()
{
    const char test[] = "eto be or not to be that is the questioneeezeee";
    const char set[] = "e";
    printf("ma fonction renvoie: %s", ft_strtrim(test, set));
    return 0;
}*/
