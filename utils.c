#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

char *getID(int i)
{
    char temp[16];
    int index = 0;

    // Si i est invalide, retourner NULL
    if (i <= 0) return NULL;
    i--; /* passage à un index 0-based */

    // Conversion en base 26 (A-Z)
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
        if (index >= (int)(sizeof(temp) - 1)) break; /* sécurité contre débordement */
    }

    /* allouer la taille exacte (+1 pour le '\0') */
    char *res = (char *)malloc(index + 1);
    if (!res) return NULL;

    /* écrire les caractères dans l'ordre correct (inversion) */
    for (int j = 0; j < index; j++)
        res[j] = temp[index - j - 1];
    res[index] = '\0';

    return res; /* l'appelant doit libérer la mémoire avec free() */
}

int validateIntRange(int value, int min, int max, const char *errMsg) {
    // Vérifier si la valeur est hors des limites
    if (value < min || value > max) {
        if (errMsg) fprintf(stderr, "%s\n", errMsg);
        return 0;
    }
    return 1;
}

t_stack * createStack() {
    // Allouer la mémoire pour la pile
    t_stack * stack = (t_stack *)malloc(sizeof(t_stack));
    stack->top = NULL;
    return stack;
}

int isStackEmpty(t_stack *stack) {
    // La pile est vide si le sommet est NULL
    return stack->top == NULL;
}

void pushStack(t_stack *stack, int value) {
    // Créer une nouvelle cellule
    t_stack_cell *newCell = (t_stack_cell *)malloc(sizeof(t_stack_cell));
    newCell->value = value;
    // Insérer au sommet de la pile
    newCell->next = stack->top;
    stack->top = newCell;
}

int popStack(t_stack *stack) {
    // Vérifier si la pile est vide
    if (isStackEmpty(stack)) {
        return STACK_EMPTY;
    }
    // Récupérer la cellule du sommet
    t_stack_cell *topCell = stack->top;
    int value = topCell->value;
    // Mettre à jour le sommet
    stack->top = topCell->next;
    free(topCell);
    return value;
}

void freeStackCells(t_stack_cell *cell) {
    // Cas de base : cellule NULL
    if (cell == NULL) return;
    // Récursion pour libérer les cellules suivantes
    freeStackCells(cell->next);
    free(cell);
}

void freeStack(t_stack *stack) {
    // Libérer toutes les cellules
    freeStackCells(stack->top);
    // Libérer la structure de la pile
    free(stack);
}

int minInt(int a, int b) {
    // Retourner le plus petit des deux
    return (a < b) ? a : b;
}

int maxInt(int a, int b) {
    // Retourner le plus grand des deux
    return (a > b) ? a : b;
}

char *intToStr(int value) {
    // Buffer statique pour la conversion
    static char buffer[20];
    snprintf(buffer, sizeof(buffer), "%d", value);
    return buffer;
}

void debugPrint(int isDebugMode, const char *msg) {
    // N'afficher que si le mode debug est activé
    if (!isDebugMode) return;
    printf("[DEBUG] %s\n", msg);
}
