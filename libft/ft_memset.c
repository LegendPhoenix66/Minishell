/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 10:44:49 by drenquin          #+#    #+#             */
/*   Updated: 2024/02/20 15:10:47 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;
	char	*str;

	i = 0;
	str = (char *)s;
	while (i < n)
	{
		str[i] = c;
		i++;
	}
	return (str);
}
/*
#include <stdio.h>  
int main() 
{ 
    char str[50] = "GeeksForGeeks is for programming geeks."; 
    printf("\nBefore memset(): %s\n", str); 
  
    // Fill 8 characters starting from str[13] with '.' 
   ft_memset(str +13 , '.',8*sizeof(char)); 
  
    printf("After memset():  %s", str); 
    return 0; 
}*/		
