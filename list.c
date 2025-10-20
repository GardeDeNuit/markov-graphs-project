//
// Created by matteo on 20/10/2025.
//

#include "list.h"

t_list createEmptyList(){
    t_list new_list;
    new_list.head = NULL;
    return new_list;
}

void addCell(t_list *ptr_list, int vertex, double weight)
{
    t_cell *cell = createCell(vertex, weight);
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



