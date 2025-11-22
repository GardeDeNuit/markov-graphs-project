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
int* createClassArrayFromPartition(t_graph *graph, t_partition *partition);

// Crée un tableau pour stocker les noms des classes (composantes)
char** createClassNamesFromPartition(t_partition *partition, int num_classes);

void addLink(t_link_array *link_array, int dept, int dest);
int linkExists(t_link_array *link_array, int dept, int dest);
void removeTransitiveLinks(t_link_array *p_link_array);
void displayHasseLinksDetailed(t_link_array *links, t_partition *partition, int *class_array, int num_classes);
void displayHasseLinks(t_link_array *links, t_partition *partition, int *class_array, int num_classes);

int *ClassType(int*,int,t_link_array);
int isAbsorbingState(int*,int,int*);
int isIrreductible(int*,int);
/**
 * @brief Creates a link array from the given partition and graph.
 *
 * @param part The partition of the graph.
 * @param graph The adjacency list representation of the graph.
 * @return The created link array.
 */

#endif