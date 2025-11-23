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

    t_graph graph = importGraphFromFile("../test_bench/exemple2.txt");
    if (graph.size == 0) {
        fprintf(stderr, "Failed to import graph from file.\n");
        return -1;
    }
    displayGraph(graph);
    exportGraphToMermaidFile(graph, "../results/exemple2_mermaid.mmd");
    t_partition *partition = tarjan(graph);
    displayPartition(partition);
    t_hasse_diagram hasse = createHasseDiagram(graph);
    displayHasseDiagram(hasse);
    exportHasseDiagramToMermaidFile(hasse, "../results/exemple2_hasse_mermaid.mmd");
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