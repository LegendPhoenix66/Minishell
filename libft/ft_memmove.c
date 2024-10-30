/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 10:35:14 by drenquin          #+#    #+#             */
/*   Updated: 2024/03/11 10:54:15 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*destination;
	unsigned char	*source;
	unsigned int	i;

	destination = (unsigned char *)dest;
	source = (unsigned char *)src;
	i = 0;
	if (source < destination)
	{
		while (n--)
			destination[n] = source[n];
	}
	else
	{
		while (n--)
		{
			destination[i] = source[i];
			i++;
		}
	}
	return (destination);
}
