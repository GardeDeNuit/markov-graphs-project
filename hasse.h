#ifndef __HASSE_H__
#define __HASSE_H__

#include "partition.h"
#include "graph.h"

struct s_link {
    int src_nb;
    int dest_nb;
};

typedef struct s_link t_link;

struct s_link_array {
    t_link * links;
    int log_size;
    int max_size;
};


typedef struct s_link_array t_link_array;

/**
 * @brief Build an array mapping each vertex to its class index.
 * @param graph Pointer to the graph structure.
 * @param partition Partition containing the classes and their vertices.
 * @return Dynamically allocated array of size graph->size, where each entry
 *         stores the class index of the corresponding vertex (0-based).
 *         The caller is responsible for freeing this array.
 */
int* makeClassArray(t_graph *graph, t_partition *partition);

#endif