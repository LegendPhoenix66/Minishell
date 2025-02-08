/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:30:31 by lhopp             #+#    #+#             */
/*   Updated: 2024/02/27 13:59:38 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	calculate_indices(char const *s1, char const *set, size_t *start,
		size_t *end)
{
	while (*start < *end && ft_strchr(set, s1[*start]))
	{
		(*start)++;
	}
	while (*end > *start && ft_strchr(set, s1[*end - 1]))
	{
		(*end)--;
	}
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*trimmed_str;

	if (!s1 || !set)
	{
		return (NULL);
	}
	start = 0;
	end = ft_strlen(s1);
	calculate_indices(s1, set, &start, &end);
	if (start >= end)
	{
		return (ft_strdup(""));
	}
	trimmed_str = ft_substr(s1, start, end - start);
	return (trimmed_str);
}
