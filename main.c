#include <stdio.h>
#include "graph.h"
#include "mermaidchart-file-generator.h"
#include "partition.h"
#include "tests.h"
#include "hasse.h"
#include "tarjan.h"


int main(void) {

        // 1️⃣ Charger le graphe depuis un fichier
        t_graph g = importGraphFromFile("../data/exemple_valid_step3.txt");
        if (g.size == 0) {
            printf("Impossible de charger le graphe.\n");
            return 1;
        }
        printf("Graphe importé :\n");
        displayGraph(g);

        // 2️⃣ Trouver les composantes fortement connexes avec Tarjan
        printf("\n=== Exécution de l'algorithme de Tarjan ===\n");
        t_partition *partition = tarjan(g);

        printf("\n");
        displayPartition(partition);

        int num_vertices = g.size;

        // 3️⃣ Créer un tableau : class_array[sommet] = numéro de classe
        int *class_array = makeClassArray(&g, partition);

        // 4️⃣ Compter le nombre de classes
        int num_classes = 0;
        t_class *curr_class = partition->classes;
        while (curr_class != NULL) {
            num_classes++;
            curr_class = curr_class->next;
        }

        printf("\nNombre de composantes fortement connexes : %d\n", num_classes);

        // 5️⃣ Créer et initialiser le diagramme de Hasse
        t_link_array hasse;
        hasse.logical_size = 0;
        hasse.physical_size = num_classes * num_classes;  // pire cas
        hasse.links = malloc(hasse.physical_size * sizeof(t_link));

        // 6️⃣ Construire les liens Hasse entre classes


    addLinkToHasseDiagram(&hasse, g, class_array);

        // 9️⃣ Construire un t_graph temporaire pour l'export Mermaid
        t_graph hasseGraph;
        hasseGraph.size = num_classes;
        hasseGraph.values = malloc(num_classes * sizeof(t_list));
        for (int i = 0; i < num_classes; i++) {
            hasseGraph.values[i] = createEmptyList();
        }

        // Ajouter les liens Hasse dans le graph temporaire
        for (int i = 0; i < hasse.logical_size; i++) {
            int src = hasse.links[i].src_id;
            int dest = hasse.links[i].dest_id;
            addCell(&hasseGraph.values[src], dest + 1, 1.0);  // +1 pour 1-based
        }

    // 🔟 Exporter le diagramme Hasse en Mermaid (avec sommets des composantes)
    exportHasseDiagramToMermaidFile(&hasse, partition, num_classes, "../data/output_hasse.txt");
    printf("\nDiagramme Hasse exporté dans 'output_hasse.txt'.\n");

    // 1️⃣1️⃣ Nettoyer la mémoire
    for (int i = 0; i < num_classes; i++) {
        freeList(&hasseGraph.values[i]);
    }

    free(hasseGraph.values);
    free(hasse.links);
    free(class_array);
    freePartition(partition);
    freeGraph(&g);

    return 0;
    /*
    // Enregistrer les tests du projet puis exécuter tous les tests.
    register_project_tests();
    int failures = run_all_tests();

    // Retourner le nombre d'échecs comme code de sortie (0 = tout OK)
    return failures;
     */
}