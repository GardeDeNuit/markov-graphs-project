#include "class.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>

t_class* createClass(const char *name) {
    debugPrint(DEBUG_CLASS, "createClass: start");
    if (strlen(name) >= CLASS_NAME_MAX_SIZE) {
        debugPrint(DEBUG_CLASS, "createClass: name too long");
        fprintf(stderr,"createClass: name too long\n");
        return NULL;
    }
    t_class* class = (t_class *)malloc(sizeof(t_class));
    if (class == NULL) {
        debugPrint(DEBUG_CLASS, "createClass: allocation failed");
        perror("createClass: allocation failed");
        return NULL;
    }
    strcpy(class->name, name);
    class->vertices = NULL;
    class->next = NULL;
    debugPrint(DEBUG_CLASS, "createClass: success");
    return class;
}

t_vertex* createVertex(int value) {
    debugPrint(DEBUG_CLASS, "createVertex: start");
    t_vertex* vertex = (t_vertex *)malloc(sizeof(t_vertex));
    if (vertex == NULL) {
        debugPrint(DEBUG_CLASS, "createVertex: allocation failed");
        perror("createVertex: allocation failed");
        return NULL;
    }
    vertex->value = value;
    vertex->next = NULL;
    debugPrint(DEBUG_CLASS, "createVertex: success");
    return vertex;
}

void addVertexToClass(t_class *class, int value) {
    debugPrint(DEBUG_CLASS, "addVertexToClass: start");
    if (class == NULL) {
        debugPrint(DEBUG_CLASS, "addVertexToClass: class is NULL");
        fprintf(stderr,"addVertexToClass: class is NULL\n");
        return;
    }
    t_vertex *new_vertex = createVertex(value);
    if (new_vertex == NULL) {
        debugPrint(DEBUG_CLASS, "addVertexToClass: createVertex failed");
        return;
    }
    new_vertex->next = class->vertices;
    class->vertices = new_vertex;
    debugPrint(DEBUG_CLASS, "addVertexToClass: vertex added");
}

void displayVertices(t_vertex *vertex) {
    debugPrint(DEBUG_CLASS, "displayVertices: start");
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
    debugPrint(DEBUG_CLASS, "displayClass: start");
    if (class == NULL) {
        debugPrint(DEBUG_CLASS, "displayClass: class is NULL");
        return;
    }
    printf("Class %s: {", class->name);
    displayVertices(class->vertices);
    printf("}\n");
    debugPrint(DEBUG_CLASS, "displayClass: done");
}

void freeVertices(t_vertex *vertex) {
    debugPrint(DEBUG_CLASS, "freeVertices: start");
    if (vertex == NULL) return;
    freeVertices(vertex->next);
    free(vertex);
}


void freeClass(t_class *class) {
    debugPrint(DEBUG_CLASS, "freeClass: start");
    if (class == NULL) return;
    freeVertices(class->vertices);
    free(class);
    debugPrint(DEBUG_CLASS, "freeClass: done");
}
