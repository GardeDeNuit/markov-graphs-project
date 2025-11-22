#ifndef CELL_H
#define CELL_H

/**
 * @file cell.h
 * @brief Cell structure and operations for linked lists
 */

/**
 * @struct s_cell
 * @brief Represents a cell in a linked list with vertex ID and weight
 * @param vertex The vertex identifier
 * @param weight The weight associated with the vertex
 * @param next Pointer to the next cell in the list
 */
struct s_cell {
    int vertex;
    double weight;
    struct s_cell* next;
};
typedef struct s_cell t_cell;

/**
 * @brief Creates a new cell with specified vertex and weight
 * @param vertex The vertex identifier
 * @param weight The weight value
 * @return Pointer to the newly created cell, or NULL on allocation failure
 */
t_cell* createCell(int vertex, double weight);

/**
 * @brief Displays a cell's content (vertex, weight)
 * @param cell The cell to display
 */
void displayCell(t_cell cell);

/**
 * @brief Frees the memory allocated for a cell
 * @param cell Pointer to the cell to free
 * @return 1 on success, -1 if cell is NULL
 */
int freeCell(t_cell* cell);

#endif //CELL_H
