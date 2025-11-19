#ifndef __UTILS_H__
#define __UTILS_H__

#define TRUE 1
#define FALSE 0

#define STACK_EMPTY (-1)
// Vérifie qu'un entier est dans l'intervalle [min,max]. Si erreur et errMsg non NULL, affiche errMsg.
int validateIntRange(int value, int min, int max, const char *errMsg);

// Utilitaire existant : obtient un identifiant (A,B,...)
char *getID(int i);

typedef struct s_stack_cell{
    int value;
    struct s_stack_cell *next;
}t_stack_cell;

typedef struct {
    t_stack_cell *top;
}t_stack;


t_stack * createStack(void);
int isStackEmpty(t_stack *);
void pushStack(t_stack *, int);
int popStack(t_stack *);
void FreeStackCells(t_stack_cell *);
void freeStack(t_stack *);
int minInt(int, int);
int maxInt(int, int);
char *intToStr(int);
void debugPrint(int isDebugMode, const char *msg);

#endif