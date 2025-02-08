/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:18:04 by lhopp             #+#    #+#             */
/*   Updated: 2024/02/27 13:50:28 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total_size;
	void	*ptr;
	char	*temp_ptr;
	size_t	i;

	if (size != 0 && nmemb > __SIZE_MAX__ / size)
	{
		return (NULL);
	}
	total_size = nmemb * size;
	ptr = malloc(total_size);
	if (ptr == NULL)
	{
		return (NULL);
	}
	temp_ptr = (char *)ptr;
	i = 0;
	while (i < total_size)
	{
		temp_ptr[i] = 0;
		i++;
	}
	return (ptr);
}
