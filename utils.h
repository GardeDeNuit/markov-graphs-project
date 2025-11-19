#ifndef __UTILS_H__
#define __UTILS_H__

// Vérifie qu'un entier est dans l'intervalle [min,max]. Si erreur et errMsg non NULL, affiche errMsg.
int validateIntRange(int value, int min, int max, const char *errMsg);

// Utilitaire existant : obtient un identifiant (A,B,...)
char *getID(int i);

typedef struct s_stack_cell{
    int value;
    struct s_cell *next;
}t_stack_cell;

typedef struct {
    t_stack_cell *top;
}t_stack;

void init_Stack(t_stack *stack);
void create_Stack(t_stack *stack);
int is_Stack_Empty(t_stack *stack);
void push_stack(t_stack *stack, int value);
int pop_stack(t_stack *stack);
void free_Stack(t_stack *stack);

#endif