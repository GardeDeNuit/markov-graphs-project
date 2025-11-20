#include "tarjan_vertex.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

t_tarjan_vertex * createTarjanVertex(int id, int num, int num_accessible, int in_pile) {
    debugPrint(DEBUG_TARJAN_VERTEX, "createTarjanVertex: start");

    if (id < 0) {
        debugPrint(DEBUG_TARJAN_VERTEX, "createTarjanVertex: id must be >= 0");
        fprintf(stderr, "createTarjanVertex: id must be >= 0\n");
        return NULL;
    }

    if (in_pile != 0 && in_pile != 1) {
        debugPrint(DEBUG_TARJAN_VERTEX, "createTarjanVertex: in_pile must be 0 or 1");
        fprintf(stderr, "createTarjanVertex: in_pile must be 0 or 1\n");
        return NULL;
    }

    t_tarjan_vertex * tarjan_vertex = (t_tarjan_vertex *)malloc(sizeof(t_tarjan_vertex));
    if (tarjan_vertex == NULL) {
        debugPrint(DEBUG_TARJAN_VERTEX, "createTarjanVertex: allocation failed");
        perror("createTarjanVertex: allocation failed");
        return NULL;
    }

    tarjan_vertex->id = id;
    tarjan_vertex->num = num;
    tarjan_vertex->num_accessible = num_accessible;
    tarjan_vertex->in_pile = in_pile;
    tarjan_vertex->next = NULL;

    debugPrint(DEBUG_TARJAN_VERTEX, "createTarjanVertex: success");
    return tarjan_vertex;
}

void displayTarjanVertex(t_tarjan_vertex tarjan_vertex) {
    printf("(%d, %d, %d, %d)", tarjan_vertex.id, tarjan_vertex.num, tarjan_vertex.num_accessible, tarjan_vertex.in_pile);
}

void freeTarjanVertex(t_tarjan_vertex* tarjan_vertex) {
    debugPrint(DEBUG_TARJAN_VERTEX, "freeTarjanVertex: start");
    if (tarjan_vertex == NULL) {
        debugPrint(DEBUG_TARJAN_VERTEX, "freeTarjanVertex: tarjan_vertex is NULL");
        return;
    }
    free(tarjan_vertex);
    debugPrint(DEBUG_TARJAN_VERTEX, "freeTarjanVertex: done");
}