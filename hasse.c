#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hasse.h"
#include "tarjan.h"

/**
 * @brief Make a Hasse Diagram
 */

/* private functions =================================================== */

/**
 * @brief Remove redundant transitive links from a link array.
 * @param p_link_array Pointer to the link array to be cleaned.
 * @return Nothing.
 */

void removeTransitiveLinks(t_link_array* p_link_array)
{
    int i = 0;
    while (i < p_link_array->logical_size)
    {
        t_link link1 = p_link_array->links[i];
        int j = 0;
        int to_remove = 0;
        while (j < p_link_array->logical_size && !to_remove)
        {
            if (j != i)
            {
                t_link link2 = p_link_array->links[j];
                if (link1.src_id == link2.src_id)
                {
                    int k = 0;
                    while (k < p_link_array->logical_size && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = p_link_array->links[k];
                            if ((link3.src_id == link2.dest_id) && (link3.dest_id == link1.dest_id))
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
            p_link_array->links[i] = p_link_array->links[p_link_array->logical_size - 1];
            p_link_array->logical_size--;
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
 * @param src Source node.
 * @param dest Destination node.
 * @return 1 if the link exists, 0 otherwise.
 */
int linkExists(t_link_array link_array, int src, int dest)
{
    for (int i = 0; i < link_array.logical_size; i++) {
        if (link_array.links[i].src_id == src && link_array.links[i].dest_id == dest) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Add a link between two nodes in the link array.
 * @param link_array Link array to update.
 * @param src Source node.
 * @param dest Destination node.
 * @return Nothing.
 */
int addLink(t_link_array *link_array, int src, int dest) {
    if (linkExists(*link_array, src, dest)) {
        printf("Link already exists: %d -> %d\n", src, dest);
        return 0;
    }

    if (link_array->logical_size >= link_array->physical_size) {
        // Augmenter la taille physique du tableau
        link_array->physical_size *= 2;
        t_link* new_links = realloc(link_array->links, link_array->physical_size * sizeof(t_link));
        if (new_links == NULL) {
            perror("addLink: realloc failed");
            return -1;
        }
        link_array->links = new_links;
        printf("Link array resized to %d\n", link_array->physical_size);
    }

    link_array->links[link_array->logical_size].src_id = src;
    link_array->links[link_array->logical_size].dest_id = dest;
    link_array->logical_size++;

    printf("New link added: %d -> %d (total links: %d)\n", src, dest, link_array->logical_size);
    return 1;
}

/**
 * @brief Display the Hasse diagram.
 * @param hasse The Hasse diagram to display.
 * @return Nothing.
 */
void displayHasseDiagram(t_hasse_diagram hasse) {
    if (hasse.logical_size == 0) {
        printf("Aucun lien entre les classes.\n");
        return;
    }

    printf("Liens entre classes :\n");
    for (int i = 0; i < hasse.logical_size; i++) {
        int from = hasse.links[i].src_id;
        int to = hasse.links[i].dest_id;
        t_class *from_class = NULL;
        t_class *to_class = NULL;

        // On parcourt les classes pour pouvoir les récupérer et les afficher
        t_class* curr_class = hasse.partition->classes;
        while (curr_class != NULL) {
            if (from_class == NULL && curr_class->id == from) {
                from_class = curr_class;
            }
            if (to_class == NULL && curr_class->id == to) {
                to_class = curr_class;
            }
            if (from_class != NULL && to_class != NULL) {
                break;
            }
            curr_class = curr_class->next;
        }

        displayClass(from_class);
        printf(" --> ");
        displayClass(to_class);
        printf("\n");
    }
}

/**
 * @brief Determine the type of each class (persistent or transient).
 * @param class_array Array of class indices.
 * @param num_vertices Number of vertices.
 * @param class_links Link array between classes.
 * @return Array of class types (0 = persistent, 1 = transient).
 */

t_class_type_array createClassTypeArray(t_link_array link_array) {
    t_class_type_array type_array = calloc(link_array.logical_size, sizeof(int));

    for (int i = 0; i < link_array.logical_size; i++) {
        int dept = link_array.links[i].src_id;
        int dest = link_array.links[i].dest_id;

        if (dept != dest) {
            type_array[dest] = 1;
        }

    }
    return type_array;
}

void freeClassTypeArray(t_class_type_array type_array) {
    free(type_array);
}

int isPersistantClass(t_hasse_diagram hasse, int class_id) {
    t_class_type_array type_array = createClassTypeArray(hasse);
    int is_persistant = type_array[class_id];
    freeClassTypeArray(type_array);
    return is_persistant;
}

/**
 * @brief Check if a class is absorbing.
 * @param class_sizes Array of class sizes.
 * @param class_nb Index of the class to check.
 * @param type_array Array of class types.
 * @return 1 if absorbing, 0 otherwise.
 */
int isAbsorbingState(t_hasse_diagram hasse, int state_id, int graph_size) {
    int state_class = hasse.association_array[state_id - 1];
    if (isPersistantClass(hasse, state_class)) {
        int class_occurrence = 0;
        for (int i = 0; i < graph_size; i++) {
            if (hasse.association_array[i] == state_class) {
                class_occurrence++;
            }
            if (class_occurrence > 1) {
                return 0;
            }
        }
    }
    return 0;
}

/**
 * @brief Check if the graph is irreducible.
 * @param type_array Array of class indices.
 * @param link_array .
 * @return 1 if irreducible, 0 otherwise.
 */

int isIrreductible(t_hasse_diagram hasse){
    return hasse.logical_size == 1;
}

t_association_array createAssociationArray(t_graph graph, t_partition partition) {
    t_association_array array = calloc(graph.size, sizeof(int));
    t_class *curr_class = partition.classes;
    while (curr_class != NULL) {
        t_vertex *curr_vertex = curr_class->vertices;
        while (curr_vertex != NULL) {
            int vertex_id = curr_vertex->value;
            array[vertex_id - 1] = curr_class->id;
            curr_vertex = curr_vertex->next;
        }
        curr_class = curr_class->next;
    }
    return array;
}

t_hasse_diagram createHasseDiagram(t_graph g){
    printf("=== Hasse Diagram Creation ===\n\n");
    t_partition* partition = tarjan(g);
    displayPartition(partition);
    t_association_array association_array = createAssociationArray(g, *partition);

    t_hasse_diagram hasse;
    hasse.logical_size = 0;
    hasse.physical_size = g.size;
    hasse.links = malloc(hasse.physical_size * sizeof(t_link));
    if (hasse.links == NULL) {
        fprintf(stderr, "createHasseDiagram: malloc failed for links array\n");
        hasse.physical_size = 0;
        hasse.partition = partition;
        hasse.association_array = association_array;
        return hasse;
    }
    hasse.partition = partition;
    hasse.association_array = association_array;

    printf("\n=== Building Hasse Diagram ===\n");
    printf("Graph size: %d\n", g.size);
    printf("Number of classes: %d\n", partition->class_number);

    for (int i = 0; i < g.size; i++) {
        int ci = association_array[i];
        t_cell *cur = g.values[i].head;

        while (cur != NULL) {
            int v = cur->vertex - 1;
            int cj = association_array[v];

            if (ci != cj) {
                addLink(&hasse, ci, cj);
            }
            cur = cur->next;
        }
    }

    printf("Before transitive reduction: %d links\n", hasse.logical_size);
    removeTransitiveLinks(&hasse);
    printf("After transitive reduction: %d links\n", hasse.logical_size);
    printf("=== Hasse Diagram Complete ===\n\n");

    return hasse;
}