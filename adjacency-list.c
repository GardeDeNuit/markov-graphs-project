//
// Created on 20/10/2025.
//
#include "adjacency-list.h"

t_adjacency_list createEmptyAdjacencyList(int size) {
    t_adjacency_list alist = { .values = NULL, .size = 0 };

    if (size < MIN_SIZE_ADJACENCY_LIST) {
        fprintf(stderr, "createEmptyAdjacencyList: size must be >= %d (got %d)\n", MIN_SIZE_ADJACENCY_LIST, size);
        return alist;
    }

    alist.values = malloc((size_t)size * sizeof *alist.values);
    if (alist.values == NULL) {
        perror("createEmptyAdjacencyList: allocation failed");
        return alist;
    }

    alist.size = size;
    for (int i = 0; i < size; ++i) {
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

void freeAdjacencyList(t_adjacency_list* alist) {
    if (alist == NULL) return;
    if (alist->values == NULL) {
        alist->size = 0;
        return;
    }
    for (int i = 0; i < alist->size; ++i) {
        freeList(&alist->values[i]);
    }
    free(alist->values);
    alist->values = NULL;
    alist->size = 0;
}
