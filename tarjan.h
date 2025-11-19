#ifndef TARJAN_H
#define TARJAN_H

#define UNVISITED -1
#define DEFAULT_CLASS_NAME "SCC"

#include "partition.h"
#include "graph.h"
#include "tarjan_vertex.h"
#include "utils.h"

t_tarjan_vertex** graphToTarjanVertices(t_graph);

void freeTarjanVertices(t_tarjan_vertex **, int);

void tarjanVisit(t_graph *,
                    t_tarjan_vertex **,
                    int ,
                    int *,
                    t_partition *,
                    t_stack *);

t_partition *tarjan(t_graph);

#endif //TARJAN_H
