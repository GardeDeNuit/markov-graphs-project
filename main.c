#include <stdio.h>
#include "graph.h"
#include "mermaidchart-file-generator.h"
#include "utils.h"
#include "partition.h"
#include "tests.h"
#include "matrix.h"
#include "tarjan.h"

int main(void) {
    char* path = "../test_bench/exemple1.txt";
    char* graph_path = "../results/exemple_graph.mmd";
    char* hasse_path = "../results/exemple_hasse.mmd";
    int unit_tests = FALSE;

    t_graph graph = importGraphFromFile(path);
    if (graph.size == 0) {
        fprintf(stderr, "Failed to import graph from file.\n");
        return -1;
    }

    printf("\n==================================[ PART 1 ]==================================\n");

    printf("\n========= Validation 1 =========\n");
    printf("We want to display the Markov graph/adjacency list of our text file in the console.\n\n");
    printf("=== Display of a Markov graph === \n");
    displayGraph(graph);

    printf("\n========= Validation 2 =========\n");
    printf("We want to know whether our graph properly satisfies the characteristics of Markov with the function isMarkovGraph \n\n");
    printf(" === Verification of the characteristics === \n");
    isMarkovGraph(graph);

    printf("\n========= Validation 3 =========\n");
    printf("We want to display our Markov graph more clearly on Mermaid. \n\n");
    exportGraphToMermaidFile(graph, graph_path);


    printf("\n==================================[ PART 2 ]==================================\n");

    printf("\n========= Validation 1 =========\n");
    printf("We want to display the distribution of the graph’s classes created using Tarjan’s algorithm. \n\n");

    printf("=== Partition creation === \n");
    t_partition *partition = tarjan(graph);

    printf("=== Partition display === \n");
    displayPartition(partition);

    printf("\n========= Validation 2 =========\n");
    printf("Production of a Hasse diagram to display the links between each class. \n\n");

    t_hasse_diagram hasse = createHasseDiagram(graph);

    printf("=== Display of the hasse diagram === \n");
    displayHasseDiagram(hasse);
    printf("\n");

    exportHasseDiagramToMermaidFile(hasse, hasse_path);

    printf("\n========= Validation 3 =========\n");
    printf("We want to know the state of the graph’s classes. \n\n");
    displayDetailedCharacteristics(hasse,graph.size);

    printf("\n==================================[ PART 3 ]==================================\n");

    printf("\n========= Validation 1 =========\n");
    printf("We want to calculate M, for which the difference between Mn and Mn-1 is less than e = 0.01. \n\n");
    t_matrix matrix;
    createMatrixFromGraph(graph, &matrix);
    dipslayConvergedMatrixPower(matrix, 0.01, 200);

    printf("\n========= Validation 2 =========\n");
    printf("We want to obtain the stationary distributions for each of the graph’s classes.\n");
    printf("=== Stationary distributions for all classes ===\n");
    computeStationaryDistributionsForAllClasses(matrix,*partition,hasse,0.01);

    if (unit_tests == TRUE) {
        printf("\n==================================[ UNIT TESTS ]==================================\n");
        // Enregistrer les tests du projet puis exécuter tous les tests.
        register_project_tests();
        int failures = run_all_tests();

        // Retourner le nombre d'échecs comme code de sortie (0 = tout OK)
        return failures;
        return 0;
    }

}