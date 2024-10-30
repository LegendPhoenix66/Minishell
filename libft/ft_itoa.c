/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 13:03:24 by drenquin          #+#    #+#             */
/*   Updated: 2024/03/11 13:02:00 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static size_t	array_size(int nbr)
{
	int	i;

	i = 0;
	if (nbr == 0)
		return (1);
	if (nbr < 0)
		i++;
	while (nbr != 0)
	{
		nbr /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	size_t			len;
	char			*tab;
	long long		nbr;

	len = array_size(n);
	nbr = (long long)n;
	tab = (char *)malloc(sizeof(char) * (len + 1));
	if (tab == NULL)
		return (NULL);
	if (n < 0)
	{
		tab[0] = '-';
		nbr *= -1;
	}
	tab[len--] = '\0';
	while (nbr > 0)
	{
		tab[len--] = (nbr % 10) + '0';
		nbr /= 10;
	}
	if (n == 0)
		tab[0] = '0';
	return (tab);
}
/*#include <stdio.h>
int main() {
    int number = -548;
    int number1 = 0;
    int number2 = 951;

    char *result = ft_itoa(number);
    char *result1 = ft_itoa(number1);
    char *result2 = ft_itoa(number2);

    if (result != NULL) {
        printf("Conversion de %d en chaîne : %s\n", number, result);
	printf("Conversion de %d en chaîne : %s\n", number1, result1);
	printf("Conversion de %d en chaîne : %s\n", number2, result2);
        free(result);
	free(result1);
	free(result2);
    } else {
        printf("Erreur d'allocation de mémoire.\n");
    }

    return 0;

    }
#include <stdio.h>

static size_t array_size(int nbr);

int main(void)
{
    // Exemples d'utilisation de la fonction array_size
    int num1 = 12345;
    int num2 = -9;
    int num3 = 0;

    // Calcul de la taille de chaque nombre
    size_t size1 = array_size(num1);
    size_t size2 = array_size(num2);
    size_t size3 = array_size(num3);

    // Affichage des résultats
    printf("Taille de %d : %zu\n", num1, size1);
    printf("Taille de %d : %zu\n", num2, size2);
    printf("Taille de %d : %zu\n", num3, size3);

    return 0;
}
*/
