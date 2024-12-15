/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 14:34:58 by lhopp             #+#    #+#             */
/*   Updated: 2024/02/27 13:50:57 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*dest_c;
	unsigned char	*src_c;
	size_t			i;

	dest_c = (unsigned char *)dest;
	src_c = (unsigned char *)src;
	if (dest_c < src_c)
	{
		i = 0;
		while (i < n)
		{
			dest_c[i] = src_c[i];
			i++;
		}
	}
	else
	{
		while (n--)
		{
			dest_c[n] = src_c[n];
		}
	}
	return (dest);
}
