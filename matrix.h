#ifndef MATRIX_H
#define MATRIX_H

#include "graph.h"

typedef struct s_matrix {
    int rows;
    int cols;
    double **data;
} t_matrix;

t_matrix createEmptyMatrix();

t_matrix createMatrix(int rows, int cols);

void freeMatrix(t_matrix *matrix);

int isEmptyMatrix(t_matrix m);

int isValidMatrix(t_matrix m);

void displayMatrix(t_matrix matrix);

int createMatrixFromGraph(t_graph graph, t_matrix *result);

int copyMatrix(t_matrix src, t_matrix *dest);

int multiplyMatrices(t_matrix a, t_matrix b, t_matrix *result);

double diffMatrices(t_matrix a, t_matrix b);

int powerMatrix(t_matrix matrix, int power, t_matrix *result);

int setMatrixValue(t_matrix *matrix, int row, int col, double value);

int setMatrixData(t_matrix *matrix, double* data, int dataSize);

#endif //MATRIX_H
