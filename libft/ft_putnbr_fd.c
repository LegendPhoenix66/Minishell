/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 21:08:00 by drenquin          #+#    #+#             */
/*   Updated: 2024/03/03 21:16:22 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	nbl;
	short	i;
	char	buffer[10];

	i = 0;
	nbl = (long)n;
	if (0 == n)
	{
		write (fd, "0", 1);
		return ;
	}
	if (nbl < 0)
	{
		nbl *= -1;
		write(fd, "-", 1);
	}
	while (nbl)
	{
		buffer[i++] = (nbl % 10) + '0';
		nbl = nbl / 10;
	}
	while (i > 0)
		write (fd, &buffer[--i], 1);
}
