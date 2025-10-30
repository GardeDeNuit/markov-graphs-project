//
// Created by matteo on 20/10/2025.
//

#ifndef CELL_H
#define CELL_H
#include <stdio.h>
#include <stdlib.h>

struct s_cell {
    int vertex;
    double weight;
    struct s_cell* next;
};
typedef struct s_cell t_cell;

t_cell* createCell(int, double);
void displayCell(t_cell);

// Libère une cellule (attend un pointeur sur la cellule)
void freeCell(t_cell*);

#endif //CELL_H
