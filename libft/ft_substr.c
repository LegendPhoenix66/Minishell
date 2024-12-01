/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:30:42 by lhopp             #+#    #+#             */
/*   Updated: 2024/02/27 14:07:51 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	calculate_actual_len(char const *s, unsigned int start,
		size_t len)
{
	size_t	s_len;
	size_t	actual_len;

	s_len = ft_strlen(s);
	if (start >= s_len)
	{
		return (0);
	}
	actual_len = (s_len - start);
	if (len < actual_len)
	{
		actual_len = len;
	}
	return (actual_len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	actual_len;
	char	*substr;

	if (!s)
	{
		return (NULL);
	}
	actual_len = calculate_actual_len(s, start, len);
	if (actual_len == 0)
	{
		return (ft_strdup(""));
	}
	substr = malloc(actual_len + 1);
	if (!substr)
	{
		return (NULL);
	}
	ft_memcpy(substr, s + start, actual_len);
	substr[actual_len] = '\0';
	return (substr);
}
