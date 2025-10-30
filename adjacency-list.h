//
// Created on 20/10/2025.
//

#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define MIN_SIZE_ADJACENCY_LIST 1

struct s_adjacency_list {
    t_list* values;
    int size;
};
typedef struct s_adjacency_list t_adjacency_list;

// Crée une liste d'adjacence vide de la taille donnée
t_adjacency_list createEmptyAdjacencyList(int);
// Affiche la liste d'adjacence
void displayAdjacencyList(t_adjacency_list);
// Libère toutes les listes internes et le tableau
void freeAdjacencyList(t_adjacency_list*);

#endif //ADJACENCY_LIST_H
