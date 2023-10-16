#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define BUFSIZE 65536

/**
 * @brief A structure representing a node in a linked list of vertices.
 * 
 */
typedef struct chainon {
    int numero_sommet; /**< The number of the vertex represented by this node. */
    struct chainon* next; /**< A pointer to the next node in the linked list. */
} chainon_t;

/**
 * @brief Definition of a pointer to a chainon_t struct, used as a type for a linked list.
 * 
 */
typedef chainon_t* liste_t;

/**
 * @file TP2.c
 * @brief This file contains the definition of the graphe_t struct and its components.
 */
typedef struct {
    int nbr_sommets;
    liste_t *listes;
} graphe_t;

/**
 * @brief Definition of a structure representing an edge between two vertices.
 * 
 */
typedef struct { 
    int sommet1; /**< The first vertex of the edge. */
    int sommet2; /**< The second vertex of the edge. */
} arete_t;

/**
 * @brief A struct that contains a key of type arete_t.
 * 
 */
typedef struct { 
    arete_t key; 
} key_tt;

/**
 * Adds a new node to the beginning of a linked list.
 * 
 * @param numero_sommet The number of the vertex to be added to the linked list.
 * @param liste A pointer to the linked list.
 * 
 * @return A pointer to the newly added node.
 */
chainon_t * ajouter_chainon(int numero_sommet, liste_t *liste) {
    chainon_t *nouveau_chainon = malloc(sizeof(chainon_t));
    if (nouveau_chainon == NULL) return NULL;
    nouveau_chainon->numero_sommet = numero_sommet;
    nouveau_chainon->next = *liste;
    *liste = nouveau_chainon;
    return nouveau_chainon;
}

/**
 * Frees the memory allocated for a linked list and sets the pointer to NULL.
 * 
 * @param liste A pointer to the head of the linked list.
 */
void detruire_liste(liste_t *liste) {
    chainon_t *chainon = *liste;
    while (chainon != NULL) {
        chainon_t *suivant = chainon->next;
        free(chainon);
        chainon = suivant;
    }
    *liste = NULL;
}

/**
 * @brief Frees the memory allocated for a graph and its adjacency lists.
 * 
 * @param graphe Pointer to the graph to be destroyed.
 */
void detruire_graphe(graphe_t *graphe){
    for (int i = 0; i < graphe->nbr_sommets; i++) {
        if (graphe->listes[i] != NULL){
            detruire_liste(&graphe->listes[i]);
        }
    }
    free(graphe->listes);
    graphe->listes = NULL;
    graphe->nbr_sommets = 0;
}

/**
 * Calculates the length of a linked list.
 * @param liste The linked list to calculate the length of.
 * @return The length of the linked list.
 */
int longueur_liste(liste_t liste){
    while (liste != NULL) {
        return 1 + longueur_liste(liste->next);
    }   
    return 0;
}

/**
 * Checks if a graph contains a loop.
 * 
 * @param graphe The graph to check.
 * @return True if the graph contains a loop, false otherwise.
 */
bool contient_boucle(graphe_t graphe){
    for (int i = 0; i < graphe.nbr_sommets; i++) {
        chainon_t *chainon = graphe.listes[i];
        while (chainon != NULL) {
            if (chainon->numero_sommet == i) return true;
            chainon = chainon->next;
        }
    }
    return false;
}

/**
 * Check if a graph is symmetric.
 * A graph is symmetric if for every edge (u, v), there exists an edge (v, u).
 * 
 * @param graphe The graph to check.
 * @return true if the graph is symmetric, false otherwise.
 */
bool est_symetrique(graphe_t graphe){
    assert(!contient_boucle(graphe));
    key_tt *hash = NULL;
    for (int i = 0; i < graphe.nbr_sommets; i++) {
        chainon_t *chainon = graphe.listes[i];
        while (chainon != NULL) {
            arete_t k = {i, chainon->numero_sommet};
            hmputs(hash, (key_tt) {k});
            chainon = chainon->next;
        }
    }

    for(int i=0; i < hmlen(hash); i++) {
        arete_t k = hash[i].key;
        if (hmgeti(hash, ((arete_t) { k.sommet2, k.sommet1 })) == -1){
            hmfree(hash);
            return false;
        }
    }
    
    hmfree(hash);
    return true;
}

/**
 * @brief A structure representing a stack data structure.
 * 
 */
typedef struct pile{
    int *elements; /**< An array of integers representing the elements in the stack. */
    int taille; /**< The maximum size of the stack. */
    int sommet; /**< The index of the top element in the stack. */
} pile_t;

/**
 * Empile un élément sur la pile.
 * 
 * @param p Pointeur vers la pile.
 * @param element L'élément à empiler.
 */
void empiler(struct pile *p, int element) {
    if (p->sommet == p->taille) {
        return;
    }
    p->elements[p->sommet] = element;
    p->sommet++;
}

/**
 * @brief Dépile un élément de la pile.
 * 
 * @param p Pointeur vers la pile à dépiler.
 * @return int L'élément dépilé, ou -1 si la pile est vide.
 */
int depiler(struct pile *p) {
  if (p->sommet == 0) {
    // La pile est vide
    return -1;
  }

  p->sommet--;
  return p->elements[p->sommet];
}

/**
 * Writes the graph in Graphviz format to a file.
 * 
 * @param f The file to write the graph to.
 * @param graphe The graph to write.
 */
void write_graphviz2(FILE *f, graphe_t graphe) {
    fprintf(f, "graph G {\n");
    fprintf(f, "layout = fdp;\n");
    fprintf(f, "node  [shape=point, width=.4, colorscheme=paired12 ];\n");
    fprintf(f, "edge [ width=.4, colorscheme=paired12, penwidth=4 ];\n");

    int size = sqrt(graphe.nbr_sommets);
    int x = 0;
    int y = 0;
    for (int i = 0; i < graphe.nbr_sommets; i++) {
        fprintf(f, "%d [pos=\"%d,%d\", pin=true];\n", i, x, y);
        x++;
        if (x == size) {
            x = 0;
            y++;
        }
    }

    for (int i = 0; i < graphe.nbr_sommets; i++) {
        chainon_t *chainon = graphe.listes[i];
        while (chainon != NULL) {
            if (i < chainon->numero_sommet) {
                fprintf(f, "%d -- %d;\n", i, chainon->numero_sommet);
            }
            chainon = chainon->next;
        }
    }
    fprintf(f, "}\n");
}

/**
 * Writes a graph in Graphviz format to a file.
 * 
 * @param f The file to write the graph to.
 * @param graphe The graph to write.
 * @param couleurs The colors to use for each node in the graph.
 */
void write_graphviz3(FILE *f, graphe_t graphe, int *couleurs);
void write_graphviz3(FILE *f, graphe_t graphe, int *couleurs) {
    fprintf(f, "graph G {\n");
    fprintf(f, "layout = fdp;\n");
    fprintf(f, "node  [width=.4, colorscheme=paired12 ];\n");
    fprintf(f, "edge [ width=.4, colorscheme=paired12, penwidth=4 ];\n");

    int size = sqrt(graphe.nbr_sommets);
    int x = 0;
    int y = 0;
    for (int i = 0; i < graphe.nbr_sommets; i++) {
        fprintf(f, "%d [pos=\"%d,%d\", pin=true, color=%d];\n", i, x, y, couleurs[i]);
        x++;
        if (x == size) {
            x = 0;
            y++;
        }
    }

    for (int i = 0; i < graphe.nbr_sommets; i++) {
        chainon_t *chainon = graphe.listes[i];
        while (chainon != NULL) {
            if (i < chainon->numero_sommet) {
                fprintf(f, "%d -- %d [color=%d];\n", i, chainon->numero_sommet, couleurs[i]);
            }
            chainon = chainon->next;
        }
    }
    fprintf(f, "}\n");
}

/**
 * Generates a graph with a given size and probability of edges between vertices.
 * 
 * @param size The size of the graph (number of vertices).
 * @param probability The probability of an edge between two vertices being created.
 * @return A graph with the given size and probability of edges.
 */
graphe_t exo_coloration_step1(int size, double probability){
    int matrice[size][size];
    srand(time(NULL));
    int comp = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; ++j) {
            matrice[i][j] = comp;
            comp++;
        }
    }

    int matrice_adjacence[size*size][size*size];
    for (int i = 0; i < size*size; i++) {
        for (int j = 0; j < size*size; ++j) {
            matrice_adjacence[i][j] = 0;
        }
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; ++j) { 
            if (i+1 < size) {
                if (rand() < probability * ((double)RAND_MAX + 1.0)) {
                    matrice_adjacence[matrice[i][j]][matrice[i+1][j]] = 1;
                    matrice_adjacence[matrice[i+1][j]][matrice[i][j]] = 1;
                }
            }
            if (j+1 < size) {
                if (rand() < probability * ((double)RAND_MAX + 1.0)) {
                    matrice_adjacence[matrice[i][j]][matrice[i][j+1]] = 1;
                    matrice_adjacence[matrice[i][j+1]][matrice[i][j]] = 1;
                }
            }
            /*if (j+1 < size && i+1 < size) {
                if (rand() < probability * ((double)RAND_MAX + 1.0)) {
                    matrice_adjacence[matrice[i][j]][matrice[i+1][j+1]] = 1;
                    matrice_adjacence[matrice[i+1][j+1]][matrice[i][j]] = 1;
                }
            }*/
        }
    }
    graphe_t graphe; graphe.nbr_sommets = size*size;
    graphe.listes = malloc(size*size * sizeof(liste_t));
    if (graphe.listes == NULL) { graphe.nbr_sommets = 0; return graphe; }
    for(int i = 0; i < size*size; i++) graphe.listes[i] = NULL;
    for (int i = 0; i < size*size; i++) {
        for (int j = 0; j < size*size; ++j) {
            if (matrice_adjacence[i][j] == 1) {
                if (ajouter_chainon(j, &graphe.listes[i]) == NULL) {
                    detruire_graphe(&graphe); return graphe;
                }
            }
        }
    }
    return graphe;
}

/**
 * Affiche les éléments d'une pile.
 * @param pile La pile à afficher.
 */
void afficher_pile(pile_t pile) {
    for (int i = 0; i < pile.sommet; i++) {
        printf("%d ", pile.elements[i]);
    }
    printf("\n");
}


/**
 * This function performs the second step of graph coloring algorithm.
 * It takes a graph and an array of colors as input and returns the number of colors used.
 * The function uses a stack to traverse the graph and assigns colors to each vertex.
 * The algorithm starts by marking all vertices as unvisited and initializes the color to 0.
 * It then iterates over all unvisited vertices and assigns a new color to each connected component.
 * The function returns the number of colors used.
 *
 * @param graphe The graph to be colored
 * @param couleurs The array of colors to be assigned to each vertex
 * @return The number of colors used
 */
int exo_coloration_step2(graphe_t graphe, int *couleurs){
    assert(est_symetrique(graphe));
    bool marquage[graphe.nbr_sommets];
    memset(marquage, false, sizeof(marquage));
    int couleur = 0;
    if (couleurs == NULL) return 0;
    pile_t pile; pile.taille = graphe.nbr_sommets; pile.sommet = 0;
    pile.elements = malloc(graphe.nbr_sommets * sizeof(int));
    for (int i = 0; i < graphe.nbr_sommets; i++) {
        if (!marquage[i]) {
            pile.sommet = 0;
            empiler(&pile, i);
            couleur = couleur + 1;
            while (pile.sommet != 0) {
                int sommet = depiler(&pile);
                if (!marquage[sommet]) {
                    marquage[sommet] = true;
                    couleurs[sommet] = couleur;
                    chainon_t *chainon = graphe.listes[sommet];
                    while (chainon != NULL) {
                        empiler(&pile, chainon->numero_sommet);
                        chainon = chainon->next;
                    }
                }
            }
        }
    }
    if (pile.elements != NULL) {
        free(pile.elements);
        pile.elements = NULL;
    }
    return couleur;
}

/**
 * Calculates the average number of colors used to color a graph of a given size and probability.
 * 
 * @param size The number of vertices in the graph.
 * @param probability The probability of an edge existing between two vertices.
 * @return The average number of colors used to color the graph over 100 iterations.
 */
double moyenne_couleur(int size, double probability) {
    int nbr_couleurs_total = 0;
    for (int i = 0; i < 100; i++) {
        graphe_t graphe = exo_coloration_step1(size, probability);
        int *couleurs = malloc(graphe.nbr_sommets * sizeof(int));
        int nbr_couleurs = exo_coloration_step2(graphe, couleurs);
        nbr_couleurs_total += nbr_couleurs;
        detruire_graphe(&graphe);
        free(couleurs);
    }
    return (double) nbr_couleurs_total/100;
}

/**
 * Calculates the optimal probability for a given graph size and number of colors.
 * @param size The size of the graph.
 * @param k The number of colors.
 * @return The optimal probability.
 */
double prob_optimale(int size, int k) {
    double proba = 0.5;
    double proba_min = 0;
    double proba_max = 1;
    double moyenne = moyenne_couleur(size, proba);
    while (moyenne > k + 0.0001 || moyenne < k - 0.0001) {
        fprintf(stdout, "%f\n", moyenne);
        fprintf(stdout, "%f\n", proba);
        if (moyenne < k) {
            proba_max = proba;
            proba = (proba + proba_min) / 2;
        } else {
            proba_min = proba;
            proba = (proba + proba_max) / 2;
        }
        moyenne = moyenne_couleur(size, proba);
    }
    return proba;
}


int mainsave(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: %s [taille du carré] [probabilité] ...\n", argv[0]);
        return 1;
    }
    char *size = argv[1];
    char *probability = argv[2];
    int size_int = atoi(size);
    double probability_double = atof(probability);

    graphe_t graphe = exo_coloration_step1(size_int, probability_double);
    int *couleurs = malloc(graphe.nbr_sommets * sizeof(int));
    int nbr_couleurs = exo_coloration_step2(graphe, couleurs);
    fprintf(stderr, "nbr_couleurs = %d\n", nbr_couleurs);
    FILE *f = fopen("exemple2.dot", "w");
    write_graphviz3(f, graphe, couleurs);
    fclose(f);
    detruire_graphe(&graphe);
    free(couleurs);
    
}

int mainsave2(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [taille du carré] [probabilité] ...\n", argv[0]);
        return 1;
    }
    char *size = argv[1];
    char *probability = argv[2];
    int size_int = atoi(size);
    double probability_double = atof(probability);
    fprintf(stdout, "%f",moyenne_couleur(size_int, probability_double));
    fprintf(stdout, "%f",prob_optimale(size_int, 2));
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [taille du carré] [probabilité] ...\n", argv[0]);
        return 1;
    }
    char *size = argv[1];
    char *probability = argv[2];
    int size_int = atoi(size);
    double probability_double = atof(probability);
    graphe_t graphe = exo_coloration_step1(size_int, probability_double);
    int *couleurs = malloc(graphe.nbr_sommets * sizeof(int));
    int nbr_couleurs = exo_coloration_step2(graphe, couleurs);
    fprintf(stderr, "nbr_couleurs = %d\n", nbr_couleurs);
    FILE *f = fopen("exemple2.dot", "w");
    write_graphviz3(f, graphe, couleurs);
    fclose(f);
    detruire_graphe(&graphe);
    free(couleurs);
    return 0;
    
}
 