#include <stdio.h>
#include "graph.h"
#include "mermaidchart-file-generator.h"
#include "utils.h"
#include "partition.h"
#include "matrix.h"
#include "tarjan.h"

int main(void) {

    /* à modifier selon où est situé votre fichier matrice.txt */
    char* matrix_file = "C:/Users/matte/OneDrive - Efrei/Efrei/2025-2026/SM301 - Probabilité/Projet/Probability_SM301_Projet_Matrice.txt";
    char* mermaid_file = "../results/exemple_graph.mmd";

    /* Importation du graphe */
    t_graph graph = importGraphFromFile(matrix_file);

    /* Exportation du graphe au format Mermaid */
    exportGraphToMermaidFile(graph, mermaid_file);

    /* Transformation du graphe en matrice d'adjacence */
    t_matrix matrix;
    createMatrixFromGraph(graph, &matrix);
    displayMatrix(matrix);

    /* Création d'une distribution initiale d'état 2 */
    t_matrix distribution = createMatrix(1, matrix.cols);
    setMatrixValue(&distribution, 0, 1, 1.0);

    /* Calculs des distributions après n étapes */
    displayDistributionAfterNSteps(matrix, distribution, 1);
    displayDistributionAfterNSteps(matrix, distribution, 2);
    displayDistributionAfterNSteps(matrix, distribution, 10);
    displayDistributionAfterNSteps(matrix, distribution, 50);

    return 1;

}