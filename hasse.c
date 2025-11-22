#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "hasse.h"

/**
 * @brief Make a Hasse Diagram
 */



/* private functions =================================================== */

/**
 * @brief Remove redundant transitive links from a link array.
 * @param p_link_array Pointer to the link array to be cleaned.
 * @return Nothing.
 */

static void removeTransitiveLinks(t_link_array *p_link_array)
{
    int i = 0;
    while (i < p_link_array->log_size)
    {
        t_link link1 = p_link_array->links[i];
        int j = 0;
        int to_remove = 0;
        while (j < p_link_array->log_size && !to_remove)
        {
            if (j != i)
            {
                t_link link2 = p_link_array->links[j];
                if (link1.src_nb == link2.src_nb)
                {
                    int k = 0;
                    while (k < p_link_array->log_size && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = p_link_array->links[k];
                            if ((link3.src_nb == link2.dest_nb) && (link3.dest_nb == link1.dest_nb))
                            {
                                to_remove = 1;
                            }
                        }
                        k++;
                    }
                }
            }
            j++;
        }
        if (to_remove)
        {
            p_link_array->links[i] = p_link_array->links[p_link_array->log_size - 1];
            p_link_array->log_size--;
        }
        else
        {
            i++;
        }
    }
}

/**
 * @brief Check if a link already exists between two nodes.
 * @param link_array Link array to search in.
 * @param dept Source node.
 * @param dest Destination node.
 * @return 1 if the link exists, 0 otherwise.
 */

static int linkExists(t_link_array *link_array, int dept, int dest)
{
    for (int i = 0; i < link_array->log_size; i++)
    {
        if (link_array->links[i].src_nb == dept && link_array->links[i].dest_nb == dest)
        {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Add a link between two nodes in the link array.
 * @param link_array Link array to update.
 * @param dept Source node.
 * @param dest Destination node.
 * @return Nothing.
 */

static void addLink(t_link_array *link_array, int dept, int dest)
{

    if (linkExists(link_array, dept, dest))
        return;

    if (link_array->log_size >= link_array->max_size)
    {
        link_array->max_size *= 2;
        link_array->links = realloc(link_array->links, link_array->max_size * sizeof(t_link));
    }

    link_array->links[link_array->log_size].src_nb = dept;
    link_array->links[link_array->log_size].dest_nb = dest;
    link_array->log_size++;
}

/**
 * @brief Build an array of strings representing class names.
 * @param partition Partition containing the classes.
 * @param num_classes Number of classes.
 * @return Array of strings representing class names.
 */

static char** buildName(t_partition *partition, int num_classes)
{
    char **class_names = malloc(num_classes * sizeof(char*));

    int class_num = 0;
    t_class *curr_class = partition->classes;

    while (curr_class != NULL && class_num < num_classes)
    {
        class_names[class_num] = malloc(64 * sizeof(char));
        memset(class_names[class_num], 0, 64);

        strcpy(class_names[class_num], "{");

        t_vertex *curr_vertex = curr_class->vertices;
        int first = 1;
        while (curr_vertex != NULL)
        {
            if (!first) strcat(class_names[class_num], ",");

            char vertex_str[16];
            snprintf(vertex_str, sizeof(vertex_str), "%d", curr_vertex->value);
            strcat(class_names[class_num], vertex_str);

            first = 0;
            curr_vertex = curr_vertex->next;
        }

        strcat(class_names[class_num], "}");

        class_num++;
        curr_class = curr_class->next;
    }

    return class_names;
}

/**
 * @brief Display the Hasse diagram after removing redundant links.
 * @param links Link array between classes.
 * @param partition Partition containing the classes.
 * @param class_array Array of class indices.
 * @param num_classes Number of classes.
 * @return Nothing.
 */

static void displayHasse(t_link_array *links, t_partition *partition, int *class_array, int num_classes)
{
    printf("\n=== Diagramme de Hasse (après suppression des redondances) ===\n");

    if (links->log_size == 0) {
        printf("Aucun lien entre les classes.\n");
        return;
    }

    char **class_names = buildName(partition, num_classes);

    printf("Liens entre classes:\n");
    for (int i = 0; i < links->log_size; i++)
    {
        int from = links->links[i].src_nb;
        int to = links->links[i].dest_nb;
        printf("%s -> %s\n", class_names[from], class_names[to]);
    }

    for (int i = 0; i < num_classes; i++) {
        free(class_names[i]);
    }
    free(class_names);
}

/**
 * @brief Determine the type of each class (persistent or transient).
 * @param class_array Array of class indices.
 * @param num_vertices Number of vertices.
 * @param class_links Link array between classes.
 * @return Array of class types (0 = persistent, 1 = transient).
 */

static int* ClassType(int* class_array,int num_vertices,t_link_array class_links) {
    int *type_array = malloc(num_vertices * sizeof(int));

    for (int i = 0; i < num_vertices; i++) {
        type_array[i] = 0;
    }

    for (int i = 0; i < class_links.log_size; i++) {
        int dept = class_links.links[i].src_nb;
        int dest = class_links.links[i].dest_nb;

        if (dept != dest) {
            type_array[dest] = 1;
        }

    }
    return type_array;
}

/**
 * @brief Check if a class is absorbing.
 * @param class_sizes Array of class sizes.
 * @param class_nb Index of the class to check.
 * @param type_array Array of class types.
 * @return 1 if absorbing, 0 otherwise.
 */

static int isAbsorbingState(int* class_sizes,int class_nb,int* type_array) {

    for (int i = 0; i < class_sizes[class_nb]; i++) {
        if (type_array[i] != 0) {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Check if the graph is irreducible.
 * @param class_array Array of class indices.
 * @param num_vertices Number of vertices.
 * @return 1 if irreducible, 0 otherwise.
 */

static int isIrreductible(int* class_array,int num_vertices) {

    int first_class = class_array[0];
    for (int i = 1; i < num_vertices; i++) {
        if (class_array[i] != first_class) {
            return 0;
        }
    }
    return 1;
}

/* public functions =================================================== */


int* makeClassArray(t_graph *graph, t_partition *partition)
{
    int *class_array = malloc(graph->size * sizeof(int));
    memset(class_array, -1, graph->size * sizeof(int));

    int class_num = 0;
    t_class *curr_class = partition->classes;

    while (curr_class != NULL)
    {

        t_vertex *curr_vertex = curr_class->vertices;
        while (curr_vertex != NULL)
        {
            int vertex_id = curr_vertex->value;

            class_array[vertex_id - 1] = class_num;
            curr_vertex = curr_vertex->next;
        }

        class_num++;
        curr_class = curr_class->next;
    }

    return class_array;
}