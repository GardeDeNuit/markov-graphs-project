//
// Created by matteo on 20/10/2025.
//

#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include "cell.h"

typedef struct s_list {
    t_cell* head;
}t_list;

t_list createEmptyList();
void displayList(t_list);
void addCell(t_list*, int, double);
void freeList(t_list*);

#endif //LIST_H
