#ifndef __HASSE_H__
#define __HASSE_H__

struct s_link {
    int dept_nb;
    int dest_nb;
};

typedef struct s_link t_link;

struct s_link_array {
    t_link * links;
    int log_size;
    int max_size;
};

typedef struct s_link_array t_link_array;


t_link_array* addLink(t_link_array *link_array, int dept, int dest);
t_link_array* createClassLinks(int num_vertices,int **adj_list,int *adj_size,t_link_array *class_links);
void removeTransitiveLinks(t_link_array *p_link_array);
int *createArrayClass(int nb_sommets, t_link_array *arr);


int *ClassType(int*,int,t_link_array);
int isAbsorbingState(int*,int,int*);
int isIrreductible(int*,int);
/**
 * @brief Creates a link array from the given partition and graph.
 *
 * @param part The partition of the graph.
 * @param graph The adjacency list representation of the graph.
 * @return The created link array.
 */

#endif