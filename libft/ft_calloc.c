/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 09:23:34 by drenquin          #+#    #+#             */
/*   Updated: 2024/03/10 17:02:43 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t		i;
	int			zero;
	char		*array;

	i = 0;
	zero = 0;
	if (nmemb == 0 || size == 0)
	{
		array = (char *)malloc(1);
		if (array == NULL)
			return (NULL);
		array[0] = zero;
		return (array);
	}
	if (size > (4294967295 / nmemb))
		return (NULL);
	array = (char *) malloc (nmemb * size);
	if (array == NULL)
		return (NULL);
	while (i < (nmemb * size))
		array[i++] = zero;
	return (array);
}
/*
#include <stdio.h>
int main() {
    int *tableau;
    int taille = 10; // Nombre d'éléments dans le tableau

    // Allocation dynamique de mémoire pour le tableau avec calloc
    tableau = (int *)ft_calloc(taille, sizeof(int));

    // Vérification si l'allocation a réussi
    if (tableau == NULL) {
        fprintf(stderr, "Allocation de mémoire a échoué\n");
        return 1; // Quitter le programme avec un code d'erreur
    }

    // Affichage des valeurs du tableau (initialement à zéro)
    printf("Valeurs du tableau (initialisées à zéro) : ");
    for (int i = 0; i < taille; ++i) {
        printf("%d ", tableau[i]);
    }
    printf("\n");

    // Libération de la mémoire allouée avec calloc
    free(tableau);

    return 0; // Fin normale du programme
}*/		
