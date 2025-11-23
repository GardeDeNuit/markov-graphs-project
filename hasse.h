#ifndef __HASSE_H__
#define __HASSE_H__

#include "partition.h"
#include "graph.h"

typedef struct s_link {
    int src_id;
    int dest_id;
} t_link;

typedef int* t_class_type_array;
typedef int* t_association_array;

typedef struct s_link_array {
    t_link* links;
    t_partition *partition;
    t_association_array association_array;
    int logical_size;
    int physical_size;
} t_link_array;

typedef t_link_array t_hasse_diagram;

/* private functions =================================================== */

static void removeTransitiveLinks(t_link_array* link_array);
static int linkExists(t_link_array link_array, int dept, int dest);
static int addLink(t_link_array *link_array, int dept, int dest);
static char** buildName(t_partition *partition, int num_classes);
static void displayHasseDiagram(t_hasse_diagram hasse);
t_class_type_array createClassTypeArray(t_link_array link_array);
int isAbsorbingState(int* class_sizes,int class_nb,t_class_type_array type_array);
int isIrreductible(t_class_type_array type_array,t_link_array link_array);

t_association_array createAssociationArray(t_graph *graph, t_partition *partition);

void addLinkToHasseDiagram(t_link_array *hasse, t_graph g, int *class_array);

#endif