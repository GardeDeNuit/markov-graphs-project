#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/**
 * @file graph.h
 * @brief Graph structure and operations for directed weighted graphs
 */

#define MIN_SIZE_GRAPH 1

/**
 * @struct s_graph
 * @brief Represents a directed weighted graph using adjacency lists
 * @param values Array of adjacency lists (one per vertex)
 * @param size Number of vertices in the graph
 */
struct s_graph {
    t_list* values;
    int size;
};
typedef struct s_graph t_graph;

/**
 * @brief Creates an empty graph with size 0 and NULL values
 * @return An empty graph structure
 */
t_graph createEmptyGraph(void);

/**
 * @brief Creates a graph with a specified number of vertices
 * @param size The number of vertices (must be >= MIN_SIZE_GRAPH)
 * @return The created graph, or an empty graph on failure
 */
t_graph createGraph(int size);

/**
 * @brief Displays the entire graph (all adjacency lists)
 * @param graph The graph to display
 */
void displayGraph(t_graph graph);

/**
 * @brief Adds a directed edge from src to dest with a given weight
 * @param graph Pointer to the graph
 * @param src Source vertex (1-indexed)
 * @param dest Destination vertex (1-indexed)
 * @param weight Weight of the edge
 * @return 1 on success, -1 on failure
 */
int addEdge(t_graph *graph, int src, int dest, double weight);

/**
 * @brief Checks if an edge exists from src to dest
 * @param graph The graph to check
 * @param src Source vertex (1-indexed)
 * @param dest Destination vertex (1-indexed)
 * @return 1 if edge exists, 0 otherwise
 */
int hasEdge(t_graph graph, int src, int dest);

/**
 * @brief Gets the adjacency list (neighbors) of a vertex
 * @param graph Pointer to the graph
 * @param src Source vertex (1-indexed)
 * @return Pointer to the adjacency list, or NULL on error
 */
t_list* getNeighbors(t_graph *graph, int src);

/**
 * @brief Frees all memory allocated for the graph
 * @param graph Pointer to the graph to free
 * @return 1 on success, -1 on failure
 */
int freeGraph(t_graph *graph);

/**
 * @brief Imports a graph from a file
 * @param path Path to the file containing the graph data
 * @return The imported graph, or an empty graph on failure
 * @note File format: first line contains number of vertices,
 *       following lines contain edges as "src dest weight"
 */
t_graph importGraphFromFile(const char* path);

/**
 * @brief Checks if a graph is a Markov graph
 * @param graph The graph to check
 * @return 1 if the graph is Markovian, 0 otherwise
 * @note A Markov graph has outgoing edge weights summing to 1.0 for each vertex
 */
int isMarkovGraph(t_graph graph);

#endif //GRAPH_H
