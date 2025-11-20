#ifndef TARJAN_H
#define TARJAN_H

#define UNVISITED (-1)
#define DEBUG_TARJAN 1

#include "partition.h"
#include "graph.h"
#include "tarjan_vertex.h"
#include "utils.h"

#include "tarjan.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

t_tarjan_vertex** graphToTarjanVertices(t_graph graph);
void freeTarjanVerticesPartial(t_tarjan_vertex **tarjan_vertices, int count);
static void initializeTarjanVertex(t_tarjan_vertex *vertex, int *current_num, t_stack *stack);
static void processTarjanNeighbor(
        t_graph *graph,
        t_tarjan_vertex **tarjan_vertices,
        t_tarjan_vertex *curr,
        int neighbor_id,
        int *current_num,
        t_partition *partition,
        t_stack *stack);
static void visitTarjanNeighbors(
        t_graph *graph,
        t_tarjan_vertex **tarjan_vertices,
        t_tarjan_vertex *curr,
        int *current_num,
        t_partition *partition,
        t_stack *stack);
static void extractStronglyConnectedComponent(
        t_graph *graph,
        t_tarjan_vertex **tarjan_vertices,
        t_tarjan_vertex *curr,
        t_partition *partition,
        t_stack *stack);
void tarjanVisit(
        t_graph *graph,
        t_tarjan_vertex **tarjan_vertices,
        int vertex_id,
        int *current_num,
        t_partition *partition,
        t_stack *stack);
t_partition *tarjan(t_graph graph);


#endif //TARJAN_H
