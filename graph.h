#ifndef GRAPH_H
#define GRAPH_H

#include "adjacency-list.h"

typedef t_adjacency_list t_graph;

// Crée un graphe vide de la taille donnée
t_graph createGraph(int size);

// Affiche le graphe
void displayGraph(t_graph graph);

// Ajoute une arête directed src -> dest avec un poids
void addEdge(t_graph *graph, int src, int dest, double weight);

// Lit un graphe à partir d'un fichier
t_graph readGraphFromFile(const char* path);


#endif //GRAPH_H
