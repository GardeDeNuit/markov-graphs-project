#include "partition.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

t_partition * createPartition(void){
    debugPrint(DEBUG_PARTITION, "createPartition: start");
    t_partition *partition = malloc(sizeof(t_partition));
    if (partition == NULL) {
        debugPrint(DEBUG_PARTITION, "createPartition: allocation failed");
        perror("createPartition : allocation failed");
        return NULL;
    }
    partition->classes = NULL;
    debugPrint(DEBUG_PARTITION, "createPartition: success");
    return partition;
}

void freeClasses(t_class *class){
    debugPrint(DEBUG_PARTITION, "freeClasses: start");
    if (class == NULL) {
        debugPrint(DEBUG_PARTITION, "freeClasses: class is NULL");
        return;
    }
    debugPrint(DEBUG_PARTITION, "freeClasses: freeing recursively");
    freeClasses(class->next);
    freeClass(class);
    debugPrint(DEBUG_PARTITION, "freeClasses: done");
}

void freePartition(t_partition *partition){
    debugPrint(DEBUG_PARTITION, "freePartition: start");
    if (partition == NULL) {
        debugPrint(DEBUG_PARTITION, "freePartition: partition is NULL");
        return;
    }
    debugPrint(DEBUG_PARTITION, "freePartition: freeing classes");
    freeClasses(partition->classes);
    free(partition);
    debugPrint(DEBUG_PARTITION, "freePartition: done");
}

void addClassToPartition(t_partition *partition, t_class *class){
    debugPrint(DEBUG_PARTITION, "addClassToPartition: start");
    if (partition == NULL) {
        debugPrint(DEBUG_PARTITION, "addClassToPartition: partition is NULL");
        fprintf(stderr, "addClassToPartition: partition is NULL\n");
        return;
    }
    if (class == NULL) {
        debugPrint(DEBUG_PARTITION, "addClassToPartition: class is NULL");
        fprintf(stderr, "addClassToPartition: class is NULL\n");
        return;
    }
    debugPrint(DEBUG_PARTITION, "addClassToPartition: adding class to partition");
    class->next = partition->classes;
    partition->classes = class;
    debugPrint(DEBUG_PARTITION, "addClassToPartition: success");
}

void displayPartition(t_partition *partition){
    debugPrint(DEBUG_PARTITION, "displayPartition: start");
    if (partition == NULL) {
        debugPrint(DEBUG_PARTITION, "displayPartition: partition is NULL");
        fprintf(stderr, "displayPartition: partition is NULL\n");
        return;
    }
    debugPrint(DEBUG_PARTITION, "displayPartition: displaying classes");
    t_class *curr = partition->classes;
    while (curr != NULL) {
        displayClass(curr);
        curr = curr->next;
    }
    debugPrint(DEBUG_PARTITION, "displayPartition: done");
}
