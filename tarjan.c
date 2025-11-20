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
        fprintf(stderr, "graphToTarjanVertices: invalid graph size\n");
        return NULL;
    }
    t_tarjan_vertex **tarjan_vertices = calloc(size, sizeof(t_tarjan_vertex*));
    if (tarjan_vertices == NULL) {
        debugPrint(DEBUG_TARJAN, "graphToTarjanVertices: allocation failed");
        perror("graphToTarjanVertices: allocation failed");
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        int vertex_id = i + 1;
        tarjan_vertices[i] = createTarjanVertex(vertex_id, UNVISITED, UNVISITED, FALSE);
        if (tarjan_vertices[i] == NULL) {
            debugPrint(DEBUG_TARJAN, "graphToTarjanVertices: vertex creation failed");
            perror("graphToTarjanVertices: vertex creation failed");
            freeTarjanVerticesPartial(tarjan_vertices, i);
            return NULL;
        }
    }
    debugPrint(DEBUG_TARJAN, "graphToTarjanVertices: success");
    return tarjan_vertices;
}

static void initializeTarjanVertex(t_tarjan_vertex *vertex, int *current_num, t_stack *stack) {
    vertex->num = *current_num;
    vertex->num_accessible = *current_num;
    (*current_num)++;
    pushStack(stack, vertex->id);
    vertex->in_pile = TRUE;
    debugPrint(DEBUG_TARJAN, "tarjanVisit: vertex pushed to stack");
}

static void processTarjanNeighbor(
        t_graph *graph,
        t_tarjan_vertex **tarjan_vertices,
        t_tarjan_vertex *curr,
        int neighbor_id,
        int *current_num,
        t_partition *partition,
        t_stack *stack) {
    t_tarjan_vertex *neighbor_tv = tarjan_vertices[neighbor_id - 1];

    if (DEBUG_TARJAN == TRUE) printf("tarjanVisit: inspecting neighbor %d\n", neighbor_id);

    if (neighbor_tv->num == UNVISITED) {
        tarjanVisit(graph, tarjan_vertices, neighbor_id, current_num, partition, stack);
        curr->num_accessible = minInt(curr->num_accessible, neighbor_tv->num_accessible);
    } else if (neighbor_tv->in_pile == TRUE) {
        curr->num_accessible = minInt(curr->num_accessible, neighbor_tv->num);
    }
}

static void visitTarjanNeighbors(
        t_graph *graph,
        t_tarjan_vertex **tarjan_vertices,
        t_tarjan_vertex *curr,
        int *current_num,
        t_partition *partition,
        t_stack *stack) {
    t_list *neighbors = getNeighbors(graph, curr->id);
    if (neighbors == NULL || neighbors->head == NULL) return;

    t_cell *cell = neighbors->head;
    while (cell != NULL) {
        processTarjanNeighbor(graph,
                                tarjan_vertices,
                                curr,
                                cell->vertex,
                                current_num,
                                partition,
                                stack);
        cell = cell->next;
    }
}

static void extractStronglyConnectedComponent(
        t_graph *graph,
        t_tarjan_vertex **tarjan_vertices,
        t_tarjan_vertex *curr,
        t_partition *partition,
        t_stack *stack) {
    debugPrint(DEBUG_TARJAN, "tarjanVisit: root of SCC detected");
    int class_has_members = FALSE;

    // Creation d'une nouvelle classe pour la SCC
    // On commence par générer un nom unique
    char* class_name[CLASS_NAME_MAX_SIZE];
    generateClassName(*partition, class_name, CLASS_NAME_DEFAULT);
    t_class *new_class = createClass(class_name);
    if (new_class == NULL) return;

    int w_id;
    do {
        if (stack->top == NULL) break;
        w_id = popStack(stack);
        if (w_id == STACK_EMPTY || w_id < 1 || w_id > graph->size) break;

        t_tarjan_vertex *w_tv = tarjan_vertices[w_id - 1];
        w_tv->in_pile = FALSE;
        addVertexToClass(new_class, w_id);
        class_has_members = TRUE;
        if (DEBUG_TARJAN == TRUE) printf("tarjanVisit: vertex %d added to SCC\n", w_id);
    } while (w_id != curr->id);

    if (class_has_members) {
        debugPrint(DEBUG_TARJAN, "tarjanVisit: SCC completed");
        addClassToPartition(partition, new_class);
    } else {
        debugPrint(DEBUG_TARJAN, "tarjanVisit: SCC empty, freeing");
        freeClass(new_class);
    }
}

void tarjanVisit(
        t_graph *graph,
        t_tarjan_vertex **tarjan_vertices,
        int vertex_id,
        int *current_num,
        t_partition *partition,
        t_stack *stack) {
    debugPrint(DEBUG_TARJAN, "tarjanVisit: start");
    if (graph == NULL || tarjan_vertices == NULL || current_num == NULL ||
        partition == NULL || stack == NULL) {
        debugPrint(DEBUG_TARJAN, "tarjanVisit: invalid arguments");
        fprintf(stderr, "tarjanVisit: invalid arguments\n");
        return;
    }
    if (vertex_id < 1 || vertex_id > graph->size) {
        debugPrint(DEBUG_TARJAN, "tarjanVisit: vertex_id out of bounds");
        fprintf(stderr, "tarjanVisit: vertex_id out of bounds\n");
        return;
    }

    t_tarjan_vertex *curr = tarjan_vertices[vertex_id - 1];
    initializeTarjanVertex(curr, current_num, stack);

    if (DEBUG_TARJAN == TRUE) printf("[DEBUG] tarjanVisit: visiting vertex %d\n", curr->id);
    visitTarjanNeighbors(graph, tarjan_vertices, curr, current_num, partition, stack);

    if (curr->num_accessible == curr->num) {
        extractStronglyConnectedComponent(graph, tarjan_vertices, curr, partition, stack);
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
        debugPrint(DEBUG_TARJAN, "tarjan: stack creation failed");
        freeTarjanVerticesPartial(tarjan_vertices, graph.size);
        freePartition(partition);
        return NULL;
    }

    int current_num = 0;
    for (int i = 0; i < graph.size; i++) {
        if (tarjan_vertices[i] == NULL) continue;
        if (tarjan_vertices[i]->num == UNVISITED) {
            if (DEBUG_TARJAN == TRUE) printf("[DEBUG] tarjan: launching visit from %d\n", tarjan_vertices[i]->id);
            tarjanVisit(&graph, tarjan_vertices, tarjan_vertices[i]->id, &current_num, partition, stack);
        }
    }

    freeTarjanVerticesPartial(tarjan_vertices, graph.size);
    freeStack(stack);

    debugPrint(DEBUG_TARJAN, "tarjan: completed");
    return partition;
}
