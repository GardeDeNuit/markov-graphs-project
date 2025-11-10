#include <stdio.h>
#include "graph.h"
#include "mermaidchart-file-generator.h"
#include "utils.h"
#include "partition.h"

static void print_result(const char* name, int ok) {
    printf("%s: %s\n", name, ok ? "PASS" : "FAIL");
}

// Test basique de la liste chaînée
static int test_list_basic(void) {
    t_list l = createEmptyList();
    if (l.head != NULL) return 1;
    addCell(&l, 3, 1.5);
    if (l.head == NULL) { freeList(&l); return 1; }
    if (l.head->vertex != 3) { freeList(&l); return 1; }
    freeList(&l);
    if (l.head != NULL) return 1;
    return 0;
}

// Test basique de la liste d'adjacence
static int test_adjacency_basic(void) {
    t_adjacency_list al = createEmptyAdjacencyList(3);
    if (al.size != 3) return 1;
    if (al.values == NULL) { freeAdjacencyList(&al); return 1; }
    freeAdjacencyList(&al);
    if (al.values != NULL) return 1;
    return 0;
}

// Test basique du graphe (ajout, recherche, suppression)
static int test_graph_edges(void) {
    t_graph g = createGraph(4);
    addEdge(&g, 1, 2, 0.5);
    if (!hasEdge(g, 1, 2)) { freeGraph(&g); return 1; }
    t_list* neigh = getNeighbors(&g, 1);
    if (neigh == NULL) { freeGraph(&g); return 1; }
    if (neigh->head == NULL) { freeGraph(&g); return 1; }
    if (neigh->head->vertex != 2) { freeGraph(&g); return 1; }
    freeGraph(&g);
    return 0;
}

// Test lecture depuis fichier (chemin relatif géré par importGraphFromFile)
static int test_read_from_file(void) {
    t_graph g = importGraphFromFile("../data/exemple1.txt");
    int ok = 0;
    if (g.size == 4 && hasEdge(g, 1, 2) && hasEdge(g, 4, 1)) ok = 1;
    freeGraph(&g);
    return ok ? 0 : 1;
}

// Test probabilités sommets d'un graphe
static int test_sum_list(void){
    t_list l = createEmptyList();
    if (sumValues(l)!=0) {return 1;}
    addCell(&l, 1, 1);
    if (sumValues(l)!=1) {return 1;}
    addCell(&l, 2, 0.60);
    if (sumValues(l)!=1.60) {return 1;}
    return 0;
}

static int test_isMarkov_graph(void){
    t_graph g = importGraphFromFile("../data/exemple1_from_chatGPT.txt");
    if (is_graphMarkov(g)!=0) {return 1;}

    g = importGraphFromFile("../data/exemple1.txt");
    if (is_graphMarkov(g)!=1) {return 1;}
    return 0;
}

static int test_partition(void) {
    t_partition *part = createPartition();
    if (part == NULL) return 1;
    t_class *class1 = createClass("C1");
    addClassToPartition(part, class1);
    if (part->classes == NULL) {
        freePartition(part);
        return 1;
    }
    t_class *class2 = createClass("C2");
    addClassToPartition(part, class2);
    if (part->classes->next == NULL) {
        freePartition(part);
        return 1;
    }
    freePartition(part);
    return 0;
}

int main(void) {
    t_graph g = importGraphFromFile("../data/exemple_valid_step3.txt");
    displayGraph(g);
    exportGraphToMermaidFile(g, "../data/output.txt");

    int failures = 0;
    int r;

    r = test_list_basic();
    print_result("test_list_basic", r == 0);
    failures += r;

    r = test_adjacency_basic();
    print_result("test_adjacency_basic", r == 0);
    failures += r;

    r = test_graph_edges();
    print_result("test_graph_edges", r == 0);
    failures += r;

    r = test_read_from_file();
    print_result("test_read_from_file", r == 0);
    failures += r;

    r = test_sum_list();
    print_result("test_sum_list",r==0);
    failures += r;

    r = test_isMarkov_graph();
    print_result("test_isMarkov_graph",r==0);
    failures += r;

    r = test_partition();
    print_result("test_partition", r == 0);
    failures += r;

    printf("Summary: %d tests failed\n", failures);
    return failures;
}