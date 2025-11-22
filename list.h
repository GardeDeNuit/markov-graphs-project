#ifndef LIST_H
#define LIST_H

#include "cell.h"

/**
 * @file list.h
 * @brief Linked list structure and operations
 */

/**
 * @struct s_list
 * @brief Represents a singly linked list of cells
 * @param head Pointer to the first cell in the list
 */
typedef struct s_list {
    t_cell* head;
}t_list;

/**
 * @brief Creates an empty list
 * @return An empty list with head set to NULL
 */
t_list createEmptyList();

/**
 * @brief Displays all cells in the list
 * @param list The list to display
 */
void displayList(t_list list);

/**
 * @brief Adds a new cell at the head of the list
 * @param list Pointer to the list
 * @param vertex The vertex identifier for the new cell
 * @param weight The weight value for the new cell
 * @return 1 on success, -1 on failure (NULL list or allocation error)
 */
int addCell(t_list* list, int vertex, double weight);

/**
 * @brief Frees all cells in the list
 * @param list Pointer to the list to free
 * @return 1 on success, -1 if list is NULL
 */
int freeList(t_list* list);

/**
 * @brief Calculates the sum of all weights in the list
 * @param list The list to sum
 * @return The sum of all weight values
 */
double sumListValues(t_list list);

#endif //LIST_H
