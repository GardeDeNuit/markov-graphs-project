#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hasse.h"
#include "tarjan.h"

void removeTransitiveLinks(t_link_array* p_link_array)
{
    int i = 0;
    while (i < p_link_array->logical_size)
    {
        t_link link1 = p_link_array->links[i];
        int j = 0;
        int to_remove = 0;

        // Check if link1 can be inferred through other links
        while (j < p_link_array->logical_size && !to_remove)
        {
            if (j != i)
            {
                t_link link2 = p_link_array->links[j];

                // If link2 starts from the same source as link1
                if (link1.src_id == link2.src_id)
                {
                    int k = 0;
                    // Look for a third link that creates a transitive path
                    while (k < p_link_array->logical_size && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = p_link_array->links[k];

                            // If we find: link2.src -> link2.dest -> link1.dest
                            // Then link1 (src -> dest) is redundant
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
            // Remove by replacing with last element
            p_link_array->links[i] = p_link_array->links[p_link_array->logical_size - 1];
            p_link_array->logical_size--;
        }
        else
        {
            i++;
        }
    }
}

int linkExists(t_link_array link_array, int src, int dest)
{
    for (int i = 0; i < link_array.logical_size; i++) {
        if (link_array.links[i].src_id == src && link_array.links[i].dest_id == dest) {
            return 1;
        }
    }
    return 0;
}

int addLink(t_link_array *link_array, int src, int dest) {
    // Don't add duplicate links
    if (linkExists(*link_array, src, dest)) {
        return 0;
    }

    // Resize array if capacity is reached
    if (link_array->logical_size >= link_array->physical_size) {
        link_array->physical_size *= 2;
        t_link* new_links = realloc(link_array->links, link_array->physical_size * sizeof(t_link));
        if (new_links == NULL) {
            perror("addLink: realloc failed");
            return -1;
        }
        link_array->links = new_links;
    }

    // Add the new link
    link_array->links[link_array->logical_size].src_id = src;
    link_array->links[link_array->logical_size].dest_id = dest;
    link_array->logical_size++;

    return 1;
}

void displayHasseDiagram(t_hasse_diagram hasse) {
    if (hasse.logical_size == 0) {
        printf("No links between classes.\n");
        return;
    }

    printf("Links between classes:\n");
    for (int i = 0; i < hasse.logical_size; i++) {
        int from = hasse.links[i].src_id;
        int to = hasse.links[i].dest_id;
        t_class *from_class = NULL;
        t_class *to_class = NULL;

        // Find the source and destination classes
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

t_class_type_array createClassTypeArray(t_hasse_diagram hasse) {
    // Allocate array for all classes
    t_class_type_array type_array = calloc(hasse.logical_size, sizeof(int));
    if (!type_array) {
        perror("createClassTypeArray: allocation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize all classes as persistent (0)
    for (int i = 0; i < hasse.logical_size; i++) {
        type_array[i] = 0;

        // Check if this class has outgoing links to other classes
        for (int j = 0; j < hasse.logical_size; j++) {
            if (hasse.links[j].src_id == i &&
                hasse.links[j].dest_id != i) {
                // Found an outgoing link to a different class
                type_array[i] = 1; // Mark as transient
                break;
            }
        }
    }

    return type_array;
}

void freeClassTypeArray(t_class_type_array type_array) {
    free(type_array);
}

int isPersistantClass(t_hasse_diagram hasse, int class_id) {
    // Create type array to determine class type
    t_class_type_array type_array = createClassTypeArray(hasse);
    int persistante = (type_array[class_id] == 0);
    free(type_array);
    return persistante;
}

int isAbsorbingState(t_hasse_diagram hasse, int state_id, int graph_size) {
    // Get the class ID of this state
    int class_id = hasse.association_array[state_id - 1];

    // If the class is transient, the state cannot be absorbing
    if (!isPersistantClass(hasse, class_id))
        return 0;

    // Count how many states belong to this class
    int count = 0;
    for (int i = 0; i < graph_size; i++) {
        if (hasse.association_array[i] == class_id) {
            count++;
            if (count > 1)
                return 0; // More than one state means not absorbing
        }
    }

    // Single state in a persistent class = absorbing state
    return 1;
}

int isIrreductible(t_hasse_diagram hasse) {
    // A graph is irreducible if it has only one strongly connected component
    return hasse.partition->class_number == 1;
}

t_association_array createAssociationArray(t_graph graph, t_partition partition) {
    // Allocate array mapping each vertex to its class ID
    t_association_array array = calloc(graph.size, sizeof(int));

    // Traverse all classes and their vertices
    t_class *curr_class = partition.classes;
    while (curr_class != NULL) {
        t_vertex *curr_vertex = curr_class->vertices;
        while (curr_vertex != NULL) {
            int vertex_id = curr_vertex->value;
            // Map vertex to class ID (convert to 0-indexed)
            array[vertex_id - 1] = curr_class->id;
            curr_vertex = curr_vertex->next;
        }
        curr_class = curr_class->next;
    }

    return array;
}

t_hasse_diagram createHasseDiagram(t_graph g){
    printf("=== Hasse Diagram Creation ===\n\n");

    // Step 1: Compute strongly connected components
    t_partition* partition = tarjan(g);
    displayPartition(partition);

    // Step 2: Create vertex-to-class mapping
    t_association_array association_array = createAssociationArray(g, *partition);

    // Step 3: Initialize Hasse diagram structure
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

    // Step 4: Build links between different classes
    for (int i = 0; i < g.size; i++) {
        int ci = association_array[i];  // Class of current vertex
        t_cell *cur = g.values[i].head;

        // Examine all neighbors
        while (cur != NULL) {
            int v = cur->vertex - 1;
            int cj = association_array[v];  // Class of neighbor

            // Add link only if classes are different
            if (ci != cj) {
                addLink(&hasse, ci, cj);
            }
            cur = cur->next;
        }
    }

    // Step 5: Remove transitive redundancies
    printf("Before transitive reduction: %d links\n", hasse.logical_size);
    removeTransitiveLinks(&hasse);
    printf("After transitive reduction: %d links\n", hasse.logical_size);
    printf("=== Hasse Diagram Complete ===\n\n");

    return hasse;
}

void displayDetailedCharacteristics(t_hasse_diagram hasse, int graph_size) {
    printf("\n=== Displaying Graph Detailed Characteristics ===\n");

    t_partition *part = hasse.partition;
    int nb_classes = part->class_number;

    // Count transient and persistent classes
    int transient_count = 0, persistent_count = 0;
    for (int i = 0; i < nb_classes; i++) {
        if (isPersistantClass(hasse, i))
            persistent_count++;
        else
            transient_count++;
    }

    // Detect all absorbing states
    int absorbing_states[graph_size];
    int absorbing_count = 0;

    for (int s = 0; s < graph_size; s++) {
        if (isAbsorbingState(hasse, s + 1, graph_size)) {
            absorbing_states[absorbing_count++] = s + 1;
        }
    }

    // Print summary statistics
    printf("   Summary:\n");
    printf("     - Transient classes   : %d\n", transient_count);
    printf("     - Persistent classes  : %d\n", persistent_count);
    printf("     - Absorbing states    : %d", absorbing_count);

    if (absorbing_count > 0) {
        printf(" (");
        for (int i = 0; i < absorbing_count; i++) {
            printf("%d", absorbing_states[i]);
            if (i < absorbing_count - 1) printf(", ");
        }
        printf(")");
    }
    printf("\n\n");

    // Display detailed information for each class
    printf("   Detailed class information:\n");

    t_class *classe = part->classes;
    while (classe != NULL) {
        int id = classe->id;

        // Print vertices in this class
        printf("  Class C%d : {", id);
        t_vertex *v = classe->vertices;
        while (v != NULL) {
            printf("%d", v->value);
            if (v->next != NULL) printf(", ");
            v = v->next;
        }
        printf("}\n");

        // Print class type
        printf("     Type : %s\n", isPersistantClass(hasse, id) ? "Persistent" : "Transient");

        // Check for absorbing states in singleton classes
        if (classe->vertices != NULL && classe->vertices->next == NULL) {
            int state_id = classe->vertices->value;
            if (isAbsorbingState(hasse, state_id, graph_size))
                printf("     Absorbing state : YES (State %d)\n", state_id);
            else
                printf("     Absorbing state : NO\n");
        }

        classe = classe->next;
    }

    // Display graph irreducibility
    printf("\n   Graph is %sirreducible\n",
           isIrreductible(hasse) ? "" : "NOT ");

    printf("=== End of Detailed Characteristics ===\n");
}