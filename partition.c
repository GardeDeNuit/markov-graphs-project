#include "partition.h"

t_partition * createPartition(){
    t_partition *partition = malloc(sizeof(t_partition));
    partition->classes = NULL;
    return partition;
}

void freeClasses(t_class *class){
    if (class == NULL){
        return;
    }
    freeClasses(class->next);
    freeClass(class);
}

void freePartition(t_partition *partition){
    freeClasses(partition->classes);
    free(partition);
}

void addClassToPartition(t_partition *partition, t_class *class){
    class->next = partition->classes;
    partition->classes = class;
}

void displayPartition(t_partition *partition){
    if (partition == NULL) return;
    while (partition->classes != NULL) {
        displayClass(partition->classes);
        partition->classes = partition->classes->next;
    }
}