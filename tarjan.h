#ifndef TARJAN_H
#define TARJAN_H

#define UNVISITED (-1)

#include "partition.h"
#include "graph.h"
#include "tarjan_vertex.h"
#include "utils.h"

/**
 * @brief Computes the strongly connected components of a graph using Tarjan's algorithm.
 *
 * This function applies Tarjan's algorithm to partition the graph into its strongly
 * connected components (SCCs). Each SCC becomes a class in the returned partition.
 *
 * @param graph The graph to analyze. Must have size > 0 for meaningful results.
 * @return Pointer to a partition containing all strongly connected components.
 *         Returns an empty partition if graph is empty.
 *         Returns NULL on memory allocation failure.
 *         Caller must free with freePartition().
 */
t_partition *tarjan(t_graph graph);

/**
 * @brief Frees a partially allocated array of Tarjan vertices.
 *
 * @param tarjan_vertices Array of Tarjan vertices to free.
 * @param count Number of vertices that were successfully allocated.
 */
static void freeTarjanVerticesPartial(t_tarjan_vertex **tarjan_vertices, int count);

/**
 * @brief Converts a graph into an array of Tarjan vertices.
 *
 * Creates one Tarjan vertex for each vertex in the graph, all initialized
 * as unvisited and not on the stack.
 *
 * @param graph The graph to convert.
 * @return Array of Tarjan vertices, or NULL on allocation failure.
 */
static t_tarjan_vertex** graphToTarjanVertices(t_graph graph);

/**
 * @brief Initializes a Tarjan vertex during the first visit.
 *
 * Sets the vertex's discovery number and low-link value, then pushes it onto the stack.
 *
 * @param vertex The vertex to initialize.
 * @param current_num Pointer to the current discovery number counter.
 * @param stack The stack.
 */
static void initializeTarjanVertex(t_tarjan_vertex *vertex, int *current_num, t_stack *stack);

/**
 * @brief Processes a single neighbor during Tarjan's DFS traversal.
 *
 * If the neighbor is unvisited, recursively visits it.
 * If the neighbor is on the stack, updates the low-link value.
 *
 * @param graph The graph being traversed.
 * @param tarjan_vertices Array of all Tarjan vertices.
 * @param curr The current vertex being processed.
 * @param neighbor_id ID of the neighbor to process.
 * @param current_num Pointer to the current discovery number.
 * @param partition The partition being built.
 * @param stack The stack.
 */
static void processTarjanNeighbor(
        t_graph *graph,
        t_tarjan_vertex **tarjan_vertices,
        t_tarjan_vertex *curr,
        int neighbor_id,
        int *current_num,
        t_partition *partition,
        t_stack *stack);

/**
 * @brief Visits all neighbors of the current vertex in the DFS traversal.
 *
 * @param graph The graph being traversed.
 * @param tarjan_vertices Array of all Tarjan vertices.
 * @param curr The current vertex being processed.
 * @param current_num Pointer to the current discovery number.
 * @param partition The partition being built.
 * @param stack The stack.
 */
static void visitTarjanNeighbors(
        t_graph *graph,
        t_tarjan_vertex **tarjan_vertices,
        t_tarjan_vertex *curr,
        int *current_num,
        t_partition *partition,
        t_stack *stack);

/**
 * @brief Extracts a strongly connected component from the stack.
 *
 * @param graph The graph being traversed.
 * @param tarjan_vertices Array of all Tarjan vertices.
 * @param curr The root vertex of the SCC.
 * @param partition The partition to add the new class to.
 * @param stack The stack.
 */
static void extractStronglyConnectedComponent(
        t_graph *graph,
        t_tarjan_vertex **tarjan_vertices,
        t_tarjan_vertex *curr,
        t_partition *partition,
        t_stack *stack);

/**
 * @brief Performs a DFS visit on a vertex using Tarjan's algorithm.
 *
 * This is the core recursive function of Tarjan's algorithm. It:
 * 1. Marks the vertex as visited and pushes it onto the stack
 * 2. Recursively visits all unvisited neighbors
 * 3. Updates low-link values based on neighbors
 * 4. Extracts an SCC if this vertex is a root
 *
 * @param graph The graph being traversed.
 * @param tarjan_vertices Array of all Tarjan vertices.
 * @param vertex_id ID of the vertex to visit.
 * @param current_num Pointer to the current discovery number.
 * @param partition The partition being built.
 * @param stack The stack.
 */
static void tarjanVisit(
        t_graph *graph,
        t_tarjan_vertex **tarjan_vertices,
        int vertex_id,
        int *current_num,
        t_partition *partition,
        t_stack *stack);


#endif //TARJAN_H
