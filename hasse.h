#ifndef __HASSE_H__
#define __HASSE_H__

#include "partition.h"
#include "graph.h"

/**
 * @brief Represents a directed link between two classes in a Hasse diagram.
 * @param src_id Source class ID
 * @param dest_id Destination class ID
 */
typedef struct s_link {
    int src_id;
    int dest_id;
} t_link;

/**
 * @brief Array storing the type of each class (0 = persistent, 1 = transient).
 */
typedef int* t_class_type_array;

/**
 * @brief Array mapping each vertex to its corresponding class ID.
 */
typedef int* t_association_array;

/**
 * @brief Dynamic array of links between classes.
 * @param links Array of links
 * @param partition Pointer to the partition containing all classes
 * @param association_array Maps vertices to class IDs
 * @param logical_size Current number of links
 * @param physical_size Maximum capacity before needing reallocation
 */
typedef struct s_link_array {
    t_link* links;
    t_partition *partition;
    t_association_array association_array;
    int logical_size;
    int physical_size;
} t_link_array;

/**
 * @brief Alias for link array representing a Hasse diagram.
 */
typedef t_link_array t_hasse_diagram;

/**
 * @brief Removes transitive redundant links from the link array.
 *
 * Eliminates links that can be inferred through transitivity.
 * For example, if A->B and B->C exist, then A->C is redundant.
 *
 * @param p_link_array Pointer to the link array to clean.
 */
void removeTransitiveLinks(t_link_array* p_link_array);

/**
 * @brief Checks if a link already exists between two classes.
 *
 * @param link_array Link array to search in.
 * @param src Source class ID.
 * @param dest Destination class ID.
 * @return 1 if the link exists, 0 otherwise.
 */
int linkExists(t_link_array link_array, int src, int dest);

/**
 * @brief Adds a new link between two classes.
 *
 * Automatically resizes the array if needed. Does not add duplicate links.
 *
 * @param link_array Pointer to the link array.
 * @param src Source class ID.
 * @param dest Destination class ID.
 * @return 1 on success, 0 if link already exists, -1 on allocation failure.
 */
int addLink(t_link_array *link_array, int src, int dest);

/**
 * @brief Displays the Hasse diagram with all links between classes.
 *
 * @param hasse The Hasse diagram to display.
 */
void displayHasseDiagram(t_hasse_diagram hasse);

/**
 * @brief Creates an array indicating the type of each class.
 *
 * @param link_array Link array containing class connections.
 * @return Array where index represents class ID and value is 0 (persistent) or 1 (transient).
 *         Caller must free with freeClassTypeArray().
 */
t_class_type_array createClassTypeArray(t_link_array link_array);

/**
 * @brief Frees memory allocated for a class type array.
 *
 * @param type_array The class type array to free.
 */
void freeClassTypeArray(t_class_type_array type_array);

/**
 * @brief Creates an association array mapping vertices to their class IDs.
 *
 * @param graph The graph structure.
 * @param partition The partition containing classes.
 * @return Array where array[vertex_index] = class_id.
 *         Caller must free the returned array.
 */
t_association_array createAssociationArray(t_graph graph, t_partition partition);

/**
 * @brief Checks if a class is persistent.
 *
 * A class is persistent if it has no outgoing links to other classes.
 *
 * @param hasse The Hasse diagram.
 * @param class_id ID of the class to check.
 * @return 1 if persistent, 0 if transient.
 */
int isPersistantClass(t_hasse_diagram hasse, int class_id);

/**
 * @brief Checks if a state is absorbing.
 *
 * A state is absorbing if:
 * 1. It forms a class with one vertex
 * 2. Its class is persistent (no outgoing links)
 *
 * @param hasse The Hasse diagram.
 * @param state_id ID of the state to check (1-indexed).
 * @param graph_size Total number of vertices in the graph.
 * @return 1 if absorbing, 0 otherwise.
 */
int isAbsorbingState(t_hasse_diagram hasse, int state_id, int graph_size);

/**
 * @brief Checks if the graph is irreducible.
 *
 * A graph is irreducible if it contains only one strongly connected component.
 *
 * @param hasse The Hasse diagram.
 * @return 1 if irreducible, 0 otherwise.
 */
int isIrreductible(t_hasse_diagram hasse);

/**
 * @brief Creates a complete Hasse diagram from a graph.
 *
 * This function:
 * 1. Computes strongly connected components using Tarjan's algorithm
 * 2. Creates an association array mapping vertices to classes
 * 3. Builds links between different classes
 * 4. Removes transitive redundancies
 *
 * @param g The graph to analyze.
 * @return Complete Hasse diagram structure.
 */
t_hasse_diagram createHasseDiagram(t_graph g);

/**
 * @brief Displays comprehensive characteristics of the graph.
 *
 * Shows:
 * - Number of transient and persistent classes
 * - List of absorbing states
 * - Detailed information for each class
 * - Graph irreducibility
 *
 * @param hasse The Hasse diagram.
 * @param graph_size Total number of vertices in the graph.
 */
void displayDetailedCharacteristics(t_hasse_diagram hasse, int graph_size);

#endif