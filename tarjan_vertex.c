#include "tarjan_vertex.h"
#include <stdlib.h>
#include <stdio.h>


t_tarjan_vertex * createTarjanVertex(int id, int num, int num_accessible, int in_pile) {
    t_tarjan_vertex * tarjan_vertex = (t_tarjan_vertex *)malloc(sizeof(t_tarjan_vertex));
    if (tarjan_vertex == NULL) {
        perror("createTarjanVertex: allocation failed");
        return NULL;
    }
    tarjan_vertex->id = id;
    tarjan_vertex->num = num;
    tarjan_vertex->id = num_accessible;
    tarjan_vertex->in_pile = in_pile;
    tarjan_vertex->next = NULL;
    return tarjan_vertex;
}

void displayTarjanVertex(t_tarjan_vertex tarjan_vertex) {
    printf("(%d, %d, %d, %d )", tarjan_vertex.id, tarjan_vertex.num,tarjan_vertex.num_accessible, tarjan_vertex.in_pile);
}

void freeTarjanVertex(t_tarjan_vertex* tarjan_vertex) {
    if (tarjan_vertex == NULL) return;
    free(tarjan_vertex);
}