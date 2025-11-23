#ifndef PARTITION_H
#define PARTITION_H

/**
 * @file partition.h
 * @brief Structures and operations for managing partitions of classes
 */

#include "class.h"

/**
 * @struct s_partition
 * @brief Represents a partition containing a linked list of classes
 * @param classes Pointer to the first class in the partition
 */
typedef struct s_partition{
    t_class *classes;
    int class_number;
} t_partition;

/**
 * @brief Creates a new empty partition
 * @return Pointer to the newly created partition, or NULL on allocation failure
 */
t_partition * createPartition();

/**
 * @brief Frees all memory allocated for a partition and its classes
 * @param partition Pointer to the partition to free
 * @return 1 on success, -1 if partition is NULL
 */
int freePartition(t_partition *partition);

/**
 * @brief Adds a class to the beginning of a partition's class list
 * @param partition Pointer to the partition
 * @param class Pointer to the class to add
 * @return 1 on success, -1 on failure (NULL partition or class)
 */
int addClassToPartition(t_partition *partition, t_class *class);

/**
 * @brief Displays all classes in a partition
 * @param partition Pointer to the partition to display
 */
void displayPartition(t_partition *partition);

/**
 * @brief Generates a unique class ID based on existing classes in partition
 * @param partition The partition to analyze
 * @return A unique class ID (number of existing classes + 1)
 */
int generateClassId(t_partition partition);

#endif //PARTITION_H
