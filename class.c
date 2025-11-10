#include "class.h"
#include <string.h>
#include <stdio.h>

t_class* createClass(const char *name) {
    t_class* class = (t_class *)malloc(sizeof(t_class));
    if (class == NULL) {
        perror("createClass: allocation failed");
        return NULL;
    }
    strcpy(class->name, name);
    class->vertexes = NULL;
    class->next = NULL;
    return class;
}

void addVertexToClass(t_class *class, t_tarjan_vertex *vertex) {
    if (class == NULL || vertex == NULL) return;
    vertex->next = class->vertexes;
    class->vertexes = vertex;
}

void displayClass(t_class *class) {
    if (class == NULL) return;
    printf("Class %s:\n", class->name);
    t_tarjan_vertex *curr = class->vertexes;
    while (curr != NULL) {
        displayTarjanVertex(*curr);
        printf("->");
        curr = curr->next;
    }
    printf("NULL\n");
}

void freeVertexes(t_tarjan_vertex* vertex) {
    if (vertex == NULL) return;
    freeVertexes(vertex->next);
    freeTarjanVertex(vertex);
}

void freeClass(t_class *class) {
    if (class == NULL) return;
    freeVertexes(class->vertexes);
    free(class);
}

