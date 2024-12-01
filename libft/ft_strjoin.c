/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:30:13 by lhopp             #+#    #+#             */
/*   Updated: 2024/02/27 13:51:33 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*joined_str;

	if (!s1 || !s2)
	{
		return (NULL);
	}
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	joined_str = malloc(s1_len + s2_len + 1);
	if (!joined_str)
	{
		return (NULL);
	}
	ft_memcpy(joined_str, s1, s1_len);
	ft_memcpy(joined_str + s1_len, s2, s2_len);
	joined_str[s1_len + s2_len] = '\0';
	return (joined_str);
}
