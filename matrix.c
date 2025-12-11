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
                if (m.data[i][j] == 0.0) {
                    printf("0 ");
                } else {
                    printf("%.4f ", m.data[i][j]);
                }
            }
            printf("\n");
        }
    }
    printf("\n");
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

t_matrix buildSubMatrixFromVertices(t_matrix matrix, int* vertices, int vertexCount) {
    // Validation des paramètres
    if (!isValidMatrix(matrix)) {
        fprintf(stderr, "buildSubMatrixFromVertices: invalid input matrix\n");
        return createEmptyMatrix();
    }

    if (vertices == NULL) {
        fprintf(stderr, "buildSubMatrixFromVertices: NULL vertices array\n");
        return createEmptyMatrix();
    }

    if (vertexCount <= 0) {
        fprintf(stderr, "buildSubMatrixFromVertices: invalid vertex count (%d)\n", vertexCount);
        return createEmptyMatrix();
    }

    // Vérification que tous les indices de sommets sont valides (1-based)
    for (int i = 0; i < vertexCount; i++) {
        if (vertices[i] < 1 || vertices[i] > matrix.rows) {
            fprintf(stderr, "buildSubMatrixFromVertices: vertex ID %d out of bounds (valid range: 1-%d)\n",
                    vertices[i], matrix.rows);
            return createEmptyMatrix();
        }
    }

    // Création de la sous-matrice carrée
    t_matrix sub_m = createMatrix(vertexCount, vertexCount);
    if (!isValidMatrix(sub_m)) {
        fprintf(stderr, "buildSubMatrixFromVertices: failed to create submatrix\n");
        return createEmptyMatrix();
    }

    // Remplissage de la sous-matrice
    // Pour chaque paire (i,j) de sommets dans le tableau vertices,
    // on copie la valeur de la matrice originale
    for (int i = 0; i < vertexCount; i++) {
        int row_index = vertices[i] - 1; // Conversion 1-based vers 0-based
        for (int j = 0; j < vertexCount; j++) {
            int col_index = vertices[j] - 1; // Conversion 1-based vers 0-based
            sub_m.data[i][j] = matrix.data[row_index][col_index];
        }
    }

    return sub_m;
}

int computeConvergedMatrixPower(t_matrix matrix,
                                t_matrix initialDistribution,
                                double epsilon,
                                t_matrix *limitDistribution,
                                int maxIter) {

    if (!isValidMatrix(matrix) || matrix.rows != matrix.cols) {
        fprintf(stderr, "computeConvergedMatrixPower: invalid matrix\n");
        return -1;
    }
    if (!isValidMatrix(initialDistribution) || initialDistribution.rows != 1 || initialDistribution.cols != matrix.cols) {
        fprintf(stderr, "computeConvergedMatrixPower: invalid initial distribution (must be 1x%d)\n", matrix.cols);
        return -1;
    }
    if (limitDistribution == NULL) {
        fprintf(stderr, "computeConvergedMatrixPower: NULL limit distribution pointer\n");
        return -1;
    }

    t_matrix prev;
    if (createResultMatrix(&prev, 1, matrix.cols) < 0) return -1;
    if (copyMatrix(initialDistribution, &prev) < 0) {
        freeMatrix(&prev);
        return -1;
    }

    t_matrix curr;

    for (int n = 1; n <= maxIter; n++) {
        if (multiplyMatrices(prev, matrix, &curr) < 0) {
            freeMatrix(&prev);
            return -1;
        }

        printf("n = %d:\n", n);
        displayMatrix(curr); // afficher chaque distribution intermédiaire

        double diff = diffMatrices(prev, curr);
        if (diff < epsilon) {
            *limitDistribution = curr;
            freeMatrix(&prev);
            return n;
        }

        freeMatrix(&prev);
        prev = curr;
    }

    freeMatrix(&prev);
    return -1;
}

void displayConvergedMatrixPower(t_matrix matrix,
                                 t_matrix initialDistribution,
                                 double epsilon,
                                 int maxIter) {
    t_matrix limitDistribution;
    printf("Initial distribution, n = 0:\n");
    displayMatrix(initialDistribution);
    int n = computeConvergedMatrixPower(matrix, initialDistribution, epsilon, &limitDistribution, maxIter);
    if (n < 0) {
        printf("No convergence within %d iterations.\n", maxIter);
    } else {
        printf("Converged at n = %d with limit distribution:\n", n);
        displayMatrix(limitDistribution);
        freeMatrix(&limitDistribution);
    }
}

int computeDistributionAfterNSteps(t_matrix transitionMatrix,
                                    t_matrix initialDistribution,
                                    int n,
                                    t_matrix *result) {
    // Vérification des paramètres
    if (!isValidMatrix(transitionMatrix)) {
        fprintf(stderr, "computeDistributionAfterNSteps: invalid transition matrix\n");
        return -1;
    }

    if (!isValidMatrix(initialDistribution)) {
        fprintf(stderr, "computeDistributionAfterNSteps: invalid initial distribution\n");
        return -1;
    }

    if (result == NULL) {
        fprintf(stderr, "computeDistributionAfterNSteps: NULL result pointer\n");
        return -1;
    }

    if (n < 0) {
        fprintf(stderr, "computeDistributionAfterNSteps: n must be non-negative (given: %d)\n", n);
        return -1;
    }

    // Vérification que la matrice de transition est carrée
    if (transitionMatrix.rows != transitionMatrix.cols) {
        fprintf(stderr, "computeDistributionAfterNSteps: transition matrix must be square\n");
        return -1;
    }

    // Vérification que la distribution initiale est un vecteur ligne
    if (initialDistribution.rows != 1) {
        fprintf(stderr, "computeDistributionAfterNSteps: initial distribution must be a row vector (1xN)\n");
        return -1;
    }

    // Vérification de la compatibilité des dimensions
    if (initialDistribution.cols != transitionMatrix.rows) {
        fprintf(stderr, "computeDistributionAfterNSteps: incompatible dimensions (distribution: 1x%d vs matrix: %dx%d)\n",
                initialDistribution.cols, transitionMatrix.rows, transitionMatrix.cols);
        return -1;
    }

    // Cas n = 0 : la distribution reste inchangée
    if (n == 0) {
        if (createResultMatrix(result, 1, initialDistribution.cols) < 0) return -1;
        return copyMatrix(initialDistribution, result);
    }

    // Calcul de M^n
    t_matrix matrixPowerN;
    if (powerMatrix(transitionMatrix, n, &matrixPowerN) < 0) {
        fprintf(stderr, "computeDistributionAfterNSteps: failed to compute matrix power\n");
        return -1;
    }

    // Multiplication : distribution * M^n
    if (multiplyMatrices(initialDistribution, matrixPowerN, result) < 0) {
        fprintf(stderr, "computeDistributionAfterNSteps: failed to multiply matrices\n");
        freeMatrix(&matrixPowerN);
        return -1;
    }

    freeMatrix(&matrixPowerN);
    return 1;
}

void displayDistributionAfterNSteps(t_matrix transitionMatrix,
                                     t_matrix initialDistribution,
                                     int n) {
    printf("=== Computing Distribution After %d Steps ===\n", n);
    printf("Initial distribution:\n");
    displayMatrix(initialDistribution);

    t_matrix resultDistribution;
    int status = computeDistributionAfterNSteps(transitionMatrix, initialDistribution, n, &resultDistribution);

    if (status < 0) {
        printf("Error: Failed to compute distribution after %d steps\n", n);
        return;
    }

    printf("Distribution after %d steps:\n", n);
    displayMatrix(resultDistribution);

    freeMatrix(&resultDistribution);
}

t_matrix getDistributionForState(t_matrix matrix, int stateId) {
    // Validation des paramètres
    if (!isValidMatrix(matrix)) {
        fprintf(stderr, "getDistributionForState: invalid matrix\n");
        return createEmptyMatrix();
    }

    // Vérification que stateId est dans la plage valide (1-based)
    if (stateId < 1 || stateId > matrix.rows) {
        fprintf(stderr, "getDistributionForState: state ID %d out of bounds (valid range: 1-%d)\n",
                stateId, matrix.rows);
        return createEmptyMatrix();
    }

    // Création d'une matrice ligne (1 x cols)
    t_matrix distribution = createMatrix(1, matrix.cols);
    if (!isValidMatrix(distribution)) {
        fprintf(stderr, "getDistributionForState: failed to create distribution matrix\n");
        return createEmptyMatrix();
    }

    // Conversion de l'ID 1-based en index 0-based
    int rowIndex = stateId - 1;

    // Copie de la ligne correspondante dans la nouvelle matrice
    for (int j = 0; j < matrix.cols; j++) {
        distribution.data[0][j] = matrix.data[rowIndex][j];
    }

    return distribution;
}