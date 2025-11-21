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

    printf("=== TEST 1: createEmptyMatrix ===\n");
    t_matrix empty = createEmptyMatrix();
    displayMatrix(empty);
    // Attendu: 0x0 matrix avec NULL
    printf("Is empty: %d (attendu: 1)\n\n", isEmptyMatrix(empty));

    printf("=== TEST 2: createMatrix (3x3) ===\n");
    t_matrix m1 = createMatrix(3, 3);
    displayMatrix(m1);
    // Attendu: matrice 3x3 remplie de 0.00
    printf("Is valid: %d (attendu: 1)\n\n", isValidMatrix(m1));

    printf("=== TEST 3: setMatrixValue ===\n");
    setMatrixValue(&m1, 0, 0, 1.0);
    setMatrixValue(&m1, 0, 1, 2.0);
    setMatrixValue(&m1, 0, 2, 3.0);
    setMatrixValue(&m1, 1, 0, 4.0);
    setMatrixValue(&m1, 1, 1, 5.0);
    setMatrixValue(&m1, 1, 2, 6.0);
    setMatrixValue(&m1, 2, 0, 7.0);
    setMatrixValue(&m1, 2, 1, 8.0);
    setMatrixValue(&m1, 2, 2, 9.0);
    displayMatrix(m1);
    // Attendu:
    // 1.00 2.00 3.00
    // 4.00 5.00 6.00
    // 7.00 8.00 9.00

    printf("=== TEST 4: copyMatrix ===\n");
    t_matrix m1_copy = createMatrix(3, 3);
    copyMatrix(m1, &m1_copy);
    displayMatrix(m1_copy);
    // Attendu: même contenu que m1
    // 1.00 2.00 3.00
    // 4.00 5.00 6.00
    // 7.00 8.00 9.00

    printf("=== TEST 5: multiplyMatrices (2x3 * 3x2) ===\n");
    t_matrix a = createMatrix(2, 3);
    setMatrixValue(&a, 0, 0, 1.0);
    setMatrixValue(&a, 0, 1, 2.0);
    setMatrixValue(&a, 0, 2, 3.0);
    setMatrixValue(&a, 1, 0, 4.0);
    setMatrixValue(&a, 1, 1, 5.0);
    setMatrixValue(&a, 1, 2, 6.0);
    printf("Matrice A:\n");
    displayMatrix(a);
    // A = [1 2 3]
    //     [4 5 6]

    t_matrix b = createMatrix(3, 2);
    setMatrixValue(&b, 0, 0, 7.0);
    setMatrixValue(&b, 0, 1, 8.0);
    setMatrixValue(&b, 1, 0, 9.0);
    setMatrixValue(&b, 1, 1, 10.0);
    setMatrixValue(&b, 2, 0, 11.0);
    setMatrixValue(&b, 2, 1, 12.0);
    printf("Matrice B:\n");
    displayMatrix(b);
    // B = [7  8]
    //     [9  10]
    //     [11 12]

    t_matrix mult_result;
    multiplyMatrices(a, b, &mult_result);
    printf("Resultat A * B:\n");
    displayMatrix(mult_result);
    // Attendu (2x2):
    // [1*7+2*9+3*11  1*8+2*10+3*12]   = [58  64]
    // [4*7+5*9+6*11  4*8+5*10+6*12]   = [139 154]

    printf("=== TEST 6: subtractMatrices (3x3 - 3x3) ===\n");
    t_matrix c = createMatrix(3, 3);
    setMatrixValue(&c, 0, 0, 10.0);
    setMatrixValue(&c, 0, 1, 20.0);
    setMatrixValue(&c, 0, 2, 30.0);
    setMatrixValue(&c, 1, 0, 40.0);
    setMatrixValue(&c, 1, 1, 50.0);
    setMatrixValue(&c, 1, 2, 60.0);
    setMatrixValue(&c, 2, 0, 70.0);
    setMatrixValue(&c, 2, 1, 80.0);
    setMatrixValue(&c, 2, 2, 90.0);
    printf("Matrice C:\n");
    displayMatrix(c);

    t_matrix d = createMatrix(3, 3);
    setMatrixValue(&d, 0, 0, 1.0);
    setMatrixValue(&d, 0, 1, 2.0);
    setMatrixValue(&d, 0, 2, 3.0);
    setMatrixValue(&d, 1, 0, 4.0);
    setMatrixValue(&d, 1, 1, 5.0);
    setMatrixValue(&d, 1, 2, 6.0);
    setMatrixValue(&d, 2, 0, 7.0);
    setMatrixValue(&d, 2, 1, 8.0);
    setMatrixValue(&d, 2, 2, 9.0);
    printf("Matrice D:\n");
    displayMatrix(d);

    t_matrix sub_result;
    subtractMatrices(c, d, &sub_result);
    printf("Resultat C - D:\n");
    displayMatrix(sub_result);
    // Attendu (3x3):
    // 9.00  18.00 27.00
    // 36.00 45.00 54.00
    // 63.00 72.00 81.00

    printf("=== TEST 7: powerMatrix (matrice^2) ===\n");
    t_matrix e = createMatrix(2, 2);
    setMatrixValue(&e, 0, 0, 1.0);
    setMatrixValue(&e, 0, 1, 2.0);
    setMatrixValue(&e, 1, 0, 3.0);
    setMatrixValue(&e, 1, 1, 4.0);
    printf("Matrice E:\n");
    displayMatrix(e);
    // E = [1 2]
    //     [3 4]

    t_matrix pow_result;
    powerMatrix(e, 2, &pow_result);
    printf("Resultat E^2:\n");
    displayMatrix(pow_result);
    // Attendu (E*E):
    // [1*1+2*3  1*2+2*4]   = [7  10]
    // [3*1+4*3  3*2+4*4]   = [15 22]

    printf("=== TEST 8: powerMatrix (matrice^0) ===\n");
    t_matrix pow0_result;
    powerMatrix(e, 0, &pow0_result);
    printf("Resultat E^0 (matrice identite):\n");
    displayMatrix(pow0_result);
    // Attendu (matrice identité 2x2):
    // 1.00 0.00
    // 0.00 1.00

    printf("=== TEST 9: powerMatrix (matrice^3) ===\n");
    t_matrix pow3_result;
    powerMatrix(e, 3, &pow3_result);
    printf("Resultat E^3:\n");
    displayMatrix(pow3_result);
    // Attendu (E^2 * E):
    // [7*1+10*3   7*2+10*4]   = [37  54]
    // [15*1+22*3  15*2+22*4]  = [81  118]

    printf("=== TEST 10: freeMatrix ===\n");
    freeMatrix(&m1);
    freeMatrix(&m1_copy);
    freeMatrix(&a);
    freeMatrix(&b);
    freeMatrix(&mult_result);
    freeMatrix(&c);
    freeMatrix(&d);
    freeMatrix(&sub_result);
    freeMatrix(&e);
    freeMatrix(&pow_result);
    freeMatrix(&pow0_result);
    freeMatrix(&pow3_result);
    printf("Toutes les matrices liberees\n");
    printf("Is m1 empty after free: %d (attendu: 1)\n\n", isEmptyMatrix(m1));

    printf("=== TEST 11: Gestion d'erreurs ===\n");
    printf("Test createMatrix dimensions invalides:\n");
    t_matrix invalid = createMatrix(-1, 5);
    printf("Is invalid empty: %d (attendu: 1)\n", isEmptyMatrix(invalid));

    printf("\nTest setMatrixValue hors limites:\n");
    t_matrix test = createMatrix(2, 2);
    int res = setMatrixValue(&test, 5, 5, 99.0);
    printf("Retour setMatrixValue hors limites: %d (attendu: -1)\n", res);

    printf("\nTest multiplyMatrices dimensions incompatibles:\n");
    t_matrix incomp1 = createMatrix(2, 3);
    t_matrix incomp2 = createMatrix(2, 3);
    t_matrix incomp_result;
    res = multiplyMatrices(incomp1, incomp2, &incomp_result);
    printf("Retour multiplication incompatible: %d (attendu: -1)\n", res);

    freeMatrix(&test);
    freeMatrix(&incomp1);
    freeMatrix(&incomp2);

    printf("\n=== TOUS LES TESTS TERMINES ===\n");
    return 0;
}