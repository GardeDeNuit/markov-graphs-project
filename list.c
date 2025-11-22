#include "list.h"
#include <stdlib.h>
#include <stdio.h>

t_list createEmptyList(){
    t_list new_list;
    new_list.head = NULL;
    return new_list;
}

int addCell(t_list *list, int vertex, double weight)
{
    if (list == NULL) {
        fprintf(stderr, "addCell: list pointer is NULL\n");
        return -1;
    }

    t_cell *cell = createCell(vertex, weight);
    if (cell == NULL) {
        fprintf(stderr, "addCell: failed to create cell\n");
        return -1;
    }

    cell->next = list->head;
    list->head= cell;
    return 1;
}

void displayList(t_list list) {
    t_cell* curr;
    curr = list.head;
    printf("[head]");
    while (curr!= NULL) {
        printf("@->");
        displayCell(*curr);
        curr = curr->next;
    }
    printf("\n");
}

int freeList(t_list* list) {
    if (list == NULL) {
        fprintf(stderr, "freeList: list pointer is NULL\n");
        return -1;
    }
    t_cell* curr = list->head;
    while (curr != NULL) {
        t_cell* next = curr->next;
        freeCell(curr);
        curr = next;
    }
    list->head = NULL;
    return 1;
}

double sumListValues(t_list list){
    t_cell* curr = list.head;
    double sum = 0.00;
    while (curr != NULL){
        sum += curr->weight;
        curr = curr->next;
    }
    return sum;
}