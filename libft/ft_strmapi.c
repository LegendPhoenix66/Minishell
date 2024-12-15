/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:30:22 by lhopp             #+#    #+#             */
/*   Updated: 2024/02/27 13:58:28 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	len;
	char	*mapped_str;
	size_t	i;

	if (!s || !f)
	{
		return (NULL);
	}
	len = ft_strlen(s);
	mapped_str = malloc(len + 1);
	if (!mapped_str)
	{
		return (NULL);
	}
	i = 0;
	while (i < len)
	{
		mapped_str[i] = f(i, s[i]);
		i++;
	}
	mapped_str[len] = '\0';
	return (mapped_str);
}
