#include "mermaidchart-file-generator.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

int exportGraphToMermaidFile(t_graph graph, const char* path) {
    FILE *file = fopen(path, "w");

    if (file == NULL) {;
        return 0;
    }

    fprintf(file, "---\nconfig:\nlayout: elk\ntheme: neo\nlook: neo\n---\n\nflowchart LR\n");
    appendGraphVertexes(graph, file);
    fprintf(file, "\n");
    appendGraphEdges(graph, file);

    fclose(file);
    return 1;
}

void appendVertex(const int vertex, FILE *file) {
    fprintf(file, "%s((%d))\n", getID(vertex), vertex);
}

void appendGraphVertexes(const t_graph graph, FILE *file) {
    int i = 0;
    for (i = 0; i<graph.size; i++) {
        appendVertex(i+1, file);
    }
}

void appendEdge(int src, int dest, double weight, FILE *file) {
    char srcID[64];
    char destID[64];

    strncpy(srcID, getID(src), sizeof(srcID) - 1);
    srcID[sizeof(srcID) - 1] = '\0';
    strncpy(destID, getID(dest), sizeof(destID) - 1);
    destID[sizeof(destID) - 1] = '\0';

    fprintf(file, "%s -->|%.5f|%s\n", srcID, weight, destID);
}

void appendGraphEdges(const t_graph graph, FILE *file) {
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

int exportHasseDiagramToMermaidFile(t_link_array *links, t_partition *partition, int num_classes, const char* path)
{
    FILE *file = fopen(path, "w");

    if (file == NULL) {
        fprintf(stderr, "exportHasseDiagramToMermaidFile: Could not open file for writing\n");
        return 0;
    }

    // En-tête Mermaid
    fprintf(file, "---\n");
    fprintf(file, "config:\n");
    fprintf(file, "layout: elk\n");
    fprintf(file, "theme: neo\n");
    fprintf(file, "look: neo\n");
    fprintf(file, "---\n\n");
    fprintf(file, "flowchart LR\n");

    // Créer les étiquettes des nœuds (composantes)
    char **class_labels = malloc(num_classes * sizeof(char*));

    int class_num = 0;
    t_class *curr_class = partition->classes;

    while (curr_class != NULL && class_num < num_classes)
    {
        class_labels[class_num] = malloc(256 * sizeof(char));
        memset(class_labels[class_num], 0, 256);

        // Construire la chaîne avec les sommets
        strcpy(class_labels[class_num], "{");

        t_vertex *curr_vertex = curr_class->vertices;
        int first = 1;
        while (curr_vertex != NULL)
        {
            if (!first) strcat(class_labels[class_num], ",");

            char vertex_str[16];
            snprintf(vertex_str, sizeof(vertex_str), "%d", curr_vertex->value);
            strcat(class_labels[class_num], vertex_str);

            first = 0;
            curr_vertex = curr_vertex->next;
        }

        strcat(class_labels[class_num], "}");

        class_num++;
        curr_class = curr_class->next;
    }

    // Écrire les nœuds
    for (int i = 0; i < num_classes; i++)
    {
        char node_id = 'A' + i;  // A, B, C, D, E, F, ...
        fprintf(file, "%c(\"%s\")\n", node_id, class_labels[i]);
    }

    fprintf(file, "\n");

    // Écrire les arêtes
    for (int i = 0; i < links->log_size; i++)
    {
        int from = links->links[i].src_nb;
        int to = links->links[i].dest_nb;

        char from_id = 'A' + from;
        char to_id = 'A' + to;

        fprintf(file, "%c --> %c\n", from_id, to_id);
    }

    fclose(file);

    // Libérer la mémoire
    for (int i = 0; i < num_classes; i++) {
        free(class_labels[i]);
    }
    free(class_labels);

    return 1;
}