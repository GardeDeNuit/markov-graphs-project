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
    for (int i = 0; i < rows; ++i) {
        m.data[i] = (double*)calloc(cols, sizeof(double));
    }
    if (m.data == NULL) {
        perror("createMatrix: allocation failed");
        return m;
    };
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
        fprintf(stderr, "copyMatrix: source matrix invalid\n");
        return FALSE;
    }
    if (dest == NULL || !isValidMatrix(*dest)) {
        fprintf(stderr, "copyMatrix: destination matrix invalid\n");
        return FALSE;
    }
    if (src.rows != dest->rows || src.cols != dest->cols) {
        fprintf(stderr, "copyMatrix: dimension mismatch (Src:%dx%d vs Dest:%dx%d)\n",
                src.rows, src.cols, dest->rows, dest->cols);
        return FALSE;
    }
    return TRUE;
}

int multiplyMatricesParamsValid(t_matrix a, t_matrix b, t_matrix *result) {
    if (!isValidMatrix(a)) {
        fprintf(stderr, "multiplyMatrices: matrix A invalid\n");
        return FALSE;
    }
    if (!isValidMatrix(b)) {
        fprintf(stderr, "multiplyMatrices: matrix B invalid\n");
        return FALSE;
    }
    if (result == NULL) {
        fprintf(stderr, "multiplyMatrices: no result matrix provided\n");
        return FALSE;
    }
    if (a.cols != b.rows) {
        fprintf(stderr, "multiplyMatrices: dimension mismatch (A:%dx%d vs B:%dx%d)\n",
                a.rows, a.cols, b.rows, b.cols);
        return FALSE;
    }
    return TRUE;
}

int setMatrixValueParamsValid(t_matrix *m, int row, int col) {
    if (m == NULL || !isValidMatrix(*m)) {
        fprintf(stderr, "setMatrixValue: matrix invalid\n");
        return FALSE;
    }
    if (row < 0 || col < 0 || row >= m->rows || col >= m->cols) {
        fprintf(stderr, "setMatrixValue: index out of bounds (row:%d, col:%d)\n", row, col);
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

    *result = createMatrix(a.rows, b.cols);
    if (isValidMatrix(*result)) return -1;

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

int setMatrixValue(t_matrix *matrix, int row, int col, double value) {
    if (setMatrixValueParamsValid(matrix, row, col) == FALSE) return -1;
    matrix->data[row][col] = value;
    return 1;
}

int subtractMatrices(t_matrix a, t_matrix b, t_matrix *result) {
    return 0;
}

int powerMatrix(t_matrix matrix, int power, t_matrix *result) {
    return 0;
}

t_matrix createMatrixFromGraph(t_graph graph) {
    return createEmptyMatrix();
}

