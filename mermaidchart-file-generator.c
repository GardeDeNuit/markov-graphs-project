#include "mermaidchart-file-generator.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

int exportGraphToMermaidFile(t_graph graph, const char* path) {
    FILE *file = fopen(path, "w");

    if (file == NULL) {
        fprintf(stderr, "exportGraphToMermaidFile: Could not create file %s: %s\n", path, strerror(errno));
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