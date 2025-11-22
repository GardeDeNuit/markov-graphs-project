#ifndef CLASS_H
#define CLASS_H

#include <stdlib.h>

#define DEBUG_CLASS 0

#define CLASS_NAME_MAX_SIZE 32
#define CLASS_NAME_DEFAULT "CLASS"

typedef struct s_vertex {
    int value;
    struct s_vertex *next;
} t_vertex;

typedef struct s_class {
    int id;
    t_vertex *vertices;
    struct s_class *next;
} t_class;

t_class* createClass(int class_id);
t_vertex* createVertex(int value);
void addVertexToClass(t_class *class, int value);
void displayVertices(t_vertex *vertex);
void displayClass(t_class *class);
void freeVertices(t_vertex *vertex);
void freeClass(t_class *class);

#endif //CLASS_H
