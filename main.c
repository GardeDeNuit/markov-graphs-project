#include <stdio.h>
#include "graph.h"
#include "mermaidchart-file-generator.h"
#include "utils.h"
#include "partition.h"
#include "tests.h"
#include <string.h>
#include "matrix.h"
#include "tarjan.h"

int main(void) {

    t_graph graph = importGraphFromFile("../test_bench/exemple_valid_step3.txt");
    if (graph.size == 0) {
        fprintf(stderr, "Failed to import graph from file.\n");
        return -1;
    }

    printf("\n ===== PART 1 ===== \n");

    printf("\n Validation 1 : We want to display the Markov graph/adjacency list of our text file in the console. \n \n");
    printf(" === Display of a Markov graph === \n\n");
    displayGraph(graph);

    printf("\n Validation 2 : We want to know whether our graph properly satisfies the characteristics of Markov with the function isMarkovGraph \n \n");
    printf(" === Verification of the characteristics === \n\n");
    isMarkovGraph(graph);

    printf("\n Validation 3 : We want to display our Markov graph more clearly on Mermaid. \n");
    exportGraphToMermaidFile(graph, "../results/exemple_valid_step3_mermaid.mmd");


    printf("\n ===== PART 2 ===== \n");

    printf("\n Validation 1 : We want to display the distribution of the graph’s classes created using Tarjan’s algorithm. \n");
    t_partition *partition = tarjan(graph);

    printf(" === display of the classes === \n\n");
    displayPartition(partition);


    printf("\n Validation 2 : “Production of a Hasse diagram to display the links between each class. \n\n");
    t_hasse_diagram hasse = createHasseDiagram(graph);

    displayHasseDiagram(hasse);


    exportHasseDiagramToMermaidFile(hasse, "../results/exemple_valid_step3_hasse_mermaid.mmd");
    /*printf("Irreductible ? : %d\n", isIrreductible(hasse));
    printf("Persistant A ? : %d\n", isPersistantClass(hasse, 1));
    printf("Persistant C ? : %d\n", isPersistantClass(hasse, 3));
    printf("Absorbing ? : %d\n", isAbsorbingState(hasse, 4, graph.size));*/

    printf("\n Validation 3 : We want to know the state of the graph’s classes. \n");
    displayDetailedCharacteristics(hasse,graph.size);

    printf("\n ===== PART 3 ===== \n\n");

    printf(" Validation 1 : We want to calculate 𝑀, for which the difference between 𝑀 and 𝑀 is less than 𝜀 = 0.01. \n\n");
    t_matrix matrix;
    createMatrixFromGraph(graph, &matrix);
    dipslayConvergedMatrixPower(matrix, 0.01, 200);

    printf("\n Validation 2 : We want to obtain the stationary distributions for each of the graph’s classes.\n");
    computeStationaryDistributionsForAllClasses(matrix,*partition,hasse,0.01);

    /*
    // Enregistrer les tests du projet puis exécuter tous les tests.
    register_project_tests();
    int failures = run_all_tests();

    // Retourner le nombre d'échecs comme code de sortie (0 = tout OK)
    return failures;
    return 0;
    */
}