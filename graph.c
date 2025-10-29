//
// Created by matteo on 20/10/2025.
//

#include "graph.h"
#include <stdio.h>

// Crée un graphe vide de la taille donnée
t_graph createGraph(int size) {
    return createEmptyAdjacencyList(size);
}

// Affiche le graphe
void displayGraph(t_graph graph) {
    displayAdjacencyList(graph);
}

// Ajoute une arête directed src -> dest avec un poids
void addEdge(t_graph *graph, int src, int dest, double weight) {
    if (graph == NULL) {
        fprintf(stderr, "addEdge: graph is NULL\n");
        return;
    }
    if (src < 1 || src > graph->size) {
        fprintf(stderr, "addEdge: source vertex %d out of range [1..%d]\n", src, graph->size);
        return;
    }
    // addCell expects a t_list* for the source vertex
    addCell(&graph->values[src - 1], dest, weight);
}

// Lit un graphe à partir d'un fichier
t_graph readGraphFromFile(const char* path) {
    FILE *file = fopen(path, "rt"); // read-only, text
    int nbvert, src, dest;
    double weight;
    t_graph graph;
    if (file==NULL){
        perror("readGraphFromFile: Could not open file for reading");
        exit(EXIT_FAILURE);
    }

    // first line contains number of vertices
    if (fscanf(file, "%d", &nbvert) != 1){
        perror("readGraphFromFile: Could not read number of vertices");
        exit(EXIT_FAILURE);
    }

    graph = createEmptyAdjacencyList(nbvert);

    while (fscanf(file, "%d %d %f", &src, &dest, &weight) == 3){
        // on obtient, pour chaque ligne du fichier les valeurs
        // src, dest, et weight
        addEdge(&graph, src, dest, weight);
    }
    fclose(file);
    return graph;
}
