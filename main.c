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

    printf("\n Validation 1 : We want to display the Markov graph/adjacency list of our text file in the console. \n \n");
    printf(" === Display of a Markov graph === \n \n");
    displayGraph(graph);

    printf("\n Validation 2 : We want to know whether our graph properly satisfies the characteristics of Markov with the function isMarkovGraph \n \n");
    printf(" === Verification of the characteristics === \n\n");
    isMarkovGraph(graph);

    printf("\n Validation 3 : We want to display our Markov graph more clearly on Mermaid. \n");
    exportGraphToMermaidFile(graph, "../results/exemple_valid_step3_mermaid.mmd");

    t_partition *partition = tarjan(graph);

    displayPartition(partition);

    t_hasse_diagram hasse = createHasseDiagram(graph);

    displayHasseDiagram(hasse);

    exportHasseDiagramToMermaidFile(hasse, "../results/exemple_valid_step3_hasse_mermaid.mmd");
    /*printf("Irreductible ? : %d\n", isIrreductible(hasse));
    printf("Persistant A ? : %d\n", isPersistantClass(hasse, 1));
    printf("Persistant C ? : %d\n", isPersistantClass(hasse, 3));
    printf("Absorbing ? : %d\n", isAbsorbingState(hasse, 4, graph.size));*/

    displayDetailedCharacteristics(hasse,graph.size);



    /*
    // Enregistrer les tests du projet puis exécuter tous les tests.
    register_project_tests();
    int failures = run_all_tests();

    // Retourner le nombre d'échecs comme code de sortie (0 = tout OK)
    return failures;
    return 0;
    */
}