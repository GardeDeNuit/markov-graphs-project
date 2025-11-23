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


void removeTransitiveLinks(t_link_array* p_link_array);

int linkExists(t_link_array link_array, int src, int dest);

int addLink(t_link_array *link_array, int src, int dest);

void displayHasseDiagram(t_hasse_diagram hasse);

t_class_type_array createClassTypeArray(t_link_array link_array);

void freeClassTypeArray(t_class_type_array type_array);

t_association_array createAssociationArray(t_graph graph, t_partition partition);


int isPersistantClass(t_hasse_diagram hasse, int class_id);

int isAbsorbingState(t_hasse_diagram hasse, int state_id, int graph_size);

int isIrreductible(t_hasse_diagram hasse);

t_hasse_diagram createHasseDiagram(t_graph g);

void displayGraphCharacteristics(t_hasse_diagram hasse, t_graph graph);

#endif