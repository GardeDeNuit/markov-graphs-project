//
// Created by matteo on 06/11/2025.
//

#ifndef MERMAIDCHART_FILE_GENERATOR_H
#define MERMAIDCHART_FILE_GENERATOR_H

#include "graph.h"

int exportGraphToMermaidFile(t_graph graph, const char* path);
void appendVertex(int vertex, FILE *file);
void appendGraphVertexes(t_graph graph, FILE *file);
void appendEdge(int src, int dest, double weight, FILE *file);
void appendGraphEdges(t_graph graph, FILE *file);


#endif //MERMAIDCHART_FILE_GENERATOR_H
