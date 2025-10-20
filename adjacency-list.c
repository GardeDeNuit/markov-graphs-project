//
// Created on 20/10/2025.
//
#include "adjacency-list.h"

adjacency_list createEmptyAdjacencyList(int size) {
    adjacency_list alist = (adjacency_list)malloc(size * sizeof(adjacency_list));
    for (int i = 0; i < size; i++) {
        alist[i] = createEmptyList();
    }
    return alist;
}

void displayAdjacencyList(adjacency_list alist, int size) {
    for (int i = 0; i < size; i++) {
        printf("List of vertex %d: ", i + 1);
        displayList(alist[i]);
    }
}

int testcreateEmptyAdjacencyList(void) {
    return 1;
}