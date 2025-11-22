#include "partition.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

t_partition * createPartition(void){
    t_partition *partition = malloc(sizeof(t_partition));
    if (partition == NULL) {
        fprintf(stderr, "createPartition: memory allocation failed\n");
        return NULL;
    }
    partition->classes = NULL;
    return partition;
}

/**
 * @brief Recursively frees all classes in a linked list
 * @param class Pointer to the first class to free
 */
static void freeClasses(t_class *class){
    if (class == NULL) {
        return;
    }
    freeClasses(class->next);
    freeClass(class);
}

int freePartition(t_partition *partition){
    if (partition == NULL) {
        fprintf(stderr, "freePartition: partition pointer is NULL\n");
        return -1;
    }
    freeClasses(partition->classes);
    free(partition);
    return 1;
}

int addClassToPartition(t_partition *partition, t_class *class){
    if (partition == NULL) {
        fprintf(stderr, "addClassToPartition: partition pointer is NULL\n");
        return -1;
    }
    if (class == NULL) {
        fprintf(stderr, "addClassToPartition: class pointer is NULL\n");
        return -1;
    }
    class->next = partition->classes;
    partition->classes = class;
    return 1;
}

void displayPartition(t_partition *partition){
    if (partition == NULL) {
        fprintf(stderr, "displayPartition: partition pointer is NULL\n");
        return;
    }
    t_class *curr = partition->classes;
    while (curr != NULL) {
        displayClass(curr);
        curr = curr->next;
    }
}

int generateClassId(t_partition partition) {
    int index = 0;
    // Calculate the number of existing classes to generate a unique id
    t_class *curr = partition.classes;
    while (curr != NULL) {
        index++;
        curr = curr->next;
    }
    index++; // Increment for the new id
    return index;
}