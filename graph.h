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

// Vérifie si l'arête src -> dest existe (1 = oui, 0 = non)
int hasEdge(t_graph graph, int src, int dest);

// Retourne la liste des voisins du sommet src (ou NULL en cas d'erreur)
t_list* getNeighbors(t_graph *graph, int src);

// Libère toute la mémoire associée au graphe
void freeGraph(t_graph *graph);

// Lit un graphe à partir d'un fichier
t_graph importGraphFromFile(const char* path);

int is_graphMarkov(t_graph);

#endif //GRAPH_H
