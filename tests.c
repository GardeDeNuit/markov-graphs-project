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
}
