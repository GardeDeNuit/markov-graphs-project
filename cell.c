//
// Created on 20/10/2025.
//

#include "cell.h"

t_cell * createCell(int vertex, double weight) {
    t_cell *cell;
    cell = (t_cell *)malloc(sizeof(t_cell));
    if (cell == NULL) {
        perror("createCell: allocation failed");
        return NULL;
    }
    cell->vertex = vertex;
    cell->weight = weight;
    cell->next = NULL;
    return cell;
}

void displayCell(t_cell cell) {
    printf("(%d, %.2f)", cell.vertex, cell.weight);
}

void freeCell(t_cell* cell) {
    if (cell == NULL) return;
    free(cell);
}
