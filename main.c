#include <stdio.h>
#include "graph.h"
#include "mermaidchart-file-generator.h"
#include "utils.h"
#include "partition.h"
#include "matrix.h"
#include "tarjan.h"

int main(void) {
    /* à modifier selon où est situé votre fichier matrice.txt */
    const char* matrix_file = "C:/Users/matte/OneDrive - Efrei/Efrei/2025-2026/SM301 - Probabilité/Projet/Probability_SM301_Projet_Matrice.txt";
    const char* mermaid_file = "../results/exemple_graph.mmd";

    /* Importation du graphe */
    t_graph graph = importGraphFromFile(matrix_file);

    /* Exportation du graphe au format Mermaid */
    exportGraphToMermaidFile(graph, mermaid_file);

    /* Transformation du graphe en matrice d'adjacence */
    t_matrix matrix;
    createMatrixFromGraph(graph, &matrix);
    /*
        états : 2, 5, 12, 21 et 25
        0.12, 0.18, 0.25, 0.20, 0.25 → somme = 1.00
            setMatrixValue(&distribution, 0, 1, 0.12);
            setMatrixValue(&distribution, 0, 4, 0.18);
            setMatrixValue(&distribution, 0, 11, 0.25);
            setMatrixValue(&distribution, 0, 20, 0.20);
            setMatrixValue(&distribution, 0, 24, 0.25);

        0.10, 0.30, 0.15, 0.25, 0.20 → somme = 1.00
            setMatrixValue(&distribution, 0, 1, 0.10);
            setMatrixValue(&distribution, 0, 4, 0.30);
            setMatrixValue(&distribution, 0, 11, 0.15);
            setMatrixValue(&distribution, 0, 20, 0.25);
            setMatrixValue(&distribution, 0, 24, 0.20);

        0.22, 0.18, 0.20, 0.15, 0.25 → somme = 1.00
    */

    /* Création d'une distribution initiale */
    t_matrix distribution = createMatrix(1, matrix.cols);
    setMatrixValue(&distribution, 0, 1, 0.22);
    setMatrixValue(&distribution, 0, 4, 0.18);
    setMatrixValue(&distribution, 0, 11, 0.20);
    setMatrixValue(&distribution, 0, 20, 0.15);
    setMatrixValue(&distribution, 0, 24, 0.25);

    /* Calcul et affichage de la distribution limite */
    displayConvergedMatrixPower(matrix, distribution, 0.0001, 2000);

    return 1;
}