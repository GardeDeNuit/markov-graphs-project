#include <stdio.h>
#include "graph.h"
#include "mermaidchart-file-generator.h"
#include "utils.h"
#include "partition.h"
#include "tests.h"
#include <string.h>
#include "matrix.h"

int main(void) {
    /*
    // Enregistrer les tests du projet puis exécuter tous les tests.
    register_project_tests();
    int failures = run_all_tests();

    // Retourner le nombre d'échecs comme code de sortie (0 = tout OK)
    return failures;
    */

    t_matrix m = createMatrix(3, 3);
    displayMatrixData(m);
    displayMatrix(m);
    freeMatrix(&m);
    displayMatrixData(m);
    displayMatrix(m);
    return 0;
}