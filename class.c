#include "class.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

t_class* createClass(int class_id) {
    t_class* class = (t_class *)malloc(sizeof(t_class));
    if (class == NULL) {
        perror("createClass: allocation failed");
        return NULL;
    }
    class->id = class_id;
    class->vertices = NULL;
    class->vertex_number = 0;
    class->next = NULL;
    return class;
}

t_vertex* createVertex(int value) {
    t_vertex* vertex = (t_vertex *)malloc(sizeof(t_vertex));
    if (vertex == NULL) {
        perror("createVertex: allocation failed");
        return NULL;
    }
    vertex->value = value;
    vertex->next = NULL;
    return vertex;
}

int addVertexToClass(t_class *class, int value) {
    if (class == NULL) {
        fprintf(stderr, "addVertexToClass: class pointer is NULL\n");
        return -1;
    }
    t_vertex *new_vertex = createVertex(value);
    if (new_vertex == NULL) {
        fprintf(stderr, "addVertexToClass: failed to create vertex\n");
        return -1;
    }
    new_vertex->next = class->vertices;
    class->vertices = new_vertex;
    class->vertex_number++;
    return 1;
}

/**
 * @brief Displays all vertices in a linked list
 * @param vertex Pointer to the first vertex to display
 */
static void displayVertices(t_vertex *vertex) {
    t_vertex *curr = vertex;
    while (curr != NULL) {
        if (curr->next == NULL)
            printf("%d", curr->value);
        else
            printf("%d,", curr->value);
        curr = curr->next;
    }
}

void displayClass(t_class *class) {
    if (class == NULL) {
        fprintf(stderr, "displayClass: class pointer is NULL\n");
        return;
    }
    printf("Class %s: {", getID(class->id));
    displayVertices(class->vertices);
    printf("}");
}

void freeVertices(t_vertex *vertex) {
    if (vertex == NULL) return;
    freeVertices(vertex->next);
    free(vertex);
}

int freeClass(t_class *class) {
    if (class == NULL) {
        fprintf(stderr, "freeClass: class pointer is NULL\n");
        return -1;
    }
    freeVertices(class->vertices);
    free(class);
    return 1;
}
