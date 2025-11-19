#include <stdio.h>
#include <string.h>
#include "tests.h"
#include "graph.h"
#include "partition.h"
#include "class.h"
#include "utils.h"
#include "tarjan.h"

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

        // Afficher le nom et la description avant l'exécution
        printf("Test: %s", t->name);
        if (t->comment && strlen(t->comment) > 0) {
            printf(" (%s)", t->comment);
        }
        printf("...\n");

        // Exécuter le test
        int res = t->fn();

        // Afficher le statut après l'exécution
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


// Tests pour class.c
int test_createClass_normal() {
    t_class *class = createClass("SCC1");
    int result = (class != NULL && strcmp(class->name, "SCC1") == 0 && class->vertices == NULL && class->next == NULL) ? 0 : 1;
    freeClass(class);
    return result;
}

int test_createClass_long_name() {
    t_class *class = createClass("UnNomTropLongPourLaStructureUnNomTropLongPourLaStructure");
    int result = (class == NULL) ? 0 : 1;
    if (class) freeClass(class);
    return result;
}

int test_createVertex_normal() {
    t_vertex *vertex = createVertex(42);
    int result = (vertex != NULL && vertex->value == 42 && vertex->next == NULL) ? 0 : 1;
    free(vertex);
    return result;
}

int test_addVertexToClass_normal() {
    t_class *class = createClass("Test");
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
    t_class *class = createClass("TestDisplay");
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
    t_class *class = createClass("TestFree");
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
    t_class *class1 = createClass("SCC1");
    t_class *class2 = createClass("SCC2");

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
    t_class *class1 = createClass("SCC1");
    t_class *class2 = createClass("SCC2");

    addVertexToClass(class1, 1);
    addVertexToClass(class2, 2);

    addClassToPartition(partition, class1);
    addClassToPartition(partition, class2);

    int result = (partition->classes != NULL && partition->classes == class2) ? 0 : 1;
    freePartition(partition);
    return result;
}

int test_addClassToPartition_null_partition() {
    t_class *class = createClass("Test");
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
    t_class *class1 = createClass("SCC1");
    t_class *class2 = createClass("SCC2");

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
    t_class *class1 = createClass("SCC1");
    t_class *class2 = createClass("SCC2");

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
    t_class *scc1 = createClass("SCC1");
    t_class *scc2 = createClass("SCC2");
    t_class *scc3 = createClass("SCC3");

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

    printf("Scénario complet - Partition avec 3 classes:\n");
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

    printf("Scénario complet - Affichage de 3 vertices Tarjan:\n");
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

void register_project_tests(void) {
    // Tests class.c
    add_test("createClass_normal", test_createClass_normal, "Création normale d'une classe");
    add_test("createClass_long_name", test_createClass_long_name, "Nom trop long pour une classe");
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
}
