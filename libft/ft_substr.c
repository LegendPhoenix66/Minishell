/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:08:59 by drenquin          #+#    #+#             */
/*   Updated: 2024/03/11 17:22:55 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static size_t	substr(char const *s, unsigned int start, size_t len)
{
	size_t	slen;

	slen = ft_strlen(s);
	if ((slen - start) < len)
		return (slen - start);
	else
		return (len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*str;
	size_t			i;
	size_t			j;
	unsigned int	slen;
	size_t			sublen;

	slen = ft_strlen(s);
	if (!s)
		return (NULL);
	if (start > slen)
		return (ft_strdup("\0"));
	sublen = substr(s, start, len);
	str = malloc(sizeof(char) * (sublen + 1));
	if (str == NULL)
		return (NULL);
	i = start;
	j = 0;
	if (start < slen)
	{
		while (len-- && s[i] != '\0')
			str[j++] = s[i++];
	}
	str[j] = '\0';
	return (str);
}
/*#include <stdio.h>
int main()
{

    // Exemple d'utilisation
    const char* originalString = "Hello, world!";
    int startPosition = 3;
    int length = 5;
    char* subString = ft_substr(originalString, startPosition, length);

    printf("Chaîne d'origine : %s\n", originalString);
    printf("Sous-chaîne extraite : %s\n", subString);

    // N'oubliez pas de libérer la mémoire allouée
    free(subString);

    return 0;
}*/
