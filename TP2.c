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

/*!
 * \file TP2.c
 * \brief Implémentation du TP2
 * \author Corentin Calmels
 * \version 0.1
 * \date 2023-10-17
 */


/*!
 * @brief A structure representing a node in a linked list of vertices.
 * 
 */
typedef struct chainon {
    int numero_sommet; /**< The number of the vertex represented by this node. */
    struct chainon* next; /**< A pointer to the next node in the linked list. */
} chainon_t;

/*!
 * @brief Definition of a pointer to a chainon_t struct, used as a type for a linked list.
 * 
 */
typedef chainon_t* liste_t;

/*!
 * @file TP2.c
 * @brief This file contains the definition of the graphe_t struct and its components.
 */
typedef struct {
    int nbr_sommets;
    liste_t *listes;
} graphe_t;

/*!
 * @brief Definition of a structure representing an edge between two vertices.
 * 
 */
typedef struct { 
    int sommet1; /**< The first vertex of the edge. */
    int sommet2; /**< The second vertex of the edge. */
} arete_t;

/*!
 * @brief A struct that contains a key of type arete_t.
 * 
 */
typedef struct { 
    arete_t key; 
} key_tt;

/*!
 * @brief Adds a new node to the beginning of a linked list.
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

/*!
 * @brief Frees the memory allocated for a linked list and sets the pointer to NULL.
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

/*!
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

/*!
 * @brief Calculates the length of a linked list.
 * @param liste The linked list to calculate the length of.
 * @return The length of the linked list.
 */
int longueur_liste(liste_t liste){
    while (liste != NULL) {
        return 1 + longueur_liste(liste->next);
    }   
    return 0;
}

/*!
 * @brief Checks if a graph contains a loop.
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

/*!
 * @brief Check if a graph is symmetric.
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

/*!
 * @brief A structure representing a stack data structure.
 * 
 */
typedef struct pile{
    int *elements; /**< An array of integers representing the elements in the stack. */
    int taille; /**< The maximum size of the stack. */
    int sommet; /**< The index of the top element in the stack. */
} pile_t;

/**
 * Pushes an element onto the top of the stack.
 * 
 * @param p Pointer to the stack structure.
 * @param element The element to be pushed onto the stack.
 */
void empiler(struct pile *p, int element) {
    if (p->sommet == p->taille) {
        return;
    }
    p->elements[p->sommet] = element;
    p->sommet++;
}

/**
 * @brief Removes and returns the top element of the stack.
 * 
 * @param p Pointer to the stack structure.
 * @return int The top element of the stack, or -1 if the stack is empty.
 */
int depiler(struct pile *p) {
  if (p->sommet == 0) {
    return -1;
  }
  p->sommet--;
  return p->elements[p->sommet];
}

/*!
 * @brief Writes the graph in Graphviz format to a file.
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

/*!
 * @brief Writes a graph in Graphviz format to a file.
 * 
 * @param f The file to write the graph to.
 * @param graphe The graph to write.
 * @param couleurs The colors of the nodes.
 * @param pin Whether or not to pin the nodes.
 */
void write_graphviz3(FILE *f, graphe_t graphe, int *couleurs, bool pin) {
    char *pinstr = pin ? "true" : "false";
    fprintf(f, "graph G {\nlayout = fdp;\nnode [shape=point, width=.4, colorscheme=paired12];\nedge [width=.4, colorscheme=paired12, penwidth=4];\n");

    int size = sqrt(graphe.nbr_sommets);
    int pos_x, pos_y;
    for (int i = 0; i < graphe.nbr_sommets; i++) {
        pos_x = i % size;
        pos_y = i / size;
        fprintf(f, "%d [pos=\"%d,%d\", pin=%s, color=%d];\n", i, pos_x, pos_y, pinstr, couleurs[i]);
    }

    chainon_t *chainon;
    for (int i = 0; i < graphe.nbr_sommets; i++) {
        chainon = graphe.listes[i];
        while (chainon != NULL && i < chainon->numero_sommet) {
            fprintf(f, "%d -- %d [color=%d];\n", i, chainon->numero_sommet, couleurs[i]);
            chainon = chainon->next;
        }
    }

    fprintf(f, "}\n");
}

/*!
 * @brief Generates a graph with a given size and probability of edges between vertices.
 * 
 * @param size The size of the graph (number of vertices).
 * @param probability The probability of an edge between two vertices being created.
 * @return A graph with the given size and probability of edges.
 */
graphe_t exo_coloration_step1(int size, double probability, bool diagonal){
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
            if (diagonal && j+1 < size && i+1 < size) {
                if (rand() < probability * ((double)RAND_MAX + 1.0)) {
                    matrice_adjacence[matrice[i][j]][matrice[i+1][j+1]] = 1;
                    matrice_adjacence[matrice[i+1][j+1]][matrice[i][j]] = 1;
                }
            }
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

/*!
 * @brief Affiche les éléments d'une pile.
 * @param pile La pile à afficher.
 */
void afficher_pile(pile_t pile) {
    for (int i = 0; i < pile.sommet; i++) {
        printf("%d ", pile.elements[i]);
    }
    printf("\n");
}


/*!
 * @brief This function performs the second step of graph coloring algorithm.
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

/*!
 * @brief Calculates the average number of colors used to color a graph of a given size and probability.
 * 
 * @param size The number of vertices in the graph.
 * @param probability The probability of an edge existing between two vertices.
 * @return The average number of colors used to color the graph over 100 iterations.
 */
double moyenne_couleur(int size, double probability, int k, bool diagonal) {
    int nbr_couleurs_total = 0;
    for (int i = 0; i < k; i++) {
        graphe_t graphe = exo_coloration_step1(size, probability, diagonal);
        int *couleurs = malloc(graphe.nbr_sommets * sizeof(int));
        int nbr_couleurs = exo_coloration_step2(graphe, couleurs);
        nbr_couleurs_total += nbr_couleurs;
        detruire_graphe(&graphe);
        free(couleurs);
    }
    return (double) nbr_couleurs_total/k;
}

/*!
 * @brief Calculates the optimal probability for a given graph size and number of colors.
 * @param size The size of the graph.
 * @param k The number of colors.
 * @return The optimal probability.
 */
double prob_optimale(int size, int k, bool diagonal) {
    double proba = 0.5;
    double proba_min = 0;
    double proba_max = 1;
    double moyenne = moyenne_couleur(size, proba, 100, diagonal);
    while (moyenne > k + 0.0001 || moyenne < k - 0.0001) {
        if (moyenne < k) {
            proba_max = proba;
            proba = (proba + proba_min) / 2;
        } else {
            proba_min = proba;
            proba = (proba + proba_max) / 2;
        }
        moyenne = moyenne_couleur(size, proba, 100, diagonal);
    }
    return proba;
}

/**
 * @brief Display the help message for the program.
 * 
 * Usage: ./prog [type d'éxecution] [taille du carré] [diagonale] [option conditionnelle] ...
 * 
 * Types d'éxecution :
 * 
 * 1 : Génération d'un graphe aléatoire et coloration:
 *     ./prog 1 [taille grille] [diagonale(true/false)] [probabilité] [ping(true/false)]
 * 
 * 2 : Moyenne du nombre de parties connexes pour une taille de grille et une probabilité données
 *     ./prog 2 [taille grille] [diagonale(true/false)] [probabilité]
 * 
 * 3 : Probabilité optimale pour une taille de grille et un nombre de couleurs donnés
 *     ./prog 3 [taille grille] [diagonale(true/false)] [nombre de couleurs]
 */
void display_help(){
    printf("Usage: ./prog [type d'éxecution] [taille du carré] [diagonale] [option conditionnelle] ...\n");
    printf("Types d'éxecution :\n");
    printf("\n1 : Génération d'un graphe aléatoire et coloration:\n");
    printf("    ./prog 1 [taille grille] [diagonale(true/false)] [probabilité] [ping(true/false)]\n");
    printf("\n2 : Moyenne du nombre de parties connexes pour une taille de grille et une probabilité données\n");
    printf("    ./prog 2 [taille grille] [diagonale(true/false)] [probabilité]\n");
    printf("\n3 : Probabilité optimale pour une taille de grille et un nombre de couleurs donnés\n");
    printf("    ./prog 3 [taille grille] [diagonale(true/false)] [nombre de couleurs]\n");
}

/*!
 * @brief The main function of the program.
 * It takes command line arguments and executes different types of operations based on the arguments.
 * 
 * @param argc The number of command line arguments.
 * @param argv The array of command line arguments.
 * @return 0 if the program executes successfully, 1 otherwise.
 */
int main(int argc, char *argv[]) {

    if (argc < 2 || strcmp("--help", argv[1]) == 0) {
        display_help();
        return 0;
    }

    switch (atoi(argv[1])) {
        case 1: {
            if (argc < 4) {
                display_help();
                return 1;
            }
            int size_int = atoi(argv[2]);
            double probability_double = atof(argv[4]);
            bool diagonal = false;
            bool pin = false;
            if (strcmp("true", argv[3]) == 0) diagonal = true;
            if (strcmp("true",argv[5])==0) pin = true;
            else diagonal = false;
            graphe_t graphe = exo_coloration_step1(size_int, probability_double, diagonal);
            int *couleurs = calloc(graphe.nbr_sommets, sizeof(int));
            int nbr_couleurs = exo_coloration_step2(graphe, couleurs);
            FILE *f = fopen("exemple2.dot", "w");   
            write_graphviz3(f, graphe, couleurs, pin);
            fclose(f);
            detruire_graphe(&graphe);
            free(couleurs);
            fprintf(stdout,"\n Nombre de couleur : %d\n",nbr_couleurs);
            fprintf(stdout,"\n make display pour afficher le graphe\n");
            return 0;
        }
        case 2: {
            if (argc < 4) {
                display_help();
                return 1;
            }
            int size_int = atoi(argv[2]);
            double probability_double = atof(argv[4]);
            bool diagonal;
            if (strcmp("true", argv[3]) == 0) diagonal = true;
            else diagonal = false;
            fprintf(stdout, "\n La moyenne du nombre de parties connexes pour une grille de %d*%d et un probabilité de %f : %f\n",size_int, size_int, probability_double, moyenne_couleur(size_int, probability_double,1000, diagonal));
            return 0;
        }
        case 3: {
            if (argc < 4) {
                display_help();
                return 1;
            }
            int size_int = atoi(argv[2]);
            int k_int = atoi(argv[4]);
            bool diagonal;
            if (strcmp("true", argv[3]) == 0) diagonal = true;
            else diagonal = false;
            fprintf(stdout, "\nLa probailité optimale pour avoir %d parties connexes à partir d'une grille %d*%d est : %f\n",k_int,size_int,size_int,prob_optimale(size_int, k_int, diagonal));
            return 0;
        }
        default: {
            display_help();
            return 1;
        }
    }
}
 