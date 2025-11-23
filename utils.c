#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

char *getID(int i)
{
    char temp[16];
    int index = 0;

    if (i <= 0) return NULL;
    i--; /* passage à un index 0-based */

    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
        if (index >= (int)(sizeof(temp) - 1)) break; /* sécurité */
    }

    /* allouer la taille exacte (+1 pour le '\0') */
    char *res = (char *)malloc(index + 1);
    if (!res) return NULL;

    /* écrire dans l'ordre correct */
    for (int j = 0; j < index; j++)
        res[j] = temp[index - j - 1];
    res[index] = '\0';

    return res; /* caller doit faire free() */
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
    if (isStackEmpty(stack)) {
        return STACK_EMPTY;
    }
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

char *intToStr(int value) {
    static char buffer[20];
    snprintf(buffer, sizeof(buffer), "%d", value);
    return buffer;
}

void debugPrint(int isDebugMode, const char *msg) {
    if (!isDebugMode) return;
    printf("[DEBUG] %s\n", msg);
}
