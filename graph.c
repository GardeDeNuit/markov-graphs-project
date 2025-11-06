//
// Created by matteo on 20/10/2025.
//

#include "graph.h"

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
    if (graph->values == NULL) {
        fprintf(stderr, "addEdge: graph values not initialized\n");
        return;
    }
    if (src < 1 || src > graph->size) {
        fprintf(stderr, "addEdge: source vertex %d out of range [1..%d]\n", src, graph->size);
        return;
    }
    if (dest < 1 || dest > graph->size) {
        fprintf(stderr, "addEdge: dest vertex %d out of range [1..%d]\n", dest, graph->size);
        return;
    }
    // addCell expects a t_list* for the source vertex
    addCell(&graph->values[src - 1], dest, weight);
}

// Retourne la liste des voisins du sommet src (ou NULL en cas d'erreur)
t_list* getNeighbors(t_graph *graph, int src) {
    if (graph == NULL) {
        fprintf(stderr, "getNeighbors: graph is NULL\n");
        return NULL;
    }
    if (graph->values == NULL) {
        fprintf(stderr, "getNeighbors: graph values not initialized\n");
        return NULL;
    }
    if (src < 1 || src > graph->size) {
        fprintf(stderr, "getNeighbors: source vertex %d out of range [1..%d]\n", src, graph->size);
        return NULL;
    }
    return &graph->values[src - 1];
}

// Vérifie si l'arête src -> dest existe (1 = oui, 0 = non)
int hasEdge(t_graph graph, int src, int dest) {
    if (graph.values == NULL) return 0;
    if (src < 1 || src > graph.size) return 0;

    // Parcourir directement la liste d'adjacence du sommet src (index src-1)
    t_cell *curr = graph.values[src - 1].head;
    while (curr != NULL) {
        if (curr->vertex == dest) return 1;
        curr = curr->next;
    }
    return 0;
}

// Libère toute la mémoire associée au graphe
void freeGraph(t_graph *graph) {
    if (graph == NULL) return;
    freeAdjacencyList(graph);
}

// Lit un graphe à partir d'un fichier
t_graph importGraphFromFile(const char* path) {
    FILE *file = fopen(path, "rt"); // read-only, text
    int nbvert, src, dest;
    double weight;
    t_graph graph;
    if (file==NULL){
        perror("importGraphFromFile: Could not open file for reading");
        return createGraph(1);
    }

    // first line contains number of vertices
    if (fscanf(file, "%d", &nbvert) != 1){
        fprintf(stderr, "importGraphFromFile: Could not read number of vertices from %s\n", path);
        fclose(file);
        return createGraph(1);
    }

    // valider le nombre de sommets lu
    if (nbvert < 1) {
        fprintf(stderr, "importGraphFromFile: invalid number of vertices (%d) in %s\n", nbvert, path);
        fclose(file);
        return createGraph(1);
    }

    graph = createGraph(nbvert);

    while (fscanf(file, "%d %d %lf", &src, &dest, &weight) == 3){
        if (src < 1 || src > graph.size || dest < 1 || dest > graph.size) {
            fprintf(stderr, "importGraphFromFile: edge with invalid vertices (%d -> %d) ignored\n", src, dest);
            continue;
        }
        addEdge(&graph, src, dest, weight);
    }
    fclose(file);
    return graph;
}



int is_graphMarkov(t_graph graph){
    double sum = 0.00;
    int isMarkov = 1;
    for (int i=0;i<graph.size;i++){
        sum = sumValues(graph.values[i]);
        if (sum < 0.99 || sum > 1.00) {
            if (isMarkov) printf("Le graphe n'est pas un graphe de Markov.\n");
            printf("La somme des probabilités du sommet %d est %.5f\n", i+1, sum);
            isMarkov = 0;
        }
    }

    if (isMarkov == 1) {
        printf("Le graphe est un graphe de Markov.\n");
    }
    return isMarkov;
}