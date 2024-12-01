/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:47:06 by lhopp             #+#    #+#             */
/*   Updated: 2024/02/27 16:38:56 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	power_of_ten(int n)
{
	int	power;

	power = 1;
	while (n-- > 0)
	{
		power *= 10;
	}
	return (power);
}

static int	calculate_length(int n)
{
	int	length;

	length = 0;
	while (n != 0)
	{
		n /= 10;
		length++;
	}
	return (length);
}

static void	print_number(int n, int length, int fd)
{
	int	power;
	int	digit;

	while (length-- > 0)
	{
		power = power_of_ten(length);
		digit = (n / power) % 10;
		ft_putchar_fd((char)('0' + digit), fd);
	}
}

void	ft_putnbr_fd(int n, int fd)
{
	int	length;

	if (n == 0)
	{
		ft_putchar_fd('0', fd);
		return ;
	}
	if (n == -2147483648)
	{
		ft_putstr_fd("-2147483648", fd);
		return ;
	}
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n = -n;
	}
	length = calculate_length(n);
	print_number(n, length, fd);
}
