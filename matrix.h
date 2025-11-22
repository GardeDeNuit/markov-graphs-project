#ifndef MATRIX_H
#define MATRIX_H

#include "graph.h"

/**
 * @file matrix.h
 * @brief Matrix module providing basic creation, validation and operations (copy, multiply, power, difference).
 */

/**
 * @struct s_matrix
 * @brief Represents a dynamic 2D matrix of doubles.
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @param data Pointer to an array of row pointers (each row is an array of doubles).
 */
typedef struct s_matrix {
    int rows;
    int cols;
    double **data;
} t_matrix;

/**
 * @brief Create a matrix with all elements initialized to 0.
 * @param rows Number of rows (must be > 0).
 * @param cols Number of columns (must be > 0).
 * @return A new matrix, or an empty matrix (data = NULL) on error.
 */
t_matrix createMatrix(int rows, int cols);

/**
 * @brief Free all memory associated with a matrix and reset its fields.
 * @param matrix Pointer to the matrix to free.
 */
void freeMatrix(t_matrix *matrix);

/**
 * @brief Check if a matrix is empty (uninitialized).
 * @param m Matrix to inspect.
 * @return 1 if empty, 0 otherwise.
 */
int isEmptyMatrix(t_matrix m);

/**
 * @brief Check if a matrix is valid (allocated and dimensions > 0).
 * @param m Matrix to inspect.
 * @return 1 if valid, 0 otherwise.
 */
int isValidMatrix(t_matrix m);

/**
 * @brief Print matrix dimensions and formatted contents to stdout.
 * @param matrix Matrix to display.
 */
void displayMatrix(t_matrix matrix);

/**
 * @brief Build an adjacency/weight matrix from a graph.
 * @param graph Source graph.
 * @param result Pointer to destination matrix (allocated internally).
 * @return 1 on success, -1 on error.
 */
int createMatrixFromGraph(t_graph graph, t_matrix *result);

/**
 * @brief Copy matrix contents from source to destination (same dimensions required).
 * @param src Source matrix (must be valid).
 * @param dest Destination matrix (already allocated with identical dimensions).
 * @return 1 on success, -1 on error.
 */
int copyMatrix(t_matrix src, t_matrix *dest);

/**
 * @brief Multiply two matrices (A x B) and allocate the result.
 * @param a Left operand.
 * @param b Right operand.
 * @param result Pointer receiving the newly allocated result matrix.
 * @return 1 on success, -1 on error.
 */
int multiplyMatrices(t_matrix a, t_matrix b, t_matrix *result);

/**
 * @brief Compute the element-wise absolute difference sum between two matrices.
 * @param a First matrix.
 * @param b Second matrix.
 * @return Sum of absolute differences, or -1 on error.
 */
double diffMatrices(t_matrix a, t_matrix b);

/**
 * @brief Raise a square matrix to a non-negative power.
 * @param matrix Input square matrix.
 * @param power Exponent (>= 0).
 * @param result Pointer receiving the newly allocated result matrix.
 * @return 1 on success, -1 on error.
 */
int powerMatrix(t_matrix matrix, int power, t_matrix *result);

/**
 * @brief Set a single value in the matrix at (row, col).
 * @param matrix Target matrix.
 * @param row Zero-based row index.
 * @param col Zero-based column index.
 * @param value Value to assign.
 * @return 1 on success, -1 on error.
 */
int setMatrixValue(t_matrix *matrix, int row, int col, double value);

/**
 * @brief Replace matrix raw data with provided contiguous values (row-major order).
 * @param matrix Target matrix (already allocated).
 * @param data Pointer to contiguous data array.
 * @param dataSize Number of elements in data (must equal rows * cols).
 * @return 1 on success, -1 on error.
 */
int setMatrixData(t_matrix *matrix, double* data, int dataSize);

#endif //MATRIX_H
