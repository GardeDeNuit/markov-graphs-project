#include "matrix.h"
#include "utils.h"

t_matrix createEmptyMatrix() {
    t_matrix m = {0, 0, NULL};
    return m;
}

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

int createResultMatrix(t_matrix *result, int rows, int cols) {
    *result = createMatrix(rows, cols);
    if (!isValidMatrix(*result)) {
        fprintf(stderr, "createResultMatrix: failed to create result matrix\n");
        return -1;
    }
    return 1;
}

void displayMatrixData(t_matrix m) {
    if (isEmptyMatrix(m)) {
        printf("NULL");
    } else {
        for (int i = 0; i < m.rows; ++i) {
            for (int j = 0; j < m.cols; ++j) {
                printf("%.2f ", m.data[i][j]);
            }
            printf("\n");
        }
    }
    printf("\n");
}

void displayMatrix(t_matrix m) {
    printf("%dx%d matrix:\n", m.rows, m.cols);
    displayMatrixData(m);
}

int copyMatrixParamsValid(t_matrix src, t_matrix *dest) {
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

int multiplyMatricesParamsValid(t_matrix a, t_matrix b, t_matrix *result) {
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

int subtractMatricesParamsValid(t_matrix a, t_matrix b, t_matrix *result) {
    if (!isValidMatrix(a)) {
        fprintf(stderr, "subtractMatrices: invalid matrix A\n");
        return FALSE;
    }
    if (!isValidMatrix(b)) {
        fprintf(stderr, "subtractMatrices: invalid matrix B\n");
        return FALSE;
    }
    if (result == NULL) {
        fprintf(stderr, "subtractMatrices: no result matrix provided\n");
        return FALSE;
    }
    if (a.rows != b.rows || a.cols != b.cols) {
        fprintf(stderr, "subtractMatrices: incompatible dimensions (A:%dx%d vs B:%dx%d)\n",
                a.rows, a.cols, b.rows, b.cols);
        return FALSE;
    }
    return TRUE;
}

int powerMatrixParamsValid(t_matrix m, int power, t_matrix *result) {
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

int setMatrixValueParamsValid(t_matrix *m, int row, int col) {
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

int subtractMatrices(t_matrix a, t_matrix b, t_matrix *result) {
    if (subtractMatricesParamsValid(a, b, result) == FALSE) return -1;

    if (createResultMatrix(result, a.rows, a.cols) < 0) return -1;

    for (int i = 0; i < a.rows; ++i) {
        for (int j = 0; j < a.cols; ++j) {
            result->data[i][j] = a.data[i][j] - b.data[i][j];
        }
    }
    return 1;
}


int powerMatrixRec(t_matrix m, int power, t_matrix *result) {
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

int powerMatrix(t_matrix m, int power, t_matrix *result) {
    if (powerMatrixParamsValid(m, power, result) == FALSE) return -1;

    if (createResultMatrix(result, m.rows, m.cols) < 0) return -1;

    return powerMatrixRec(m, power, result);
}

t_matrix createMatrixFromGraph(t_graph g) {
    return createEmptyMatrix();
}