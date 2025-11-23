#include "matrix.h"
#include "utils.h"
#include <math.h>

/**
 * @file matrix.c
 * @brief Implementation of matrix creation, manipulation and operations.
 */

/* private functions =================================================== */

/**
 * @brief Create an empty (invalid) matrix.
 * @return A matrix with rows=0, cols=0, data=NULL.
 */
static t_matrix createEmptyMatrix() {
    t_matrix m = {0, 0, NULL};
    return m;
}

/**
 * @brief Allocate a result matrix with given dimensions.
 * @param result Pointer to matrix receiving allocation.
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @return 1 on success, -1 on allocation failure.
 */
static int createResultMatrix(t_matrix *result, int rows, int cols) {
    *result = createMatrix(rows, cols);
    if (!isValidMatrix(*result)) {
        fprintf(stderr, "createResultMatrix: failed to create result matrix\n");
        return -1;
    }
    return 1;
}

/**
 * @brief Validate parameters for copyMatrix before performing copy.
 * @param src Source matrix.
 * @param dest Destination matrix.
 * @return TRUE if parameters are valid, FALSE otherwise.
 */
static int copyMatrixParamsValid(t_matrix src, t_matrix *dest) {
    if (!isValidMatrix(src)) {
        fprintf(stderr, "copyMatrix: invalid source matrix\n");
        return FALSE;
    }
    if (dest == NULL || !isValidMatrix(*dest)) {
        fprintf(stderr, "copyMatrix: invalid destination matrix\n");
        return FALSE;
    }
    if (src.rows != dest->rows || src.cols != dest->cols) {
        fprintf(stderr, "copyMatrix: incompatible dimensions (Source:%dx%d vs Destination:%dx%d)\n",
                src.rows, src.cols, dest->rows, dest->cols);
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief Validate parameters for matrix multiplication.
 * @param a Left matrix.
 * @param b Right matrix.
 * @param result Destination pointer.
 * @return TRUE if valid, FALSE otherwise.
 */
static int multiplyMatricesParamsValid(t_matrix a, t_matrix b, t_matrix *result) {
    if (!isValidMatrix(a)) {
        fprintf(stderr, "multiplyMatrices: invalid matrix A\n");
        return FALSE;
    }
    if (!isValidMatrix(b)) {
        fprintf(stderr, "multiplyMatrices: invalid matrix B\n");
        return FALSE;
    }
    if (result == NULL) {
        fprintf(stderr, "multiplyMatrices: no result matrix provided\n");
        return FALSE;
    }
    if (a.cols != b.rows) {
        fprintf(stderr, "multiplyMatrices: incompatible dimensions (A:%dx%d vs B:%dx%d)\n",
                a.rows, a.cols, b.rows, b.cols);
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief Validate parameters for matrix exponentiation.
 * @param m Input square matrix.
 * @param power Non-negative exponent.
 * @param result Destination pointer.
 * @return TRUE if valid, FALSE otherwise.
 */
static int powerMatrixParamsValid(t_matrix m, int power, t_matrix *result) {
    if (!isValidMatrix(m)) {
        fprintf(stderr, "powerMatrix: invalid matrix\n");
        return FALSE;
    }
    if (m.rows != m.cols) {
        fprintf(stderr, "powerMatrix: matrix must be square (given: %dx%d)\n", m.rows, m.cols);
        return FALSE;
    }
    if (result == NULL) {
        fprintf(stderr, "powerMatrix: no result matrix provided\n");
        return FALSE;
    }
    if (power < 0) {
        fprintf(stderr, "powerMatrix: power must be non-negative (given: %d)\n", power);
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief Validate parameters for setting a matrix value.
 * @param m Pointer to matrix.
 * @param row Row index.
 * @param col Column index.
 * @return TRUE if valid, FALSE otherwise.
 */
static int setMatrixValueParamsValid(t_matrix *m, int row, int col) {
    if (m == NULL || !isValidMatrix(*m)) {
        fprintf(stderr, "setMatrixValue: invalid matrix\n");
        return FALSE;
    }
    if (row < 0 || col < 0 || row >= m->rows || col >= m->cols) {
        fprintf(stderr, "setMatrixValue: indices out of bounds (row:%d, col:%d)\n", row, col);
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief Validate parameters for computing difference between two matrices.
 * @param a First matrix.
 * @param b Second matrix.
 * @return TRUE if valid, FALSE otherwise.
 */
static int diffMatricesParamsValid(t_matrix a, t_matrix b) {
    if (!isValidMatrix(a)) {
        fprintf(stderr, "diffMatrices: invalid matrix A\n");
        return FALSE;
    }
    if (!isValidMatrix(b)) {
        fprintf(stderr, "diffMatrices: invalid matrix B\n");
        return FALSE;
    }
    if (a.rows != b.rows || a.cols != b.cols) {
        fprintf(stderr, "diffMatrices: incompatible dimensions (A:%dx%d vs B:%dx%d)\n",
                a.rows, a.cols, b.rows, b.cols);
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief Recursive function for exponentiation by repeated multiplication.
 * @param m Base matrix.
 * @param power Current exponent.
 * @param result Result matrix (already allocated).
 * @return 1 on success, -1 on error.
 */
static int powerMatrixRec(t_matrix m, int power, t_matrix *result) {
    if (power == 0) {
        // Retourner la matrice identité
        for (int i = 0; i < m.rows; i++) {
            for (int j = 0; j < m.cols; j++) {
                result->data[i][j] = (i == j) ? 1.0 : 0.0;
            }
        }
        return 1;
    }
    if (power == 1) {
        // Retourner la matrice elle-même
        return copyMatrix(m, result);
    }
    t_matrix temp;
    powerMatrix(m, power - 1, &temp);
    multiplyMatrices(m, temp, result);
    freeMatrix(&temp);
}

/**
 * @brief Internal display of matrix raw data (without header).
 * @param m Matrix to print.
 */
static void displayMatrixData(t_matrix m) {
    if (isEmptyMatrix(m)) {
        printf("NULL");
    } else {
        for (int i = 0; i < m.rows; ++i) {
            for (int j = 0; j < m.cols; ++j) {
                printf("%6.2f ", m.data[i][j]);
            }
            printf("\n");
        }
    }
    printf("\n");
}

/**
 * @brief Computes the stationary distribution of a single class.
 *
 * @param M Full adjacency matrix.
 * @param part Partition of the graph.
 * @param hasse Hasse diagram (persistent / transient info).
 * @param class Pointer to the class to process.
 * @param epsilon Convergence threshold.
 * @return the stationary distribution of that class.
 *         Zero matrix if the class is transient.
 */
static t_matrix computeStationaryDistributionForOneClass(
        t_matrix M,
        t_partition part,
        t_hasse_diagram hasse,
        t_class *class,
        double epsilon) {
    if (class == NULL) {
        fprintf(stderr, "computeStationaryDistributionForOneClass: NULL class pointer\n");
        return createMatrix(0, 0);
    }

    int class_id = class->id;

    // Construction de la sous-matrice correspondant à la classe
    t_matrix subM = buildSubMatrix(M, part, class_id);
    if (!isValidMatrix(subM)) {
        fprintf(stderr, "Error: Failed to build submatrix for class %d\n", class_id);
        return createMatrix(0, 0);
    }

    int size = subM.rows;

    // Cas ou la class n'est pas persistante
    if (!isPersistantClass(hasse, class_id)) {
        // La distribution est stationnaire
        // On doit renvoyer une matrice nulle
        t_matrix zero = createMatrix(1, size);
        freeMatrix(&subM);
        return zero;
    }

    // La matrice est persistante.
    t_matrix limit;
    int n = computeConvergedMatrixPower(subM, epsilon, &limit, 2000);

    if (n == -1) {
        fprintf(stderr, "Class %d did not converge.\n", class_id);
        t_matrix zero = createMatrix(1, size);
        freeMatrix(&subM);
        return zero;
    }

    // Extraction de la distribution stationnaire
    // La distribution stationnaire est la première ligne de la matrice limite
    t_matrix distrib = createMatrix(1, size);
    for (int j = 0; j < size; j++)
        distrib.data[0][j] = limit.data[0][j];

    freeMatrix(&limit);
    freeMatrix(&subM);

    return distrib;
}

/* public functions =================================================== */

t_matrix createMatrix(const int rows, const int cols) {
    t_matrix m = createEmptyMatrix();
    if (rows <= 0 || cols <= 0) {
        fprintf(stderr, "createMatrix: invalid dimensions (%d x %d)\n", rows, cols);
        return m;
    };
    m.data = (double**)malloc(sizeof(double*) * rows);
    if (m.data == NULL) {
        perror("createMatrix: memory allocation failed");
        return m;
    };
    for (int i = 0; i < rows; ++i) {
        m.data[i] = (double*)calloc(cols, sizeof(double));
        if (m.data[i] == NULL) {
            perror("createMatrix: row allocation failed");
            // Free already allocated rows
            for (int j = 0; j < i; ++j) {
                free(m.data[j]);
            }
            free(m.data);
            return createEmptyMatrix();
        }
    }
    m.rows = rows;
    m.cols = cols;
    return m;
}

void freeMatrix(t_matrix *m) {
    if (isEmptyMatrix(*m)) return;
    for (int i = 0; i < m->rows; ++i) {
        free(m->data[i]);
    }
    free(m->data);
    m->data = NULL;
    m->rows = 0;
    m->cols = 0;
}

int isEmptyMatrix(t_matrix m) {
    return m.data == NULL;
}

int isValidMatrix(t_matrix m) {
    return !isEmptyMatrix(m) && m.rows > 0 && m.cols > 0;
}

void displayMatrix(t_matrix m) {
    printf("%dx%d matrix:\n", m.rows, m.cols);
    displayMatrixData(m);
}

int copyMatrix(t_matrix src, t_matrix *dest) {
    if (copyMatrixParamsValid(src, dest) == FALSE) return -1;
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            dest->data[i][j] = src.data[i][j];
        }
    }
    return 1;
}

int multiplyMatrices(t_matrix a, t_matrix b, t_matrix *result) {
    if (multiplyMatricesParamsValid(a, b, result) == FALSE) return -1;

    if (createResultMatrix(result, a.rows, b.cols) < 0) return -1;

    for (int i = 0; i < a.rows; ++i) {
        for (int j = 0; j < b.cols; ++j) {
            result->data[i][j] = 0.0;
            for (int k = 0; k < a.cols; ++k) {
                result->data[i][j] += a.data[i][k] * b.data[k][j];
            }
        }
    }
    return 1;
}

int setMatrixValue(t_matrix *m, const int row, const int col, const double value) {
    if (setMatrixValueParamsValid(m, row, col) == FALSE) return -1;
    m->data[row][col] = value;
    return 1;
}

int powerMatrix(t_matrix m, int power, t_matrix *result) {
    if (powerMatrixParamsValid(m, power, result) == FALSE) return -1;

    if (createResultMatrix(result, m.rows, m.cols) < 0) return -1;

    return powerMatrixRec(m, power, result);
}

double diffMatrices(t_matrix a, t_matrix b) {
    if (diffMatricesParamsValid(a, b) == FALSE) return -1;

    double result = 0.0;
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            result += fabs(a.data[i][j] - b.data[i][j]);
        }
    }
    return result;
}

int createMatrixFromGraph(t_graph g, t_matrix *result) {
    if (result == NULL) {
        fprintf(stderr, "createMatrixFromGraph: no result matrix provided\n");
        return -1;
    }

    if (createResultMatrix(result, g.size, g.size) < 0) return -1;

    int vertex_id;
    for (int i = 0; i < g.size; i++) {
        vertex_id = i + 1;
        t_list* curr_list = getNeighbors(&g, vertex_id);
        if (curr_list == NULL) {
            fprintf(stderr, "createMatrixFromGraph: failed to access vertex neighbors (vertex: %d)\n", vertex_id);
            return -1;
        }
        t_cell* curr = curr_list->head;
        while (curr != NULL) {
            // (curr->vertex - 1) = indice du vertex courant dans la matrice
            result->data[i][curr->vertex - 1] = curr->weight;
            curr = curr->next;
        }
    }
    return 1;
}

t_matrix buildSubMatrix(t_matrix matrix, t_partition part, int class_id) {
    // Recherche de la classe correspondant à class_id dans la partition.
    // La partition est une liste chaînée de classes.
    t_class* curr_class = part.classes;
    while (curr_class != NULL && curr_class->id != class_id) {
        curr_class = curr_class->next;
    }
    if (curr_class == NULL) {
        fprintf(stderr, "subMatrix: class_id %d not found in partition\n", class_id);
        return createEmptyMatrix();
    }

    // Création d'une matrice carrée de taille égale au nombre de sommets dans la classe.
    int size = curr_class->vertex_number;
    t_matrix sub_m = createMatrix(size, size);
    if (!isValidMatrix(sub_m)) {
        fprintf(stderr, "subMatrix: failed to create submatrix\n");
        return createEmptyMatrix();
    }

    // Remplissage de la sous-matrice avec les valeurs appropriées.
    // On parcourt les sommets de la classe pour déterminer les indices
    // des valeurs à récupérer dans la matrice originale
    t_vertex* row_vertex = curr_class->vertices;
    for (int i = 0; i < size; ++i) {
        t_vertex* col_vertex = curr_class->vertices;
        for (int j = 0; j < size; ++j) {
            int row_index = row_vertex->value - 1;
            int col_index = col_vertex->value - 1;
            sub_m.data[i][j] = matrix.data[row_index][col_index];
            col_vertex = col_vertex->next;
        }
        row_vertex = row_vertex->next;
    }

    return sub_m;
}

int computeConvergedMatrixPower(t_matrix matrix, double epsilon, t_matrix *limitMatrix, int maxIter) {
    if (!isValidMatrix(matrix) || matrix.rows != matrix.cols) {
        fprintf(stderr, "computeConvergedMatrixPower: invalid input matrix\n");
        return -1;
    }

    // Calculer M^1
    // C'est égal à la matrice elle-même donc on initialise prev avec matrix
    t_matrix prev;
    if (createResultMatrix(&prev, matrix.rows, matrix.rows) < 0) return -1;
    copyMatrix(matrix, &prev);

    t_matrix curr;

    for (int n = 2; n <= maxIter; n++) {
        // Calculer M^n
        if (powerMatrix(matrix, n, &curr) < 0) {
            freeMatrix(&prev);
            return -1;
        }

        // Calculer la différence entre M^(n-1) et M^n (diff(M^(n-1), M^n))
        double diff = diffMatrices(prev, curr);

        if (diff < epsilon) {
            // La convergence est atteinte
            // Retourner n et la matrice M^n
            *limitMatrix = curr;
            freeMatrix(&prev);
            return n;
        }

        // Libérer prev pour préparer pour la prochaine itération
        freeMatrix(&prev);
        prev = curr;
    }

    // Pas de convergence atteinte dans le nombre maximal d'itérations
    // Libérer la dernière matrice calculée
    // et retourner -1
    freeMatrix(&prev);
    return -1;
}

void dipslayConvergedMatrixPower(t_matrix matrix, double epsilon, int maxIter) {
    t_matrix limitMatrix;
    int n = computeConvergedMatrixPower(matrix, epsilon, &limitMatrix, maxIter);
    if (n < 0) {
        printf("No convergence within %d iterations.\n", maxIter);
    } else {
        printf("Converged at n = %d with limit matrix:\n", n);
        displayMatrix(limitMatrix);
        freeMatrix(&limitMatrix);
    }
}

/**
 * @brief Compute and display stationary distributions for all classes.
 *
 * @param M Full adjacency matrix.
 * @param part Graph partition.
 * @param hasse Hasse diagram.
 * @param epsilon Convergence threshold.
 */
void computeStationaryDistributionsForAllClasses(
        t_matrix M,
        t_partition part,
        t_hasse_diagram hasse,
        double epsilon)
{
    t_class *class = part.classes;

    printf("\n=== Stationary distributions for all classes ===\n");

    while (class != NULL) {
        displayClass(class);
        printf(": \n");

        t_matrix distrib = computeStationaryDistributionForOneClass(M, part, hasse, class, epsilon);

        displayMatrix(distrib);
        freeMatrix(&distrib);

        class = class->next;
    }
}