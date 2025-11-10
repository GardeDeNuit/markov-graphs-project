#ifndef PARTITION_H
#define PARTITION_H

#include "class.h"

typedef struct s_partition{
    t_class *classes;
} t_partition;

t_partition * createPartition();
void freeClasses(t_class *);
void freePartition(t_partition *);
void addClassToPartition(t_partition *, t_class *);
void displayPartition(t_partition *);
#endif //PARTITION_H
