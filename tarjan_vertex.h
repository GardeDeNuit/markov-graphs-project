
#ifndef TARJAN_VERTEX_H
#define TARJAN_VERTEX_H
#include <stdlib.h>

#define DEBUG_TARJAN_VERTEX 0

struct s_tarjan_vertex {
    int id;
    int num;
    int num_accessible;
    int in_pile;
    struct s_tarjan_vertex* next;
};
typedef struct s_tarjan_vertex t_tarjan_vertex;

t_tarjan_vertex* createTarjanVertex(int, int, int, int);
void displayTarjanVertex(t_tarjan_vertex);
void freeTarjanVertex(t_tarjan_vertex*);

#endif //TARJAN_VERTEX_H
