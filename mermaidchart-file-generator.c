//
// Created by matteo on 06/11/2025.
//

#include "mermaidchart-file-generator.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

int exportGraphToMermaidFile(t_graph graph, const char* path) {
    return 1;
}
void createEntry(int src, int dest, double weight, FILE* file) {
    fprintf(file,"%s -->|%.5f|%s\n", getID(src), weight, getID(dest));
}