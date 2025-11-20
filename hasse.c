#include <malloc.h>
#include "hasse.h"


void removeTransitiveLinks(t_link_array *p_link_array)
//enlève des liens en trop

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
                if (link1.dept_nb == link2.dept_nb)
                {
                    // look for a link from link2.to to link1.to
                    int k = 0;
                    while (k < p_link_array->log_size && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = p_link_array->links[k];
                            if ((link3.dept_nb == link2.dest_nb) && (link3.dest_nb == link1.dest_nb))
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

int *createArrayClass(int nb_vertex, t_link_array * array)
//fonction qui permet de créer un tableau qui indique la classe de chaque sommet

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

void addLink(t_link_array *link_array,int dept,int dest) {
    //fonction qui permet de créer les connections du diagramme de hasse

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

int createClassLinks(int num_vertices,int **adj_list,int *adj_size,t_link_array *class_links)
//fonction qui permet de créer le diagramme de hasse

{
    // Cree le tableau de classe
    int *class_array = createArrayClass(num_vertices, class_links);

    //
    for (int i = 0; i < num_vertices; i++)
    {
        int Ci = class_array[i];

        //
        for (int k = 0; k < adj_size[i]; k++)
        {
            int j = adj_list[i][k];
            int Cj = class_array[j];

            // classe différente, on ajoute le lien
            if (Ci != Cj)
            {
                addLink(class_links, Ci, Cj);
            }
        }
    }

    // Enlève les redondances
    removeTransitiveLinks(class_links);

    return *class_array;
}

// Définition des caractéristiques du graph

int* ClassType(int* class_array,int num_vertices,t_link_array class_links) {
    int *type_array = malloc(num_vertices * sizeof(int));

    for (int i = 0; i < num_vertices; i++) {
        type_array[i] = 0; // Initialisation à 0 = absorbant
    }

    // Parcours des liens
    for (int i = 0; i < class_links.log_size; i++) {
        int dept = class_links.links[i].dept_nb;
        int dest = class_links.links[i].dest_nb;

        if (dept != dest) {
            type_array[dest] = 1; // 1 = transitoire
        }

    }
    return type_array;
}

int isAbsorbingState(int* class_sizes,int class_nb,int* type_array) {
    // Vérifie si une classe est absorbante
    for (int i = 0; i < class_sizes[class_nb]; i++) {
        if (type_array[i] != 0) {
            return 0; // Pas absorbant
        }
    }
    return 1; // Absorbant
}

int isIrreductible(int* class_array,int num_vertices) {
    // Vérifie si le graphe est irréductible
    int first_class = class_array[0];
    for (int i = 1; i < num_vertices; i++) {
        if (class_array[i] != first_class) {
            return 0; // Pas irréductible
        }
    }
    return 1; // Irréductible
}