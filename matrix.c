#include "matrix.h"

t_matrix createEmptyMatrix() {
    t_matrix m = {0, 0, NULL};
    return m;
}

int isEmptyMatrix(t_matrix m) {
    return m.data == NULL;
}

int isValidMatrix(t_matrix m) {
    return !isEmptyMatrix(m) && m.rows > 0 && m.cols > 0;
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

int copyMatrix(t_matrix src, t_matrix *dest) {
    return 0;
}

int multiplyMatrices(t_matrix a, t_matrix b, t_matrix *result) {
    return 0;
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





