#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "utils.h"   
#include "hasse.h"    

// Structure représentant une matrice carrée de nombres flottants
typedef struct s_matrix {
    float **data;           // Données de la matrice (tableau 2D)
    int size;               // Taille de la matrice (pour une matrice carrée)
} t_matrix;

/**
 * @brief Crée une matrice vide (initialisée à 0) de taille size x size.
 * @param size La dimension de la matrice carrée.
 * @return La structure matrice allouée.
 */
t_matrix create_empty_matrix(int size);

/**
 * @brief Copie le contenu d'une matrice source vers une matrice destination.
 * @param dest La matrice de destination (doit être déjà allouée).
 * @param src La matrice source.
 */
void copy_matrix(t_matrix dest_matrix, t_matrix from_matrix);

/**
 * @brief Libère la mémoire allouée pour une matrice.
 * @param matrix La matrice à libérer.
 */
void free_matrix(t_matrix matrix);

/**
 * @brief Affiche le contenu de la matrice dans la console.
 * @param matrix La matrice à afficher.
 */
void display_matrix(t_matrix matrix);

/**
 * @brief Convertit le graphe (liste d'adjacence) en matrice de transition.
 * @param graph Pointeur vers le graphe.
 * @return La matrice de transition correspondante.
 */
t_matrix create_matrix_from_graph(t_adj_list *graph);

/**
 * @brief Multiplie deux matrices carrées (A * B).
 * @param A Première matrice.
 * @param B Deuxième matrice.
 * @return Une nouvelle matrice contenant le résultat.
 */
t_matrix multiply_matrices(t_matrix matrix_A, t_matrix matrix_B);

/**
 * @brief Calcule la différence absolue totale entre deux matrices.
 * @param A Première matrice.
 * @param B Deuxième matrice.
 * @return La somme des différences absolues élément par élément.
 */
float diff_matrices(t_matrix matrix_A, t_matrix matrix_B);

/**
 * @brief Calcule la puissance p-ième d'une matrice (M^p).
 * @param M La matrice de base.
 * @param p La puissance.
 * @return Une nouvelle matrice contenant le résultat.
 */
t_matrix power_matrix(t_matrix matrix, int p);

/**
 * @brief Cherche la matrice stationnaire (limite) par itération.
 * @param M La matrice de transition initiale.
 * @param epsilon Le seuil de convergence (différence minimale).
 * @return La matrice limite approximée.
 */
t_matrix find_stationary_matrix(t_matrix matrix, float epsilon);

/**
 * @brief Extrait une sous-matrice correspondant aux sommets d'une classe donnée.
 * @param matrix La matrice globale du graphe.
 * @param partition Pointeur vers la partition contenant les classes.
 * @param class_index L'index de la classe à extraire.
 * @return Une nouvelle matrice carrée correspondant à la classe.
 */
t_matrix create_sub_matrix(t_matrix matrix, t_partition *partition, int class_index);

/**
 * @brief Calcule le Plus Grand Commun Diviseur de deux entiers.
 * @param a Premier entier.
 * @param b Deuxième entier.
 * @return Le PGCD de a et b.
 */
int gcd(int x, int y);

/**
 * @brief Calcule la période d'une sous-matrice (classe).
 * @param sub_matrix La sous-matrice de la classe.
 * @return La période (d) de la classe.
 */
int get_period(t_matrix sub_matrix);

#endif // __MATRIX_H__