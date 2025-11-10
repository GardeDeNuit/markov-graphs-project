#include "tarjan.h"

t_tarjan_vertex graphToTarjan(t_graph graph) {
    int size = graph.size;
    t_tarjan_vertex *new_vertex;
    t_tarjan_vertex tarjan_vertices[size];
    for (int i = 0; i < size; i++) {
        int vertex_id = i + 1;
        int num = -1;
        int num_accessible = -1;
        int in_pile = 0;
        new_vertex = createTarjanVertex(vertex_id, num, num_accessible, in_pile);
        tarjan_vertices[i] = *new_vertex;
    }
    return *tarjan_vertices;
}

