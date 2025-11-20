#include <malloc.h>
#include "hasse.h"


void removeTransitiveLinks(t_link_array *p_link_array)
//enleve des liens en trop

{
    int i = 0;
    while (i < p_link_array->log_size)
    {
        t_link link1 = p_link_array->links[i];
        int j = 0;
        int to_remove = 0;
        while (j < p_link_array->log_size && !to_remove)
        {
            if (j != i)
            {
                t_link link2 = p_link_array->links[j];
                if (link1.from == link2.from)
                {
                    // look for a link from link2.to to link1.to
                    int k = 0;
                    while (k < p_link_array->log_size && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = p_link_array->links[k];
                            if ((link3.from == link2.to) && (link3.to == link1.to))
                            {
                                to_remove = 1;
                            }
                        }
                        k++;
                    }
                }
            }
            j++;
        }
        if (to_remove)
        {
            // remove link1 by replacing it with the last link
            p_link_array->links[i] = p_link_array->links[p_link_array->log_size - 1];
            p_link_array->log_size--;
        }
        else
        {
            i++;
        }
    }
}

int *createArrayClass(nb_vertex, *array)
//fonction qui permet de creer un tableau qui indique la classe de chaque sommet

{
    int *class = malloc(nb_vertex * sizeof(int));

    for (int i = 0; i < nb_vertex; i++)
        class[i] = i;

    for (int i = 0; i < array->log_size; i++)
    {
        //associe chaque classe au plus petit numéro
        int a = array->links[i].dept_nb;
        int b = array->links[i].dest_nb;

        class[b] = class[a];
    }

    return class;
}

void addLink(*link_array,dept,dest) {
    //fonction qui permet de creer les connections du diagramme de hasse

    if (link_array->log_size >= link_array->max_size)
    {
        // Optional: automatic realloc
        link_array->max_size *= 2;
        link_array->links = realloc(link_array->links, link_array->max_size * sizeof(t_link));
    }

    link_array->links[link_array->log_size].dept_nb = dept;
    link_array->links[link_array->log_size].dest_nb = dest;
    link_array->log_size++;
}

int createClassLinks(num_vertices,**adj_list,*adj_size,*class_links)
//fonction qui permet de creer le diagramme de hasse

{
    // Cree le tableau de classe
    int *class_array = createArrayClass(num_vertices, class_links);
    if (!class_array) return; //si il n'y a pas de classe

    //
    for (int i = 0; i < num_vertices; i++)
    {
        int Ci = class_array[i];

        //
        for (int k = 0; k < adj_size[i]; k++)
        {
            int j = adj_list[i][k];
            int Cj = class_array[j];

            // classe différente on ajoute le liens
            if (Ci != Cj)
            {
                add_link(class_links, Ci, Cj);
            }
        }
    }

    // Enleve les redonances
    removeTransitiveLinks(class_links);

    return class_array;
}

int StatFunction(class_array, num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        class_array[i];
    }
}