#ifndef MERMAIDCHART_FILE_GENERATOR_H
#define MERMAIDCHART_FILE_GENERATOR_H

#include "graph.h"
#include "hasse.h"

int exportGraphToMermaidFile(t_graph graph, const char* path);
void appendVertex(int vertex, FILE *file);
void appendGraphVertexes(t_graph graph, FILE *file);
void appendEdge(int src, int dest, double weight, FILE *file);
void appendGraphEdges(t_graph graph, FILE *file);
int exportHasseDiagramToMermaidFile(t_link_array *links, t_partition *partition, int num_classes, const char* path);


#endif //MERMAIDCHART_FILE_GENERATOR_H
