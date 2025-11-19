#include "tarjan.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

void freeTarjanVerticesPartial(t_tarjan_vertex **tarjan_vertices, int count) {
    debugPrint(DEBUG_TARJAN, "freeTarjanVerticesPartial: start");
    if (tarjan_vertices == NULL) {
        debugPrint(DEBUG_TARJAN, "freeTarjanVerticesPartial: array is NULL");
        return;
    }
    for (int i = 0; i < count; ++i) {
        if (tarjan_vertices[i] != NULL) {
            debugPrint(DEBUG_TARJAN, "freeTarjanVerticesPartial: freeing entry");
            freeTarjanVertex(tarjan_vertices[i]);
        }
    }
    free(tarjan_vertices);
    debugPrint(DEBUG_TARJAN, "freeTarjanVerticesPartial: done");
}

t_tarjan_vertex** graphToTarjanVertices(t_graph graph) {
    debugPrint(DEBUG_TARJAN, "graphToTarjanVertices: start");
    int size = graph.size;
    if (size <= 0) {
        debugPrint(DEBUG_TARJAN, "graphToTarjanVertices: invalid graph size");
        return NULL;
    }
    t_tarjan_vertex **tarjan_vertices = calloc(size, sizeof(t_tarjan_vertex*));
    if (tarjan_vertices == NULL) {
        debugPrint(DEBUG_TARJAN, "graphToTarjanVertices: allocation failed");
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        int vertex_id = i + 1;
        tarjan_vertices[i] = createTarjanVertex(vertex_id, UNVISITED, UNVISITED, FALSE);
        if (tarjan_vertices[i] == NULL) {
            debugPrint(DEBUG_TARJAN, "graphToTarjanVertices: vertex creation failed");
            freeTarjanVerticesPartial(tarjan_vertices, i);
            return NULL;
        }
    }
    debugPrint(DEBUG_TARJAN, "graphToTarjanVertices: success");
    return tarjan_vertices;
}

void tarjanVisit(
        t_graph *graph,
        t_tarjan_vertex **tarjan_vertices,
        int vertex_id,
        int *current_num,
        t_partition *partition,
        t_stack *stack) {
    char msg[128];
    debugPrint(DEBUG_TARJAN, "tarjanVisit: start");
    if (graph == NULL || tarjan_vertices == NULL || current_num == NULL ||
        partition == NULL || stack == NULL) {
        debugPrint(DEBUG_TARJAN, "tarjanVisit: invalid arguments");
        return;
    }
    if (vertex_id < 1 || vertex_id > graph->size) {
        debugPrint(DEBUG_TARJAN, "tarjanVisit: vertex_id out of bounds");
        return;
    }

    t_tarjan_vertex *curr = tarjan_vertices[vertex_id - 1];
    if (curr == NULL) {
        debugPrint(DEBUG_TARJAN, "tarjanVisit: current vertex is NULL");
        return;
    }

    snprintf(msg, sizeof(msg), "tarjanVisit: visiting vertex %d", curr->id);
    debugPrint(DEBUG_TARJAN, msg);

    curr->num = *current_num;
    curr->num_accessible = *current_num;
    (*current_num)++;

    pushStack(stack, curr->id);
    curr->in_pile = TRUE;
    debugPrint(DEBUG_TARJAN, "tarjanVisit: vertex pushed to stack");

    t_list *neighbors = getNeighbors(graph, curr->id);
    if (neighbors != NULL && neighbors->head != NULL) {
        t_cell *cell = neighbors->head;
        while (cell != NULL) {
            int neighbor_id = cell->vertex;
            if (neighbor_id >= 1 && neighbor_id <= graph->size) {
                t_tarjan_vertex *neighbor_tv = tarjan_vertices[neighbor_id - 1];
                if (neighbor_tv != NULL) {
                    snprintf(msg, sizeof(msg), "tarjanVisit: inspecting neighbor %d", neighbor_id);
                    debugPrint(DEBUG_TARJAN, msg);
                    if (neighbor_tv->num == UNVISITED) {
                        tarjanVisit(graph, tarjan_vertices, neighbor_id, current_num, partition, stack);
                        curr->num_accessible = minInt(curr->num_accessible, neighbor_tv->num_accessible);
                    } else if (neighbor_tv->in_pile == TRUE) {
                        curr->num_accessible = minInt(curr->num_accessible, neighbor_tv->num);
                    }
                }
            }
            cell = cell->next;
        }
    }

    if (curr->num_accessible == curr->num) {
        debugPrint(DEBUG_TARJAN, "tarjanVisit: root of SCC detected");
        int class_has_members = FALSE;
        t_class *new_class = createClass(CLASS_NAME_DEFAULT);
        if (new_class == NULL) return;

        int w_id;
        do {
            if (stack->top == NULL) break;
            w_id = popStack(stack);
            if (w_id == STACK_EMPTY || w_id < 1 || w_id > graph->size) break;

            t_tarjan_vertex *w_tarjan_vertex = tarjan_vertices[w_id - 1];
            if (w_tarjan_vertex != NULL) {
                w_tarjan_vertex->in_pile = FALSE;
                addVertexToClass(new_class, w_id);
                class_has_members = TRUE;
                snprintf(msg, sizeof(msg), "tarjanVisit: vertex %d added to SCC", w_id);
                debugPrint(DEBUG_TARJAN, msg);
            }
        } while (w_id != curr->id);

        if (class_has_members) {
            debugPrint(DEBUG_TARJAN, "tarjanVisit: SCC completed");
            addClassToPartition(partition, new_class);
        } else {
            debugPrint(DEBUG_TARJAN, "tarjanVisit: SCC empty, freeing");
            freeClass(new_class);
        }
    }
}

t_partition *tarjan(t_graph graph) {
    debugPrint(DEBUG_TARJAN, "tarjan: start");
    t_partition *partition = createPartition();
    if (partition == NULL) {
        debugPrint(DEBUG_TARJAN, "tarjan: partition allocation failed");
        return NULL;
    }

    if (graph.size <= 0) {
        debugPrint(DEBUG_TARJAN, "tarjan: empty graph");
        return partition;
    }

    t_tarjan_vertex **tarjan_vertices = graphToTarjanVertices(graph);
    if (tarjan_vertices == NULL) {
        debugPrint(DEBUG_TARJAN, "tarjan: tarjan vertices creation failed");
        freePartition(partition);
        return NULL;
    }

    t_stack *stack = createStack();
    if (stack == NULL) {
        debugPrint(DEBUG_TARJAN, "tarjan: stack allocation failed");
        freeTarjanVerticesPartial(tarjan_vertices, graph.size);
        freePartition(partition);
        return NULL;
    }

    int current_num = 0;
    for (int i = 0; i < graph.size; i++) {
        if (tarjan_vertices[i] == NULL) continue;
        if (tarjan_vertices[i]->num == UNVISITED) {
            char msg[128];
            snprintf(msg, sizeof(msg), "tarjan: launching visit from %d", tarjan_vertices[i]->id);
            debugPrint(DEBUG_TARJAN, msg);
            tarjanVisit(&graph, tarjan_vertices, tarjan_vertices[i]->id, &current_num, partition, stack);
        }
    }

    freeTarjanVerticesPartial(tarjan_vertices, graph.size);
    freeStack(stack);
    debugPrint(DEBUG_TARJAN, "tarjan: completed");
    return partition;
}
