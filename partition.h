#ifndef PARTITION_H
#define PARTITION_H

#include "class.h"

typedef struct s_partition{
    t_class *classes;
} t_partition;

void createPartition();
void freePartition(t_partition *);
void addClassToPartition(t_partition *, t_class *);

#endif //PARTITION_H
