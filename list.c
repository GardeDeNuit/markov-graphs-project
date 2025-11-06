#include "list.h"

t_list createEmptyList(){
    t_list new_list;
    new_list.head = NULL;
    return new_list;
}

void addCell(t_list *ptr_list, int vertex, double weight)
{
    if (ptr_list == NULL) {
        fprintf(stderr, "addCell: ptr_list is NULL\n");
        return;
    }

    t_cell *cell = createCell(vertex, weight);
    if (cell == NULL) {
        // createCell already printed an error
        return;
    }

    cell->next = ptr_list->head;
    ptr_list->head= cell;
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

void freeList(t_list* list) {
    if (list == NULL) return;
    t_cell* curr = list->head;
    while (curr != NULL) {
        t_cell* next = curr->next;
        freeCell(curr);
        curr = next;
    }
    list->head = NULL;
}

double sumValues(t_list list){
    t_cell* curr = list.head;
    double sum = 0.00;
    while (curr != NULL){
        sum += curr->weight;
        curr = curr->next;
    }
    return sum;
}