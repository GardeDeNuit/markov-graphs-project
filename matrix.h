#ifndef MATRIX_H
#define MATRIX_H

#include "graph.h"

typedef struct s_matrix {
    int rows;
    int cols;
    double **data;
} t_matrix;

t_matrix createEmptyMatrix(); // done
int isEmptyMatrix(t_matrix m); // done
int isValidMatrix(t_matrix m); // done
t_matrix createMatrix(int rows, int cols); // done
void freeMatrix(t_matrix *matrix); // done
void displayMatrixData(t_matrix matrix); // done
void displayMatrix(t_matrix matrix); // done
t_matrix createMatrixFromGraph(t_graph graph);
int copyMatrix(t_matrix src, t_matrix *dest);
int multiplyMatrices(t_matrix a, t_matrix b, t_matrix *result);
int subtractMatrices(t_matrix a, t_matrix b, t_matrix *result);
int powerMatrix(t_matrix matrix, int power, t_matrix *result);

#endif //MATRIX_H
