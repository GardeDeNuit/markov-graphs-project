#include <stdio.h>
#include "graph.h"
#include "mermaidchart-file-generator.h"
#include "utils.h"
#include "partition.h"
#include "matrix.h"
#include "tarjan.h"

int main(void) {
    /* à modifier selon où est situé votre fichier matrice.txt */
    const char* matrix_file = "../data/matrice_m.txt";
    const char* mermaid_file = "../results/graph_m.mmd";
    const char* hasse_file = "../results/hasse_m.mmd";

    /* Importation du graphe */
    t_graph graph = importGraphFromFile(matrix_file);

    exportGraphToMermaidFile(graph, mermaid_file);
    t_hasse_diagram hasse = createHasseDiagram(graph);
    exportHasseDiagramToMermaidFile(hasse, hasse_file);

    /*
    t_matrix matrix;
    createMatrixFromGraph(graph, &matrix);
    displayMatrix(matrix);
    int* array = NULL;
    array = (int*)malloc(sizeof(int)*6);
    array[0] = 1;
    array[1] = 4;
    array[2] = 11;
    array[3] = 18;
    array[4] = 26;
    array[5] = 27;
    t_matrix sub_matrix = buildSubMatrixFromVertices(matrix, array, 6);
    displayMatrix(sub_matrix);
    */

    return 1;
}