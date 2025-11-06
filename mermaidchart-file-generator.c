//
// Created by matteo on 06/11/2025.
//

#include "mermaidchart-file-generator.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int exportGraphToMermaidFile(t_graph graph, const char* path) {
    FILE *file = fopen(path, "w");
    int i = 0;

    if (file == NULL) {
        fprintf(stderr, "exportGraphToMermaidFile: Could not create file %s: %s\n", path, strerror(errno));
        return 0;
    }
    fprintf(file, "---\nconfig:\nlayout: elk\ntheme: neo\nlook: neo\n---\n\nflowchart LR\n");
    for (i = 0; i<graph.size; i++) {
        fprintf(file,"%s((%d))", getID(i+1), i+1);
    }
    for (i = 0; i<graph.size; i++) {
        t_list neighbors = graph.values[i];
        t_cell *cur = neighbors.head;
        while (cur != NULL) {
            createEntry(i+1, cur->vertex, cur->weight, file);
            cur = cur->next;
        }
    }

    fclose(file);
}


void createEntry(int src, int dest, double weight, FILE*){}