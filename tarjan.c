#include "tarjan.h"
#include <stdlib.h>
#include <string.h>

t_tarjan_vertex** graphToTarjanVertices(t_graph graph) {
    int size = graph.size;
    if (size <= 0) return NULL;
    t_tarjan_vertex **tarjan_vertices = malloc(size * sizeof(t_tarjan_vertex*));
    if (tarjan_vertices == NULL) return NULL;
    for (int i = 0; i < size; i++) {
        int vertex_id = i + 1;
        int num = UNVISITED;
        int num_accessible = UNVISITED;
        int in_pile = FALSE;
        tarjan_vertices[i] = createTarjanVertex(vertex_id, num, num_accessible, in_pile);
    }
    return tarjan_vertices;
}

void freeTarjanVertices(t_tarjan_vertex **tarjan_vertices, int size) {
    if (tarjan_vertices == NULL) return;
    for (int i = 0; i < size; i++) {
        freeTarjanVertex(tarjan_vertices[i]);
    }
    free(tarjan_vertices);
}

void tarjanVisit(t_graph *graph,
                    t_tarjan_vertex **tarjan_vertices,
                    int vertex_id,
                    int *current_num,
                    t_partition *partition,
                    t_stack *stack)
{
    if (graph == NULL || tarjan_vertices == NULL || current_num == NULL ||
        partition == NULL || stack == NULL) return;

    if (vertex_id < 1 || vertex_id > graph->size) return;

    t_tarjan_vertex *curr = tarjan_vertices[vertex_id - 1];
    if (curr == NULL) return;

    curr->num = *current_num;
    curr->num_accessible = *current_num;
    (*current_num)++;

    pushStack(stack, curr->id);
    curr->in_pile = TRUE;

    t_list *neighbors = getNeighbors(graph, curr->id);
    t_cell *cell = (neighbors != NULL) ? neighbors->head : NULL;
    while (cell != NULL) {
        int neighbor_id = cell->vertex;
        if (neighbor_id >= 1 && neighbor_id <= graph->size) {
            t_tarjan_vertex *neighbor_tv = tarjan_vertices[neighbor_id - 1];
            if (neighbor_tv == NULL) {
                cell = cell->next;
                continue;
            }

            if (neighbor_tv->num == UNVISITED) {
                tarjanVisit(graph, tarjan_vertices, neighbor_id, current_num, partition, stack);
                curr->num_accessible = minInt(curr->num_accessible, neighbor_tv->num_accessible);
            } else if (neighbor_tv->in_pile == TRUE) {
                curr->num_accessible = minInt(curr->num_accessible, neighbor_tv->num);
            }
        }
        cell = cell->next;
    }

    // Si racine d'une composante fortement connexe
    if (curr->num_accessible == curr->num) {
        t_class *new_class = createClass("SCC");
        if (new_class == NULL) return;

        int w_id;
        do {
            // Vérifier que la pile n'est pas vide avant de dépiler
            if (stack->top == NULL) break;

            w_id = popStack(stack);
            if (w_id < 1 || w_id > graph->size) break;

            t_tarjan_vertex *w_tarjan_vertex = tarjan_vertices[w_id - 1];
            if (w_tarjan_vertex != NULL) {
                w_tarjan_vertex->in_pile = FALSE;
                addVertexToClass(new_class, w_id);
            }
        } while (w_id != curr->id);

        addClassToPartition(partition, new_class);
    }
}


t_partition *tarjan(t_graph graph) {
    t_partition *partition = createPartition();
    if (partition == NULL) return NULL;

    t_tarjan_vertex **tarjan_vertices = graphToTarjanVertices(graph);
    if (tarjan_vertices == NULL) {
        // pas d'alloc -> partition vide retournée
        return partition;
    }

    t_stack *stack = createStack();
    if (stack == NULL) {
        freeTarjanVertices(tarjan_vertices, graph.size);
        return partition;
    }

    int current_num = 0;

    for (int i = 0; i < graph.size; i++) {
        if (tarjan_vertices[i] == NULL) continue;
        if (tarjan_vertices[i]->num == UNVISITED) {
            tarjanVisit(&graph, tarjan_vertices, tarjan_vertices[i]->id, &current_num, partition, stack);
        }
    }

    // Libération de la mémoire
    freeTarjanVertices(tarjan_vertices, graph.size);
    freeStack(stack);

    return partition;
}