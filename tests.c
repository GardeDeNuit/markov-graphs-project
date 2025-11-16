#include <stdio.h>
#include <string.h>
#include "tests.h"
#include "graph.h"
#include "partition.h"
#include "class.h"
#include "utils.h"

static test_entry g_tests[256];
static int g_test_count = 0;


void add_test(const char *name, test_fn fn, const char *comment) {
    if (name == NULL || fn == NULL) return;
    if (g_test_count >= (int)(sizeof(g_tests)/sizeof(g_tests[0]))) return;
    g_tests[g_test_count].name = name;
    g_tests[g_test_count].fn = fn;
    g_tests[g_test_count].comment = comment ? comment : "";
    g_test_count++;
}

int run_all_tests(void) {
    printf("Running %d tests...\n", g_test_count);
    int failures = 0;
    for (int i = 0; i < g_test_count; ++i) {
        test_entry *t = &g_tests[i];
        int res = t->fn();
        if (res == 0) {
            printf("[PASS] %s", t->name);
        } else {
            printf("[FAIL] %s", t->name);
            failures++;
        }
        if (t->comment && strlen(t->comment) > 0) {
            printf(" (%s)", t->comment);
        }
        printf("\n");
    }
    printf("Summary: %d tests failed\n", failures);
    return failures;
}

/*
 * TESTS POUR LES LISTES CHAÎNÉES
 */

// Test création d'une liste vide
static int test_list_create_empty(void) {
    t_list l = createEmptyList();
    if (l.head != NULL) return 1;
    return 0;
}

// Test ajout d'un élément à une liste
static int test_list_add_cell(void) {
    t_list l = createEmptyList();
    addCell(&l, 3, 1.5);
    if (l.head == NULL) { freeList(&l); return 1; }
    if (l.head->vertex != 3) { freeList(&l); return 1; }
    if (l.head->weight != 1.5) { freeList(&l); return 1; }
    freeList(&l);
    return 0;
}

// Test libération mémoire d'une liste
static int test_list_free(void) {
    t_list l = createEmptyList();
    addCell(&l, 1, 0.5);
    addCell(&l, 2, 0.3);
    freeList(&l);
    if (l.head != NULL) return 1;
    return 0;
}

// Test somme des poids d'une liste vide
static int test_list_sum_empty(void) {
    t_list l = createEmptyList();
    if (sumValues(l) != 0.0) return 1;
    return 0;
}

// Test somme des poids d'une liste avec éléments
static int test_list_sum_values(void) {
    t_list l = createEmptyList();
    addCell(&l, 1, 0.4);
    addCell(&l, 2, 0.6);
    double sum = sumValues(l);
    freeList(&l);
    // Tolérance pour comparaison de flottants
    if (sum < 0.99 || sum > 1.01) return 1;
    return 0;
}

// Test ajout de plusieurs éléments (ordre d'insertion)
static int test_list_multiple_elements(void) {
    t_list l = createEmptyList();
    addCell(&l, 1, 0.1);
    addCell(&l, 2, 0.2);
    addCell(&l, 3, 0.3);
    // Les éléments sont ajoutés en tête, donc ordre inverse
    if (l.head->vertex != 3) { freeList(&l); return 1; }
    if (l.head->next->vertex != 2) { freeList(&l); return 1; }
    if (l.head->next->next->vertex != 1) { freeList(&l); return 1; }
    freeList(&l);
    return 0;
}


/*
 * TESTS POUR LES GRAPHES
 */

// Test création d'un graphe
static int test_graph_create(void) {
    t_graph g = createGraph(4);
    if (g.size != 4) { freeGraph(&g); return 1; }
    freeGraph(&g);
    return 0;
}

// Test ajout d'une arête
static int test_graph_add_edge(void) {
    t_graph g = createGraph(3);
    addEdge(&g, 1, 2, 0.5);
    if (!hasEdge(g, 1, 2)) { freeGraph(&g); return 1; }
    freeGraph(&g);
    return 0;
}

// Test détection d'arête inexistante
static int test_graph_no_edge(void) {
    t_graph g = createGraph(3);
    if (hasEdge(g, 1, 2)) { freeGraph(&g); return 1; }
    freeGraph(&g);
    return 0;
}

// Test récupération des voisins
static int test_graph_get_neighbors(void) {
    t_graph g = createGraph(4);
    addEdge(&g, 1, 2, 0.5);
    addEdge(&g, 1, 3, 0.3);
    t_list* neighbors = getNeighbors(&g, 1);
    if (neighbors == NULL) { freeGraph(&g); return 1; }
    if (neighbors->head == NULL) { freeGraph(&g); return 1; }
    // Au moins un voisin doit exister
    int found_neighbor = 0;
    t_cell* curr = neighbors->head;
    while (curr != NULL) {
        if (curr->vertex == 2 || curr->vertex == 3) found_neighbor = 1;
        curr = curr->next;
    }
    freeGraph(&g);
    return found_neighbor ? 0 : 1;
}

// Test avec sommet invalide
static int test_graph_invalid_vertex(void) {
    t_graph g = createGraph(3);
    t_list* neighbors = getNeighbors(&g, 5); // Sommet hors limites
    freeGraph(&g);
    // Devrait retourner NULL pour un sommet invalide
    return (neighbors == NULL) ? 0 : 1;
}

// Test libération d'un graphe
static int test_graph_free(void) {
    t_graph g = createGraph(2);
    addEdge(&g, 1, 2, 1.0);
    freeGraph(&g);
    // Après libération, on ne peut pas vraiment tester grand chose
    return 0;
}

/*
 * TESTS POUR LA LECTURE DE FICHIER
 */

// Test lecture d'un fichier valide
static int test_read_file_valid(void) {
    t_graph g = importGraphFromFile("../data/exemple1.txt");
    if (g.size == 0) { freeGraph(&g); return 1; }
    freeGraph(&g);
    return 0;
}

// Test lecture d'un fichier inexistant
static int test_read_file_nonexistent(void) {
    t_graph g = importGraphFromFile("fichier_inexistant.txt");
    // Devrait retourner un graphe de taille 0 en cas d'erreur
    int result = (g.size == 0) ? 0 : 1;
    freeGraph(&g);
    return result;
}

/*
 * TESTS POUR GRAPHE DE MARKOV
 */

// Test graphe de Markov valide
static int test_markov_valid(void) {
    t_graph g = importGraphFromFile("../data/exemple1.txt");
    if (g.size == 0) { freeGraph(&g); return 1; }
    int is_markov = is_graphMarkov(g);
    freeGraph(&g);
    return (is_markov == 1) ? 0 : 1;
}

// Test graphe non-Markov
static int test_markov_invalid(void) {
    t_graph g = importGraphFromFile("../data/exemple1_from_chatGPT.txt");
    if (g.size == 0) { freeGraph(&g); return 1; }
    int is_markov = is_graphMarkov(g);
    freeGraph(&g);
    return (is_markov == 0) ? 0 : 1;
}

/*
 * TESTS POUR LES CLASSES
 */

// Test création d'une classe
static int test_class_create(void) {
    t_class *c = createClass("TestClass");
    if (c == NULL) return 1;
    if (strcmp(c->name, "TestClass") != 0) { freeClass(c); return 1; }
    if (c->vertexes != NULL) { freeClass(c); return 1; }
    freeClass(c);
    return 0;
}

// Test ajout d'un sommet à une classe
static int test_class_add_vertex(void) {
    t_class *c = createClass("Test");
    if (c == NULL) return 1;
    addVertexToClass(c, 5);
    if (c->vertexes == NULL) { freeClass(c); return 1; }
    if (c->vertexes->value != 5) { freeClass(c); return 1; }
    freeClass(c);
    return 0;
}

// Test ajout de plusieurs sommets à une classe
static int test_class_multiple_vertices(void) {
    t_class *c = createClass("Multi");
    if (c == NULL) return 1;
    addVertexToClass(c, 1);
    addVertexToClass(c, 2);
    addVertexToClass(c, 3);
    // Ordre d'insertion en tête: 3 -> 2 -> 1
    if (c->vertexes->value != 3) { freeClass(c); return 1; }
    if (c->vertexes->next->value != 2) { freeClass(c); return 1; }
    if (c->vertexes->next->next->value != 1) { freeClass(c); return 1; }
    freeClass(c);
    return 0;
}

/*
 * TESTS POUR LES PARTITIONS
 */

// Test création d'une partition
static int test_partition_create(void) {
    t_partition *part = createPartition();
    if (part == NULL) return 1;
    if (part->classes != NULL) { freePartition(part); return 1; }
    freePartition(part);
    return 0;
}

// Test ajout d'une classe à une partition
static int test_partition_add_class(void) {
    t_partition *part = createPartition();
    if (part == NULL) return 1;
    t_class *class1 = createClass("C1");
    addClassToPartition(part, class1);
    if (part->classes == NULL) { freePartition(part); return 1; }
    freePartition(part);
    return 0;
}

// Test ajout de plusieurs classes à une partition
static int test_partition_multiple_classes(void) {
    t_partition *part = createPartition();
    if (part == NULL) return 1;
    t_class *class1 = createClass("C1");
    t_class *class2 = createClass("C2");
    addClassToPartition(part, class1);
    addClassToPartition(part, class2);
    if (part->classes == NULL) { freePartition(part); return 1; }
    if (part->classes->next == NULL) { freePartition(part); return 1; }
    freePartition(part);
    return 0;
}

/*
 * TESTS SUPPLÉMENTAIRES DE ROBUSTESSE
 */

// Test gestion des paramètres NULL
static int test_null_parameters(void) {
    // Test addCell avec liste NULL
    addCell(NULL, 1, 0.5); // Ne doit pas crasher

    // Test freeList avec NULL
    freeList(NULL); // Ne doit pas crasher

    return 0;
}

// Test limites des valeurs flottantes
static int test_float_limits(void) {
    t_list l = createEmptyList();
    addCell(&l, 1, 0.0);
    addCell(&l, 2, 1.0);
    addCell(&l, 3, 0.000001);
    double sum = sumValues(l);
    freeList(&l);
    // Vérifier que la somme est dans une plage raisonnable
    return (sum >= 0.9 && sum <= 1.1) ? 0 : 1;
}

void register_project_tests(void) {
    // Tests des listes
    add_test("list_create_empty", test_list_create_empty, "Création d'une liste vide");
    add_test("list_add_cell", test_list_add_cell, "Ajout d'un élément à une liste");
    add_test("list_free", test_list_free, "Libération mémoire d'une liste");
    add_test("list_sum_empty", test_list_sum_empty, "Somme d'une liste vide");
    add_test("list_sum_values", test_list_sum_values, "Somme des poids dans une liste");
    add_test("list_multiple_elements", test_list_multiple_elements, "Ordre d'insertion des éléments");

    // Tests des graphes
    add_test("graph_create", test_graph_create, "Création d'un graphe");
    add_test("graph_add_edge", test_graph_add_edge, "Ajout d'une arête");
    add_test("graph_no_edge", test_graph_no_edge, "Détection d'arête inexistante");
    add_test("graph_get_neighbors", test_graph_get_neighbors, "Récupération des voisins");
    add_test("graph_invalid_vertex", test_graph_invalid_vertex, "Gestion sommet invalide");
    add_test("graph_free", test_graph_free, "Libération d'un graphe");

    // Tests de lecture de fichier
    add_test("read_file_valid", test_read_file_valid, "Lecture d'un fichier valide");
    add_test("read_file_nonexistent", test_read_file_nonexistent, "Lecture fichier inexistant");

    // Tests Markov
    add_test("markov_valid", test_markov_valid, "Graphe de Markov valide");
    add_test("markov_invalid", test_markov_invalid, "Graphe non-Markov");

    // Tests des classes
    add_test("class_create", test_class_create, "Création d'une classe");
    add_test("class_add_vertex", test_class_add_vertex, "Ajout d'un sommet à une classe");
    add_test("class_multiple_vertices", test_class_multiple_vertices, "Plusieurs sommets dans une classe");

    // Tests des partitions
    add_test("partition_create", test_partition_create, "Création d'une partition");
    add_test("partition_add_class", test_partition_add_class, "Ajout d'une classe à une partition");
    add_test("partition_multiple_classes", test_partition_multiple_classes, "Plusieurs classes dans une partition");

    // Tests de robustesse
    add_test("null_parameters", test_null_parameters, "Gestion des paramètres NULL");
    add_test("float_limits", test_float_limits, "Limites des valeurs flottantes");
}
