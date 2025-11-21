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

int createResultMatrix(t_matrix *result, int rows, int cols);

void displayMatrix(t_matrix matrix);
void displayMatrixData(t_matrix matrix);

t_matrix createMatrixFromGraph(t_graph graph);

int copyMatrix(t_matrix src, t_matrix *dest);
int copyMatrixParamsValid(t_matrix src, t_matrix *dest);

int multiplyMatrices(t_matrix a, t_matrix b, t_matrix *result);
int multiplyMatricesParamsValid(t_matrix a, t_matrix b, t_matrix *result);

int subtractMatrices(t_matrix a, t_matrix b, t_matrix *result);
int subtractMatricesParamsValid(t_matrix a, t_matrix b, t_matrix *result);

int powerMatrix(t_matrix matrix, int power, t_matrix *result);
int powerMatrixRec(t_matrix m, int power, t_matrix *result);

int setMatrixValue(t_matrix *matrix, int row, int col, double value);
int setMatrixValueParamsValid(t_matrix *matrix, int row, int col);

int setMatrixData(t_matrix *matrix, double* data, int dataSize);
int setMatrixDataParamsValid(t_matrix *matrix, double* data, int dataSize);


#endif //MATRIX_H
