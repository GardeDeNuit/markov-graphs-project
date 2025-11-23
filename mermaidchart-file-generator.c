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

char** buildClassLabels(t_partition *partition, int num_classes)
{
    char **class_labels = malloc(num_classes * sizeof(char*));
    t_class *curr_class = partition->classes;

    for (int i = 0; i < num_classes && curr_class != NULL; i++, curr_class = curr_class->next)
    {
        t_vertex *v = curr_class->vertices;
        int len = 0;
        class_labels[i] = malloc(256);
        len += snprintf(class_labels[i] + len, 256 - len, "{");

        int first = 1;
        while (v != NULL)
        {
            len += snprintf(class_labels[i] + len, 256 - len, "%s%d", first ? "" : ",", v->value);
            first = 0;
            v = v->next;
        }

        snprintf(class_labels[i] + len, 256 - len, "}");
    }

    return class_labels;
}


void writeNodes(FILE *file, char **class_labels, int num_classes)
{
    for (int i = 0; i < num_classes; i++)
    {
        char node_id = 'A' + i;
        fprintf(file, "%c(\"%s\")\n", node_id, class_labels[i]);
    }
    fprintf(file, "\n");
}


void writeEdges(FILE *file, t_link_array *links)
{
    for (int i = 0; i < links->logical_size; i++)
    {
        int from = links->links[i].src_id;
        int to = links->links[i].dest_id;

        char from_id = 'A' + from;
        char to_id = 'A' + to;

        fprintf(file, "%c --> %c\n", from_id, to_id);
    }
}


void freeClassLabels(char **class_labels, int num_classes)
{
    for (int i = 0; i < num_classes; i++)
    {
        free(class_labels[i]);
    }
    free(class_labels);
}

int exportHasseDiagramToMermaidFile(t_link_array *links, t_partition *partition, int num_classes, const char* path)
{
    FILE *file = fopen(path, "w");
    if (file == NULL)
    {
        fprintf(stderr, "exportHasseDiagramToMermaidFile: Could not open file for writing\n");
        return 0;
    }

    // En-tête Mermaid
    fprintf(file, "---\nconfig:\nlayout: elk\ntheme: neo\nlook: neo\n---\n\nflowchart LR\n");

    // Construire les labels
    char **class_labels = buildClassLabels(partition, num_classes);

    // Écrire les nœuds et les arêtes
    writeNodes(file, class_labels, num_classes);
    writeEdges(file, links);

    // Libérer la mémoire et fermer le fichier
    freeClassLabels(class_labels, num_classes);
    fclose(file);

    return 1;
}