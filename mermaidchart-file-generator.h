//
// Created by matteo on 06/11/2025.
//

#ifndef MERMAIDCHART_FILE_GENERATOR_H
#define MERMAIDCHART_FILE_GENERATOR_H

#include "graph.h"

int exportGraphToMermaidFile(t_graph, const char*, const char*);
void createEntry(int, int, double, FILE*);

#endif //MERMAIDCHART_FILE_GENERATOR_H
