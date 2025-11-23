#include "tarjan.h"
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "tarjan_vertex.h"

/**
 * @brief Frees a partially allocated array of Tarjan vertices.
 *
 * Used for cleanup when allocation fails midway through vertex creation.
 *
 * @param tarjan_vertices Array of Tarjan vertices to free.
 * @param count Number of vertices that were successfully allocated.
 */
void freeTarjanVerticesPartial(t_tarjan_vertex **tarjan_vertices, int count) {
    if (tarjan_vertices == NULL) {
        return;
    }
    for (int i = 0; i < count; ++i) {
        if (tarjan_vertices[i] != NULL) {
            freeTarjanVertex(tarjan_vertices[i]);
        }
    }
    free(tarjan_vertices);
}

/**
 * @brief Converts a graph into an array of Tarjan vertices.
 *
 * Creates one Tarjan vertex for each vertex in the graph, all initialized
 * as unvisited and not on the stack.
 *
 * @param graph The graph to convert.
 * @return Array of Tarjan vertices, or NULL on allocation failure.
 */
t_tarjan_vertex** graphToTarjanVertices(t_graph graph) {
    int size = graph.size;
    if (size <= 0) {
        fprintf(stderr, "graphToTarjanVertices: invalid graph size\n");
        return NULL;
    }

    t_tarjan_vertex **tarjan_vertices = calloc(size, sizeof(t_tarjan_vertex*));
    if (tarjan_vertices == NULL) {
        perror("graphToTarjanVertices: allocation failed");
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        int vertex_id = i + 1;
        tarjan_vertices[i] = createTarjanVertex(vertex_id, UNVISITED, UNVISITED, FALSE);
        if (tarjan_vertices[i] == NULL) {
            perror("graphToTarjanVertices: vertex creation failed");
            freeTarjanVerticesPartial(tarjan_vertices, i);
            return NULL;
        }
    }

    return tarjan_vertices;
}

/**
 * @brief Initializes a Tarjan vertex during the first visit.
 *
 * Sets the vertex's discovery number and low-link value, then pushes it onto the stack.
 *
 * @param vertex The vertex to initialize.
 * @param current_num Pointer to the current discovery number counter.
 * @param stack The DFS stack.
 */
void initializeTarjanVertex(t_tarjan_vertex *vertex, int *current_num, t_stack *stack) {
    vertex->num = *current_num;
    vertex->num_accessible = *current_num;
    (*current_num)++;
    pushStack(stack, vertex->id);
    vertex->in_pile = TRUE;
}

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
 * @param stack The DFS stack.
 */
void processTarjanNeighbor(
        t_graph *graph,
        t_tarjan_vertex **tarjan_vertices,
        t_tarjan_vertex *curr,
        int neighbor_id,
        int *current_num,
        t_partition *partition,
        t_stack *stack) {
    t_tarjan_vertex *neighbor_tv = tarjan_vertices[neighbor_id - 1];

    if (neighbor_tv->num == UNVISITED) {
        // Recursively visit unvisited neighbors
        tarjanVisit(graph, tarjan_vertices, neighbor_id, current_num, partition, stack);
        // Update low-link value based on neighbor's low-link
        curr->num_accessible = minInt(curr->num_accessible, neighbor_tv->num_accessible);
    } else if (neighbor_tv->in_pile == TRUE) {
        // If neighbor is on stack, it's part of the current SCC
        curr->num_accessible = minInt(curr->num_accessible, neighbor_tv->num);
    }
}

/**
 * @brief Visits all neighbors of the current vertex in the DFS traversal.
 *
 * @param graph The graph being traversed.
 * @param tarjan_vertices Array of all Tarjan vertices.
 * @param curr The current vertex being processed.
 * @param current_num Pointer to the current discovery number.
 * @param partition The partition being built.
 * @param stack The DFS stack.
 */
void visitTarjanNeighbors(
        t_graph *graph,
        t_tarjan_vertex **tarjan_vertices,
        t_tarjan_vertex *curr,
        int *current_num,
        t_partition *partition,
        t_stack *stack) {
    t_list *neighbors = getNeighbors(graph, curr->id);
    if (neighbors == NULL || neighbors->head == NULL) return;

    t_cell *cell = neighbors->head;
    while (cell != NULL) {
        processTarjanNeighbor(graph,
                                tarjan_vertices,
                                curr,
                                cell->vertex,
                                current_num,
                                partition,
                                stack);
        cell = cell->next;
    }
}

/**
 * @brief Extracts a strongly connected component from the stack.
 *
 * When a root vertex is found (num_accessible == num), all vertices on the stack
 * up to and including this vertex form one SCC. This function pops them off
 * and creates a new class in the partition.
 *
 * @param graph The graph being traversed.
 * @param tarjan_vertices Array of all Tarjan vertices.
 * @param curr The root vertex of the SCC.
 * @param partition The partition to add the new class to.
 * @param stack The DFS stack.
 */
void extractStronglyConnectedComponent(
        t_graph *graph,
        t_tarjan_vertex **tarjan_vertices,
        t_tarjan_vertex *curr,
        t_partition *partition,
        t_stack *stack) {
    int class_has_members = FALSE;

    // Create a new class for this SCC
    int new_class_id = generateClassId(*partition);
    t_class *new_class = createClass(new_class_id);
    if (new_class == NULL) return;

    // Pop vertices from stack until we reach the root vertex
    int w_id;
    do {
        if (stack->top == NULL) break;
        w_id = popStack(stack);
        if (w_id == STACK_EMPTY || w_id < 1 || w_id > graph->size) break;

        t_tarjan_vertex *w_tv = tarjan_vertices[w_id - 1];
        w_tv->in_pile = FALSE;
        addVertexToClass(new_class, w_id);
        class_has_members = TRUE;
    } while (w_id != curr->id);

    // Add the class to the partition if it has members
    if (class_has_members) {
        addClassToPartition(partition, new_class);
    } else {
        freeClass(new_class);
    }
}

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
 * @param stack The DFS stack.
 */
void tarjanVisit(
        t_graph *graph,
        t_tarjan_vertex **tarjan_vertices,
        int vertex_id,
        int *current_num,
        t_partition *partition,
        t_stack *stack) {
    if (graph == NULL || tarjan_vertices == NULL || current_num == NULL ||
        partition == NULL || stack == NULL) {
        fprintf(stderr, "tarjanVisit: invalid arguments\n");
        return;
    }
    if (vertex_id < 1 || vertex_id > graph->size) {
        fprintf(stderr, "tarjanVisit: vertex_id out of bounds\n");
        return;
    }

    t_tarjan_vertex *curr = tarjan_vertices[vertex_id - 1];

    // Initialize this vertex (first visit)
    initializeTarjanVertex(curr, current_num, stack);

    // Visit all neighbors
    visitTarjanNeighbors(graph, tarjan_vertices, curr, current_num, partition, stack);

    // Check if this vertex is the root of an SCC
    if (curr->num_accessible == curr->num) {
        extractStronglyConnectedComponent(graph, tarjan_vertices, curr, partition, stack);
    }
}

t_partition *tarjan(t_graph graph) {
    // Step 1: Create empty partition
    printf("Step 1: Creating partition structure...\n");
    t_partition *partition = createPartition();
    if (partition == NULL) {
        fprintf(stderr, "Error: Failed to create partition\n");
        return NULL;
    }

    if (graph.size <= 0) {
        printf("Warning: Graph is empty\n");
        return partition;
    }

    printf("Graph size: %d vertices\n", graph.size);

    // Step 2: Convert graph to Tarjan vertices
    printf("Step 2: Initializing Tarjan data structures...\n");
    t_tarjan_vertex **tarjan_vertices = graphToTarjanVertices(graph);
    if (tarjan_vertices == NULL) {
        fprintf(stderr, "Error: Failed to create Tarjan vertices\n");
        freePartition(partition);
        return NULL;
    }

    // Step 3: Create stack for DFS
    t_stack *stack = createStack();
    if (stack == NULL) {
        fprintf(stderr, "Error: Failed to create stack\n");
        freeTarjanVerticesPartial(tarjan_vertices, graph.size);
        freePartition(partition);
        return NULL;
    }

    // Step 4: Run Tarjan's algorithm
    printf("Step 3: Running depth-first search to identify SCCs...\n");
    int current_num = 0;
    int components_found = 0;

    for (int i = 0; i < graph.size; i++) {
        if (tarjan_vertices[i] == NULL) continue;
        if (tarjan_vertices[i]->num == UNVISITED) {
            printf("  Starting DFS from vertex %d\n", tarjan_vertices[i]->id);
            int previous_class_count = partition->class_number;
            tarjanVisit(&graph, tarjan_vertices, tarjan_vertices[i]->id, &current_num, partition, stack);

            // Check if new components were found
            if (partition->class_number > previous_class_count) {
                components_found += (partition->class_number - previous_class_count);
            }
        }
    }

    // Step 5: Clean up
    freeTarjanVerticesPartial(tarjan_vertices, graph.size);
    freeStack(stack);

    printf("Step 4: Complete!\n");
    printf("Found %d strongly connected component(s)\n", partition->class_number);
    printf("\n");

    return partition;
}
