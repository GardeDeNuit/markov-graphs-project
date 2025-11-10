#ifndef CLASS_H
#define CLASS_H

#include <stdlib.h>

typedef struct s_vertex {
    int value;
    struct s_vertex *next;
} t_vertex;

typedef struct s_class {
    char name[10];
    t_vertex *vertexes;
    struct s_class *next;
} t_class;

t_class* createClass(const char *name);
void addVertexToClass(t_class *class, int value);
void displayClass(t_class *class);
void freeVertexes(t_vertex *vertex);
void freeClass(t_class *class);

#endif //CLASS_H
