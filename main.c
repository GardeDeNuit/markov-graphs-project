#include <stdio.h>

#include "graph.h"

int main() {
    // Test affichage liste remplie
    printf("Test affichage liste remplie :\n");
    t_list list = createEmptyList();
    addCell(&list,5,0.8);
    addCell(&list,2,0.4);
    addCell(&list,1,1);
    addCell(&list,8,0.982);
    displayList(list);
    printf("\n");

    // Test affichage liste vide
    printf("Test affichage liste vide :\n");
    t_list list2 = createEmptyList();
    displayList(list2);
    printf("\n");

    printf("Test affichage liste adjacente :\n");
    int alist_size = 10;
    t_adjacency_list alist = createEmptyAdjacencyList(alist_size);
    displayAdjacencyList(alist);
    addCell(&alist.values[0], 0, 0.2);
    addCell(&alist.values[0], 1, 0.3);
    addCell(&alist.values[1], 2, 0.4);
    displayAdjacencyList(alist);
    printf("\n");

    printf("Test affichage liste adjacente depuis fichier :\n");
    t_graph graph2 = readGraphFromFile("C:/Users/matte/dev/markov-graphs-project/data/exemple1.txt");
    displayGraph(graph2);
    addEdge(&graph2, 1, 2, 0.1);
    displayGraph(graph2);
}