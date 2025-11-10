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

void addVertexToClass(t_class *class, int value) {
    if (class == NULL) return;

    t_vertex *new_vertex = malloc(sizeof(t_vertex));
    if (new_vertex == NULL) {
        perror("addVertexToClass: allocation failed");
        return;
    }

    new_vertex->value = value;
    new_vertex->next = class->vertexes;
    class->vertexes = new_vertex;
}

void displayClass(t_class *class) {
    if (class == NULL) return;
    printf("Class %s: ", class->name);

    t_vertex *curr = class->vertexes;
    while (curr != NULL) {
        printf("%d -> ", curr->value);
        curr = curr->next;
    }
    printf("NULL\n");
}

void freeVertexes(t_vertex *vertex) {
    if (vertex == NULL) return;
    freeVertexes(vertex->next);
    free(vertex);
}

void freeClass(t_class *class) {
    if (class == NULL) return;
    freeVertexes(class->vertexes);
    free(class);
}
