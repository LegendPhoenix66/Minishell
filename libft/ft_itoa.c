/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:28:45 by lhopp             #+#    #+#             */
/*   Updated: 2024/02/27 13:49:16 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	fill_str_with_digits(char *str, int len, int n, int is_negative)
{
	while (len--)
	{
		str[len + is_negative] = '0' + (n % 10) * (1 - 2 * is_negative);
		n /= 10;
	}
}

char	*ft_itoa(int n)
{
	int		len;
	int		temp;
	char	*str;
	int		is_negative;

	len = 1;
	temp = n;
	is_negative = (n < 0);
	while (temp)
	{
		temp /= 10;
		if (temp)
			len++;
	}
	str = malloc(len + 1 + is_negative);
	if (!str)
		return (NULL);
	str[len + is_negative] = '\0';
	if (is_negative)
		str[0] = '-';
	fill_str_with_digits(str, len, n, is_negative);
	return (str);
}
