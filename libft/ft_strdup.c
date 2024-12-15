/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:21:56 by lhopp             #+#    #+#             */
/*   Updated: 2024/02/27 13:51:25 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*new_str;
	char	*new_str_copy;

	if (s == NULL)
		return (NULL);
	len = ft_strlen(s);
	new_str = (char *)malloc(len + 1);
	if (new_str == NULL)
		return (NULL);
	new_str_copy = new_str;
	while (*s)
	{
		*new_str_copy = *s;
		new_str_copy++;
		s++;
	}
	*new_str_copy = '\0';
	return (new_str);
}
