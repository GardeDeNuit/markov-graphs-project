#include "mermaidchart-file-generator.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

/* Private helper functions ============================================ */

/**
 * @brief Appends a single vertex to the Mermaid file.
 */
static void appendVertex(const int vertex, FILE *file) {
    fprintf(file, "%s((%d))\n", getID(vertex), vertex);
}

/**
 * @brief Appends all graph vertices to the Mermaid file.
 */
static void appendGraphVertexes(const t_graph graph, FILE *file) {
    int i = 0;
    for (i = 0; i<graph.size; i++) {
        appendVertex(i+1, file);
    }
}

/**
 * @brief Appends a single edge with its weight to the Mermaid file.
 */
static void appendEdge(int src, int dest, double weight, FILE *file) {
    char srcID[64];
    char destID[64];

    // Problème avec la fonction getID : on doit copier le résultat dans des variables locales
    strncpy(srcID, getID(src), sizeof(srcID) - 1);
    srcID[sizeof(srcID) - 1] = '\0';
    strncpy(destID, getID(dest), sizeof(destID) - 1);
    destID[sizeof(destID) - 1] = '\0';

    fprintf(file, "%s -->|%.5f|%s\n", srcID, weight, destID);
}

/**
 * @brief Appends all graph edges to the Mermaid file.
 */
static void appendGraphEdges(const t_graph graph, FILE *file) {
    int i = 0;
    for (i = 0; i < graph.size; i++) {
        t_list neighbors = graph.values[i];
        t_cell *cur = neighbors.head;
        while (cur != NULL) {
            appendEdge(i+1, cur->vertex, cur->weight, file);
            cur = cur->next;
        }
    }
}

/**
 * @brief Creates a mapping array from class ID to node index (0, 1, 2, ...).
 */
static int* createClassIdToIndexMapping(t_partition *partition) {
    // Find max class ID
    int max_id = 0;
    t_class *curr = partition->classes;
    while (curr != NULL) {
        if (curr->id > max_id) {
            max_id = curr->id;
        }
        curr = curr->next;
    }

    // Create mapping array
    int *mapping = malloc((max_id + 1) * sizeof(int));
    if (mapping == NULL) {
        fprintf(stderr, "createClassIdToIndexMapping: allocation failed\n");
        return NULL;
    }

    // Initialize to -1
    for (int i = 0; i <= max_id; i++) {
        mapping[i] = -1;
    }

    // Fill mapping
    curr = partition->classes;
    int index = 0;
    while (curr != NULL) {
        mapping[curr->id] = index;
        index++;
        curr = curr->next;
    }

    return mapping;
}

/**
 * @brief Writes all class nodes to the Mermaid file.
 */
static void writeNodes(FILE *file, char **class_labels, int num_classes) {
    for (int i = 0; i < num_classes; i++)
    {
        char node_id = 'A' + i;
        fprintf(file, "%c[\"%s\"]\n", node_id, class_labels[i]);
    }
    fprintf(file, "\n");
}

/**
 * @brief Writes all links between classes to the Mermaid file.
 */
static void writeEdges(FILE *file, t_hasse_diagram *hasse) {
    // Create mapping from class ID to index
    int *id_to_index = createClassIdToIndexMapping(hasse->partition);
    if (id_to_index == NULL) {
        fprintf(stderr, "writeEdges: failed to create mapping\n");
        return;
    }

    printf("Writing %d edges to Mermaid file\n", hasse->logical_size);

    for (int i = 0; i < hasse->logical_size; i++) {
        int from_id = hasse->links[i].src_id;
        int to_id = hasse->links[i].dest_id;

        // Convert class IDs to node indices
        int from_index = id_to_index[from_id];
        int to_index = id_to_index[to_id];

        if (from_index < 0 || to_index < 0) {
            fprintf(stderr, "writeEdges: invalid class ID mapping (from_id=%d, to_id=%d)\n", from_id, to_id);
            continue;
        }

        char from_node = 'A' + from_index;
        char to_node = 'A' + to_index;

        printf("  Writing edge: %c --> %c (class %d -> class %d)\n", from_node, to_node, from_id, to_id);
        fprintf(file, "%c --> %c\n", from_node, to_node);
    }

    free(id_to_index);
}

static char** buildClassLabels(t_partition partition) {
    char **class_labels = malloc(partition.class_number * sizeof(char*));
    if (class_labels == NULL) {
        fprintf(stderr, "buildClassLabels: allocation failed\n");
        return NULL;
    }

    t_class *curr_class = partition.classes;

    for (int i = 0; i < partition.class_number; i++, curr_class = curr_class->next)
    {
        class_labels[i] = malloc(256);
        if (class_labels[i] == NULL) {
            fprintf(stderr, "buildClassLabels: allocation failed for label %d\n", i);
            return NULL;
        }

        int pos = sprintf(class_labels[i], "{");
        t_vertex *curr_vertex = curr_class->vertices;

        while (curr_vertex!= NULL) {
            pos += sprintf(class_labels[i] + pos, "%s%d", (curr_vertex== curr_class->vertices) ? "" : ",", curr_vertex->value);
            curr_vertex= curr_vertex->next;
        }

        sprintf(class_labels[i] + pos, "}");
    }

    return class_labels;
}

static void freeClassLabels(char **class_labels, int num_classes)
{
    if (class_labels == NULL) return;

    for (int i = 0; i < num_classes; i++)
    {
        if (class_labels[i] != NULL) {
            free(class_labels[i]);
        }
    }
    free(class_labels);
}

/* Public functions ==================================================== */

int exportGraphToMermaidFile(t_graph graph, const char* path) {
    printf("\n=== Exporting Graph to Mermaid ===\n");
    printf("Path: %s\n", path);
    printf("Number of vertices: %d\n", graph.size);

    FILE *file = fopen(path, "w");

    if (file == NULL) {
        fprintf(stderr, "exportGraphToMermaidFile: Could not open file for writing\n");
        return 0;
    }

    printf("Writing header...\n");
    fprintf(file, "---\nconfig:\nlayout: elk\ntheme: neo\nlook: neo\n---\n\nflowchart LR\n");

    printf("Writing vertices...\n");
    appendGraphVertexes(graph, file);
    fprintf(file, "\n");

    printf("Writing edges...\n");
    appendGraphEdges(graph, file);

    fclose(file);

    printf("=== Export Complete ===\n\n");
    return 1;
}

int exportHasseDiagramToMermaidFile(t_hasse_diagram hasse, const char* path)
{
    printf("\n=== Exporting Hasse Diagram to Mermaid ===\n");
    printf("Path: %s\n", path);
    printf("Number of classes: %d\n", hasse.partition->class_number);
    printf("Number of links: %d\n", hasse.logical_size);

    FILE *file = fopen(path, "w");
    if (file == NULL)
    {
        fprintf(stderr, "exportHasseDiagramToMermaidFile: Could not open file for writing\n");
        return 0;
    }

    printf("Writing header...\n");
    fprintf(file, "---\nconfig:\n   layout: elk\n   theme: mc\n   look: classic\n---\n\nflowchart LR\n");

    // Build class labels
    char **class_labels = buildClassLabels(*hasse.partition);
    if (class_labels == NULL) {
        fclose(file);
        return 0;
    }

    // Write nodes and edges
    printf("Writing nodes...\n");
    writeNodes(file, class_labels, hasse.partition->class_number);

    printf("Writing edges...\n");
    writeEdges(file, &hasse);

    // Free memory and close file
    freeClassLabels(class_labels, hasse.partition->class_number);
    fclose(file);

    printf("=== Export Complete ===\n\n");
    return 1;
}