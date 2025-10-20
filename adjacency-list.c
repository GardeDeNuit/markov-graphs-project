//
// Created on 20/10/2025.
//
#include "adjacency-list.h"

t_adjacency_list createEmptyAdjacencyList(int size) {
    t_adjacency_list alist;
    alist.values = (t_list *)malloc(size * sizeof(t_list));
    if (alist.values == NULL) {
        perror("[AdjacencyList] Error allocating array memory.");
        exit(EXIT_FAILURE);
    }
    alist.size = size;
    for (int i = 0; i < size; i++) {
        alist.values[i] = createEmptyList();
    }
    return alist;
}



void displayAdjacencyList(t_adjacency_list alist) {
    for (int i = 0; i < alist.size; i++) {
        printf("List of vertex %d: ", i + 1);
        displayList(alist.values[i]);
    }
}

int testcreateEmptyAdjacencyList(void) {
    return 1;
}