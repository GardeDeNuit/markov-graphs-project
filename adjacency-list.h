//
// Created on 20/10/2025.
//

#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct s_adjacency_list {
    t_list* values;
    int size;
};
typedef struct s_adjacency_list t_adjacency_list;

t_adjacency_list createEmptyAdjacencyList(int);
void displayAdjacencyList(t_adjacency_list);
t_adjacency_list readGraph(const char*);

#endif //ADJACENCY_LIST_H
