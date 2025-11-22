#include "cell.h"
#include <stdio.h>
#include <stdlib.h>

t_cell * createCell(int vertex, double weight) {
    t_cell *cell;
    cell = (t_cell *)malloc(sizeof(t_cell));
    if (cell == NULL) {
        fprintf(stderr, "createCell: memory allocation failed\n");
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

int freeCell(t_cell* cell) {
    if (cell == NULL) {
        fprintf(stderr, "freeCell: cell pointer is NULL\n");
        return -1;
    }
    free(cell);
    return 1;
}
