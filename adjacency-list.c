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


t_adjacency_list readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt"); // read-only, text
    int nbvert, depart, arrivee;
    float proba;
    t_adjacency_list alist;
    if (file==NULL)
    {
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }

    // first line contains number of vertices
    if (fscanf(file, "%d", &nbvert) != 1)
    {
        perror("Could not read number of vertices");
        exit(EXIT_FAILURE);
    }

    alist = createEmptyAdjacencyList(nbvert);

    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3)
    {
        // on obtient, pour chaque ligne du fichier les valeurs
        // depart, arrivee, et proba
        addCell(&alist.values[depart - 1], arrivee, proba);
      }
    fclose(file);
    return alist;
}