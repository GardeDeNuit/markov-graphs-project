#include "partition.h"

t_partition * createPartition(){
    t_partition *partition = malloc(sizeof(t_partition));
    partition->classes = NULL;
    return partition;
}

void freeClasses(t_class *class){
    if (class->next == NULL){
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

