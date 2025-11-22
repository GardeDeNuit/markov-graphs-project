#include <stdio.h>
#include <string.h>
#include "tests.h"
#include "graph.h"
#include "partition.h"
#include "class.h"
#include "utils.h"
#include "tarjan.h"
#include "tarjan_vertex.h"

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
        printf("Test: %s", t->name);
        if (t->comment && strlen(t->comment) > 0) {
            printf(" (%s)", t->comment);
        }
        printf("...\n");
        int res = t->fn();
        if (res == 0) {
            printf("[PASS]\n");
        } else {
            printf("[FAIL]\n");
            failures++;
        }
    }
    printf("Summary: %d tests failed\n", failures);
    return failures;
}

// Tests pour les listes chaînées
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



// Tests pour les graphes
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


// Tests pour l'importation de graphes
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


// Tests pour les graphs de markov
static int test_markov_valid(void) {
    t_graph g = importGraphFromFile("../data/exemple1.txt");
    if (g.size == 0) { freeGraph(&g); return 1; }
    int is_markov = is_graphMarkov(g);
    freeGraph(&g);
    return (is_markov == 1) ? 0 : 1;
}

static int test_markov_invalid(void) {
    t_graph g = importGraphFromFile("../data/exemple1_from_chatGPT.txt");
    if (g.size == 0) { freeGraph(&g); return 1; }
    int is_markov = is_graphMarkov(g);
    freeGraph(&g);
    return (is_markov == 0) ? 0 : 1;
}

// Tests pour class.c
int test_createClass_normal() {
    t_class *class = createClass(1);
    int result = (class != NULL && class->id == 1 && class->vertices == NULL && class->next == NULL) ? 0 : 1;
    freeClass(class);
    return result;
}

int test_createVertex_normal() {
    t_vertex *vertex = createVertex(42);
    int result = (vertex != NULL && vertex->value == 42 && vertex->next == NULL) ? 0 : 1;
    free(vertex);
    return result;
}

int test_addVertexToClass_normal() {
    t_class *class = createClass(1);
    addVertexToClass(class, 1);
    addVertexToClass(class, 2);
    addVertexToClass(class, 3);
    int result = (class->vertices != NULL && class->vertices->value == 3) ? 0 : 1;
    freeClass(class);
    return result;
}

int test_addVertexToClass_null() {
    addVertexToClass(NULL, 1);
    return 0; // Pas de crash = succès
}

int test_displayClass() {
    t_class *class = createClass(1);
    addVertexToClass(class, 10);
    addVertexToClass(class, 20);
    addVertexToClass(class, 30);

    printf("Affichage attendu : Class TestDisplay: {30,20,10}\n");
    displayClass(class);
    freeClass(class);
    return 0; // Test visuel
}

int test_displayClass_null() {
    displayClass(NULL);
    return 0; // Pas de crash = succès
}

int test_displayVertices_empty() {
    displayVertices(NULL);
    return 0; // Pas de crash = succès
}

int test_freeClass_normal() {
    t_class *class = createClass(1);
    addVertexToClass(class, 1);
    addVertexToClass(class, 2);
    freeClass(class);
    return 0; // Pas de crash = succès
}

int test_freeClass_null() {
    freeClass(NULL);
    return 0; // Pas de crash = succès
}

int test_class_complete_scenario() {
    t_class *class1 = createClass(1);
    t_class *class2 = createClass(2);

    addVertexToClass(class1, 1);
    addVertexToClass(class1, 2);
    addVertexToClass(class2, 3);
    addVertexToClass(class2, 4);
    addVertexToClass(class2, 5);

    printf("Affichage des classes créées:\n");
    displayClass(class1);
    displayClass(class2);

    freeClass(class1);
    freeClass(class2);

    return 0; // Scénario complet sans crash
}

// Tests pour partition.c
int test_createPartition_normal() {
    t_partition *partition = createPartition();
    int result = (partition != NULL && partition->classes == NULL) ? 0 : 1;
    freePartition(partition);
    return result;
}

int test_addClassToPartition_normal() {
    t_partition *partition = createPartition();
    t_class *class1 = createClass(1);
    t_class *class2 = createClass(2);

    addVertexToClass(class1, 1);
    addVertexToClass(class2, 2);

    addClassToPartition(partition, class1);
    addClassToPartition(partition, class2);

    int result = (partition->classes != NULL && partition->classes == class2) ? 0 : 1;
    freePartition(partition);
    return result;
}

int test_addClassToPartition_null_partition() {
    t_class *class = createClass(1);
    addClassToPartition(NULL, class);
    freeClass(class);
    return 0; // Pas de crash = succès
}

int test_addClassToPartition_null_class() {
    t_partition *partition = createPartition();
    addClassToPartition(partition, NULL);
    freePartition(partition);
    return 0; // Pas de crash = succès
}

int test_displayPartition_normal() {
    t_partition *partition = createPartition();
    t_class *class1 = createClass(1);
    t_class *class2 = createClass(2);

    addVertexToClass(class1, 1);
    addVertexToClass(class1, 2);
    addVertexToClass(class2, 3);
    addVertexToClass(class2, 4);

    addClassToPartition(partition, class1);
    addClassToPartition(partition, class2);

    printf("Affichage attendu de la partition:\n");
    displayPartition(partition);

    freePartition(partition);
    return 0; // Test visuel
}

int test_displayPartition_null() {
    displayPartition(NULL);
    return 0; // Pas de crash = succès
}

int test_displayPartition_empty() {
    t_partition *partition = createPartition();
    displayPartition(partition);
    freePartition(partition);
    return 0; // Pas de crash = succès
}

int test_freePartition_normal() {
    t_partition *partition = createPartition();
    t_class *class1 = createClass(1);
    t_class *class2 = createClass(2);

    addVertexToClass(class1, 1);
    addVertexToClass(class2, 2);
    addClassToPartition(partition, class1);
    addClassToPartition(partition, class2);

    freePartition(partition);
    return 0; // Pas de crash = succès
}

int test_freePartition_null() {
    freePartition(NULL);
    return 0; // Pas de crash = succès
}

int test_partition_complete_scenario() {
    t_partition *partition = createPartition();

    // Créer plusieurs classes avec des vertices
    t_class *scc1 = createClass(1);
    t_class *scc2 = createClass(2);
    t_class *scc3 = createClass(3);

    addVertexToClass(scc1, 1);
    addVertexToClass(scc1, 2);
    addVertexToClass(scc2, 3);
    addVertexToClass(scc3, 4);
    addVertexToClass(scc3, 5);
    addVertexToClass(scc3, 6);

    // Ajouter à la partition
    addClassToPartition(partition, scc1);
    addClassToPartition(partition, scc2);
    addClassToPartition(partition, scc3);

    printf("Partition avec 3 classes:\n");
    displayPartition(partition);

    freePartition(partition);
    return 0; // Scénario complet sans crash
}

// Tests pour tarjan_vertex.c
int test_createTarjanVertex_normal() {
    t_tarjan_vertex *vertex = createTarjanVertex(1, 5, 3, 1);
    int result = (vertex != NULL && vertex->id == 1 && vertex->num == 5 &&
                  vertex->num_accessible == 3 && vertex->in_pile == 1 && vertex->next == NULL) ? 0 : 1;
    freeTarjanVertex(vertex);
    return result;
}

int test_createTarjanVertex_invalid_in_pile() {
    t_tarjan_vertex *vertex = createTarjanVertex(1, 5, 3, 2); // in_pile invalide
    int result = (vertex == NULL) ? 0 : 1;
    if (vertex) freeTarjanVertex(vertex);
    return result;
}

int test_createTarjanVertex_negative_values() {
    t_tarjan_vertex *vertex = createTarjanVertex(-1, -5, -3, 0);
    int result = (vertex == NULL) ? 0 : 1;
    if (vertex) freeTarjanVertex(vertex);
    return result;
}

int test_createTarjanVertex_negative_id() {
    t_tarjan_vertex *vertex = createTarjanVertex(-5, 10, 5, 1);
    int result = (vertex == NULL) ? 0 : 1;
    if (vertex) freeTarjanVertex(vertex);
    return result;
}

int test_displayTarjanVertex_normal() {
    t_tarjan_vertex vertex;
    vertex.id = 42;
    vertex.num = 10;
    vertex.num_accessible = 5;
    vertex.in_pile = 1;
    vertex.next = NULL;

    printf("Affichage attendu: (42, 10, 5, 1)\n");
    displayTarjanVertex(vertex);
    printf("\n");
    return 0; // Test visuel
}

int test_displayTarjanVertex_zero_values() {
    t_tarjan_vertex vertex;
    vertex.id = 0;
    vertex.num = 0;
    vertex.num_accessible = 0;
    vertex.in_pile = 0;
    vertex.next = NULL;

    printf("Affichage attendu: (0, 0, 0, 0)\n");
    displayTarjanVertex(vertex);
    printf("\n");
    return 0; // Test visuel
}

int test_freeTarjanVertex_normal() {
    t_tarjan_vertex *vertex = createTarjanVertex(1, 2, 3, 0);
    freeTarjanVertex(vertex);
    return 0; // Pas de crash = succès
}

int test_freeTarjanVertex_null() {
    freeTarjanVertex(NULL);
    return 0; // Pas de crash = succès
}

int test_tarjan_vertex_complete_scenario() {
    // Créer plusieurs vertices Tarjan
    t_tarjan_vertex *v1 = createTarjanVertex(1, 1, 1, 1);
    t_tarjan_vertex *v2 = createTarjanVertex(2, 2, 1, 1);
    t_tarjan_vertex *v3 = createTarjanVertex(3, 3, 3, 0);

    if (!v1 || !v2 || !v3) {
        if (v1) freeTarjanVertex(v1);
        if (v2) freeTarjanVertex(v2);
        if (v3) freeTarjanVertex(v3);
        return 1;
    }

    printf("Affichage de 3 vertices Tarjan:\n");
    printf("Vertex 1: ");
    displayTarjanVertex(*v1);
    printf("\nVertex 2: ");
    displayTarjanVertex(*v2);
    printf("\nVertex 3: ");
    displayTarjanVertex(*v3);
    printf("\n");

    freeTarjanVertex(v1);
    freeTarjanVertex(v2);
    freeTarjanVertex(v3);

    return 0; // Scénario complet sans crash
}

int test_graphToTarjanVertices_valid() {
    t_graph graph = createGraph(3);
    if (graph.values == NULL) return 1;
    t_tarjan_vertex **array = graphToTarjanVertices(graph);
    int result = 0;
    if (array == NULL) {
        result = 1;
    } else {
        for (int i = 0; i < graph.size; ++i) {
            if (array[i] == NULL || array[i]->id != i + 1 || array[i]->num != UNVISITED || array[i]->num_accessible != UNVISITED) {
                result = 1;
                break;
            }
        }
    }
    freeTarjanVerticesPartial(array, graph.size);
    freeGraph(&graph);
    return result;
}

int test_graphToTarjanVertices_invalid_size() {
    t_graph graph = createEmptyGraph();
    t_tarjan_vertex **array = graphToTarjanVertices(graph);
    return (array == NULL) ? 0 : 1;
}

int test_tarjan_empty_graph() {
    t_graph graph = createEmptyGraph();
    t_partition *partition = tarjan(graph);
    int result = (partition != NULL && partition->classes == NULL) ? 0 : 1;
    freePartition(partition);
    return result;
}

int test_tarjan_single_vertex_no_edges() {
    t_graph graph = createGraph(1);
    if (graph.values == NULL) return 1;
    t_partition *partition = tarjan(graph);
    int result = 1;
    if (partition != NULL && partition->classes != NULL &&
        partition->classes->vertices != NULL &&
        partition->classes->vertices->value == 1 &&
        partition->classes->vertices->next == NULL &&
        partition->classes->next == NULL) {
        result = 0;
    }
    freePartition(partition);
    freeGraph(&graph);
    return result;
}

int test_tarjan_imported_graph_example3() {
    t_graph graph = importGraphFromFile("..\\data\\exemple3.txt");
    if (graph.size == 0) return 1;
    t_partition *partition = tarjan(graph);
    int result = (partition != NULL && partition->classes != NULL) ? 0 : 1;
    printf("Partition résultante de l'exemple 3:\n");
    displayPartition(partition);
    freePartition(partition);
    freeGraph(&graph);
    return result;
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

    // Tests class.c
    add_test("createClass_normal", test_createClass_normal, "Création normale d'une classe");
    add_test("createVertex_normal", test_createVertex_normal, "Création normale d'un vertex");
    add_test("addVertexToClass_normal", test_addVertexToClass_normal, "Ajout de vertices à une classe");
    add_test("addVertexToClass_null", test_addVertexToClass_null, "Ajout vertex à classe NULL");
    add_test("displayClass", test_displayClass, "Affichage d'une classe");
    add_test("displayClass_null", test_displayClass_null, "Affichage classe NULL");
    add_test("displayVertices_empty", test_displayVertices_empty, "Affichage vertices vide");
    add_test("freeClass_normal", test_freeClass_normal, "Libération normale d'une classe");
    add_test("freeClass_null", test_freeClass_null, "Libération classe NULL");
    add_test("class_complete_scenario", test_class_complete_scenario, "Scénario complet de test");

    // Tests partition.c
    add_test("createPartition_normal", test_createPartition_normal, "Création normale d'une partition");
    add_test("addClassToPartition_normal", test_addClassToPartition_normal, "Ajout de classes à une partition");
    add_test("addClassToPartition_null_partition", test_addClassToPartition_null_partition, "Ajout classe à partition NULL");
    add_test("addClassToPartition_null_class", test_addClassToPartition_null_class, "Ajout classe NULL à partition");
    add_test("displayPartition_normal", test_displayPartition_normal, "Affichage d'une partition");
    add_test("displayPartition_null", test_displayPartition_null, "Affichage partition NULL");
    add_test("displayPartition_empty", test_displayPartition_empty, "Affichage partition vide");
    add_test("freePartition_normal", test_freePartition_normal, "Libération normale d'une partition");
    add_test("freePartition_null", test_freePartition_null, "Libération partition NULL");
    add_test("partition_complete_scenario", test_partition_complete_scenario, "Scénario complet partition");

    // Tests tarjan_vertex.c
    add_test("createTarjanVertex_normal", test_createTarjanVertex_normal, "Création normale d'un vertex Tarjan");
    add_test("createTarjanVertex_invalid_in_pile", test_createTarjanVertex_invalid_in_pile, "Création avec in_pile invalide");
    add_test("createTarjanVertex_negative_values", test_createTarjanVertex_negative_values, "Création avec id négatif");
    add_test("createTarjanVertex_negative_id", test_createTarjanVertex_negative_id, "Création avec id négatif uniquement");
    add_test("displayTarjanVertex_normal", test_displayTarjanVertex_normal, "Affichage vertex Tarjan normal");
    add_test("displayTarjanVertex_zero_values", test_displayTarjanVertex_zero_values, "Affichage vertex Tarjan valeurs nulles");
    add_test("freeTarjanVertex_normal", test_freeTarjanVertex_normal, "Libération normale vertex Tarjan");
    add_test("freeTarjanVertex_null", test_freeTarjanVertex_null, "Libération vertex Tarjan NULL");
    add_test("tarjan_vertex_complete_scenario", test_tarjan_vertex_complete_scenario, "Scénario complet vertex Tarjan");

    // Tests tarjan.c
    add_test("graphToTarjanVertices_valid", test_graphToTarjanVertices_valid, "Conversion basique du graphe vers Tarjan");
    add_test("graphToTarjanVertices_invalid_size", test_graphToTarjanVertices_invalid_size, "Conversion avec graphe vide");
    add_test("tarjan_empty_graph", test_tarjan_empty_graph, "Tarjan sur graphe vide");
    add_test("tarjan_single_vertex_no_edges", test_tarjan_single_vertex_no_edges, "Tarjan sur sommet isolé");
    add_test("tarjan_imported_graph_example3", test_tarjan_imported_graph_example3, "Tarjan sur data/example3.txt");
}
