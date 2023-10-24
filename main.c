#include <stdio.h>
#include "function.c"
/*!
 * \file main.c
 * \brief Implémentation du TP2
 * \author Corentin Calmels
 * \version 0.1
 * \date 2023-10-17
 */

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