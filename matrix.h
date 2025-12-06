#ifndef MATRIX_H
#define MATRIX_H

#include "graph.h"
#include "partition.h"
#include "graph.h"
#include "hasse.h"

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

/**
 * @brief Extracts a submatrix corresponding to a specific component of a graph partition.
 *
 * @param matrix The original adjacency matrix of the graph.
 * @param part The partition of the graph into strongly connected components.
 * @param class_id The index of the component to extract.
 * @return t_matrix The submatrix corresponding to the specified component.
 */
t_matrix buildSubMatrix(t_matrix matrix, t_partition part, int class_id);

/**
 * @brief Extracts a submatrix from specified vertices.
 *
 * This function creates a submatrix by selecting only the rows and columns
 * corresponding to the vertices specified in the vertices array.
 * The order of vertices in the array determines the order in the submatrix.
 *
 * @param matrix The original adjacency matrix of the graph.
 * @param vertices Array of vertex IDs (1-based indexing).
 * @param vertexCount Number of vertices in the array.
 * @return t_matrix The submatrix corresponding to the specified vertices.
 *         Returns an empty matrix on error.
 */
t_matrix buildSubMatrixFromVertices(t_matrix matrix, int* vertices, int vertexCount);

/**
 * @brief Calcule le plus petit n tel que diffMatrices(M^n, M^(n-1)) < epsilon, à partir d'une distribution initiale.
 *        La matrice convergée M^n est renvoyée via limitDistribution.
 *
 * @param matrix Matrice de transition carrée (Markov), de taille NxN.
 * @param initialDistribution Distribution initiale (vecteur ligne 1xN), utilisée comme point de départ.
 * @param epsilon Seuil de convergence strictement positif.
 * @param limitDistribution Pointeur recevant la matrice M^n à convergence (allouée en interne).
 * @param maxIter Limite maximale d'itérations (exemple: 2000). Arrête le calcul en absence de convergence.
 * @return int L’exposant n où la convergence est atteinte, ou -1 si la convergence n’est pas atteinte.
 */
int computeConvergedMatrixPower(t_matrix matrix,
                                t_matrix initialDistribution,
                                double epsilon,
                                t_matrix *limitDistribution,
                                int maxIter);
/**
 * @brief Affiche la matrice obtenue via le calcul de la puissance convergée de \`matrix\`, depuis une distribution initiale.
 *
 * @param matrix Matrice de transition carrée (Markov), de taille NxN.
 * @param initialDistribution Distribution initiale (vecteur ligne 1xN), utilisée comme point de départ.
 * @param epsilon Seuil de convergence strictement positif.
 * @param maxIter Limite maximale d'itérations (exemple: 2000).
 */
void displayConvergedMatrixPower(t_matrix matrix,
                                 t_matrix initialDistribution,
                                 double epsilon,
                                 int maxIter);
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
        double epsilon);

/**
 * @brief Compute the probability distribution of states after n steps.
 *
 * This function takes an initial probability distribution (row vector) and
 * computes the distribution after n steps by multiplying it with the transition
 * matrix raised to the power n: result = initialDistribution * M^n
 *
 * @param transitionMatrix The transition matrix of the Markov chain.
 * @param initialDistribution Initial probability distribution (1 x size row vector).
 * @param n Number of steps (must be >= 0).
 * @param result Pointer receiving the resulting distribution (1 x size row vector).
 * @return 1 on success, -1 on error.
 */
int computeDistributionAfterNSteps(t_matrix transitionMatrix,
                                    t_matrix initialDistribution,
                                    int n,
                                    t_matrix *result);

/**
 * @brief Display the probability distribution after n steps starting from an initial distribution.
 *
 * @param transitionMatrix The transition matrix of the Markov chain.
 * @param initialDistribution Initial probability distribution (1 x size row vector).
 * @param n Number of steps.
 */
void displayDistributionAfterNSteps(t_matrix transitionMatrix,
                                     t_matrix initialDistribution,
                                     int n);

/**
 * @brief Extract the probability distribution associated with a given state.
 *
 * This function extracts a specific row from a matrix, representing the
 * probability distribution for transitioning from a given state to all other states.
 * The state index is 1-based (matching vertex IDs in the graph).
 *
 * @param matrix The transition matrix (or any matrix to extract from).
 * @param stateId The state identifier (1-based, e.g., vertex 1, 2, 3...).
 * @return t_matrix A 1 x N row vector containing the distribution for that state.
 *         Returns an empty matrix on error.
 */
t_matrix getDistributionForState(t_matrix matrix, int stateId);

#endif //MATRIX_H
