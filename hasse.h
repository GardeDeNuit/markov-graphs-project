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

// Crée un tableau : class_array[sommet] = numéro de classe
int* makeClassArray(t_graph *graph, t_partition *partition);

/**
 * @brief Creates a link array from the given partition and graph.
 *
 * @param part The partition of the graph.
 * @param graph The adjacency list representation of the graph.
 * @return The created link array.
 */

#endif