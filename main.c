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

    t_matrix matrix;
    createMatrixFromGraph(graph, &matrix);
    displayMatrix(matrix);

    t_matrix distribution = createMatrix(1, matrix.cols);
    setMatrixValue(&distribution, 0, 0, 1.0);

    displayConvergedMatrixPower(matrix, distribution, 0.0001,2000);

    return 1;
}