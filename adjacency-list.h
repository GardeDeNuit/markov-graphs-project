//
// Created on 20/10/2025.
//

#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef t_list* adjacency_list;

adjacency_list createEmptyAdjacencyList(int);
void displayAdjacencyList(adjacency_list, int);

#endif //ADJACENCY_LIST_H
