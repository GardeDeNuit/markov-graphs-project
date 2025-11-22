#ifndef TARJAN_VERTEX_H
#define TARJAN_VERTEX_H

/**
 * @file tarjan_vertex.h
 * @brief Vertex structure and operations for Tarjan's algorithm
 */

#include <stdlib.h>

#define DEBUG_TARJAN_VERTEX 0

/**
 * @struct s_tarjan_vertex
 * @brief Represents a vertex used in Tarjan's strongly connected components algorithm
 * @param id Vertex identifier (must be >= 0)
 * @param num Discovery number in DFS traversal
 * @param num_accessible Lowest reachable discovery number
 * @param in_pile Flag indicating if vertex is in the stack (0 or 1)
 * @param next Pointer to the next vertex in linked list
 */
struct s_tarjan_vertex {
    int id;
    int num;
    int num_accessible;
    int in_pile;
    struct s_tarjan_vertex* next;
};
typedef struct s_tarjan_vertex t_tarjan_vertex;

/**
 * @brief Creates a new Tarjan vertex with specified attributes
 * @param id The vertex identifier (must be >= 0)
 * @param num The discovery number
 * @param num_accessible The lowest reachable discovery number
 * @param in_pile Stack membership flag (must be 0 or 1)
 * @return Pointer to the newly created vertex, or NULL on failure
 */
t_tarjan_vertex* createTarjanVertex(int id, int num, int num_accessible, int in_pile);

/**
 * @brief Displays a Tarjan vertex's attributes
 * @param tarjan_vertex The vertex to display
 */
void displayTarjanVertex(t_tarjan_vertex tarjan_vertex);

/**
 * @brief Frees the memory allocated for a Tarjan vertex
 * @param tarjan_vertex Pointer to the vertex to free
 * @return 1 on success, -1 if vertex is NULL
 */
int freeTarjanVertex(t_tarjan_vertex* tarjan_vertex);

#endif //TARJAN_VERTEX_H
