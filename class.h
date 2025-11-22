#ifndef CLASS_H
#define CLASS_H

/**
 * @file class.h
 * @brief Structures and operations for managing classes of vertices
 */

/**
 * @struct s_vertex
 * @brief Represents a vertex in a linked list
 * @param vertex The vertex identifier/value
 * @param next Pointer to the next vertex in the list
 */
typedef struct s_vertex {
    int value;
    struct s_vertex *next;
} t_vertex;

/**
 * @struct s_class
 * @brief Represents a class containing a list of vertices
 * @param id The class identifier
 * @param vertices Pointer to the first vertex in the class
 * @param next Pointer to the next class in the list
 */
typedef struct s_class {
    int id;
    t_vertex *vertices;
    struct s_class *next;
} t_class;

/**
 * @brief Creates a new class with the specified identifier
 * @param class_id The identifier for the new class
 * @return Pointer to the newly created class, or NULL on allocation failure
 */
t_class* createClass(int class_id);

/**
 * @brief Creates a new vertex with the specified value
 * @param value The value for the new vertex
 * @return Pointer to the newly created vertex, or NULL on allocation failure
 */
t_vertex* createVertex(int value);

/**
 * @brief Adds a vertex to the beginning of a class's vertex list
 * @param class Pointer to the class
 * @param value The value of the vertex to add
 * @return 1 on success, -1 on failure
 */
int addVertexToClass(t_class *class, int value);

/**
 * @brief Displays a class and all its vertices
 * @param class Pointer to the class to display
 */
void displayClass(t_class *class);

/**
 * @brief Frees all vertices in a linked list recursively
 * @param vertex Pointer to the first vertex to free
 */
void freeVertices(t_vertex *vertex);

/**
 * @brief Frees all memory allocated for a class and its vertices
 * @param class Pointer to the class to free
 * @return 1 on success, -1 if class is NULL
 */
int freeClass(t_class *class);

#endif //CLASS_H
