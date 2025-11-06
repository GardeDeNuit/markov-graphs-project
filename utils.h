#ifndef __UTILS_H__
#define __UTILS_H__

// Vérifie qu'un entier est dans l'intervalle [min,max]. Si erreur et errMsg non NULL, affiche errMsg.
int validateIntRange(int value, int min, int max, const char *errMsg);

// Utilitaire existant : obtient un identifiant (A,B,...)
char *getID(int i);

#endif