#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

char *getID(int i)
{
    // translate from 1,2,3, .. ,500+ to A,B,C,..,Z,AA,AB,...
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // Adjust to 0-based index
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    // Reverse the string to get the correct order
    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}

int validateIntRange(int value, int min, int max, const char *errMsg) {
    if (value < min || value > max) {
        if (errMsg) fprintf(stderr, "%s\n", errMsg);
        return 0;
    }
    return 1;
}


t_stack * createStack() {
    t_stack * stack = (t_stack *)malloc(sizeof(t_stack));
    stack->top = NULL;
    return stack;
}

int isStackEmpty(t_stack *stack) {
    return stack->top == NULL;
}

void pushStack(t_stack *stack, int value) {
    t_stack_cell *newCell = (t_stack_cell *)malloc(sizeof(t_stack_cell));
    newCell->value = value;
    newCell->next = stack->top;
    stack->top = newCell;
}

int popStack(t_stack *stack) {
    t_stack_cell *topCell = stack->top;
    int value = topCell->value;
    stack->top = topCell->next;
    return value;
}

void freeStackCells(t_stack_cell *cell) {
    if (cell == NULL) return;
    freeStackCells(cell->next);
    free(cell);
}
void freeStack(t_stack *stack) {
    freeStackCells(stack->top);
    free(stack);
}

int minInt(int a, int b) {
    return (a < b) ? a : b;
}

int maxInt(int a, int b) {
    return (a > b) ? a : b;
}