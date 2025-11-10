#ifndef CLASS_H
#define CLASS_H

#include "tarjan_vertex.h"

typedef struct s_class {
    char name[10];
    t_tarjan_vertex *vertexes;
    struct s_class *next;
} t_class;

t_class* createClass(const char *);
void addVertexToClass(t_class , t_tarjan_vertex *);
void displayClass(t_class *);
void freeClass(t_class *);

#endif //CLASS_H
