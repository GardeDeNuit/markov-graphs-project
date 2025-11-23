#ifndef MERMAIDCHART_FILE_GENERATOR_H
#define MERMAIDCHART_FILE_GENERATOR_H

#include "graph.h"
#include "hasse.h"

/**
 * @brief Exports a graph to a Mermaid flowchart file.
 *
 * Creates a Mermaid-formatted file representing the graph with:
 * - All vertices
 * - All edges (with weights)
 * - Styling heme : Neo
 *
 * @param graph The graph to export.
 * @param path Destination file path for the Mermaid file.
 * @return 1 on success, 0 on failure.
 */
int exportGraphToMermaidFile(t_graph graph, const char* path);

/**
 * @brief Exports a Hasse diagram to a Mermaid flowchart file.
 *
 * Creates a Mermaid-formatted file representing the Hasse diagram with:
 * - Classes as labeled nodes (A, B, C, ...)
 * - Links between classes
 * - Styling layout : ELK
 *
 * @param hasse The Hasse diagram to export.
 * @param path Destination file path for the Mermaid file.
 * @return 1 on success, 0 on failure.
 */
int exportHasseDiagramToMermaidFile(t_hasse_diagram hasse, const char* path);

#endif //MERMAIDCHART_FILE_GENERATOR_H
