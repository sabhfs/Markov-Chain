#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure d'une arête 
typedef struct s_cell {
    int dest;               // Sommet de destination
    float proba;            // Poid de l'arrête (probabilité)
    struct s_cell *next;    // Pointeur vers l'arête suivante
} t_cell;

// Structure de la liste des voisins d'un sommet
typedef struct s_list {
    t_cell *head;          // Pointeur vers la tête de la liste chaînée
} t_list;

// Structure principale du graphe, liste d'adjacence
typedef struct s_adj_list {
    int length;            // Nombre total de sommets
    t_list *list;          // Tableau des listes d'adjacence
} t_adj_list;

/**
 * @brief Convertit un ID numérique en nom (ex: 1 -> "A", 2 -> "B").
 * @param i L'identifiant entier du sommet.
 * @return Une chaîne de caractères allouée dynamiquement contenant le nom.
 */
char *getID(int);

/**
 * @brief Crée une nouvelle cellule (arête) pour la liste chaînée.
 * @param dest L'index du sommet de destination.
 * @param proba La probabilité de transition.
 * @return Un pointeur vers la nouvelle cellule allouée.
 */
t_cell *create_cell(int, float);

/**
 * @brief Initialise une liste vide.
 * @return Une structure t_list avec head = NULL.
 */
t_list create_empty_list(void);

/**
 * @brief Ajoute une arête en tête d'une liste donnée.
 * @param L Pointeur vers la liste à modifier.
 * @param dest Destination de l'arête.
 * @param proba Probabilité de l'arête.
 */
void list_add_front(t_list *, int, float);

/**
 * @brief Crée et initialise une structure de graphe (liste d'adjacence).
 * @param length Le nombre de sommets du graphe.
 * @return Une structure t_adj_list avec 'length' listes vides.
 */
t_adj_list create_empty_adjlist(int);

/**
 * @brief Lit un fichier texte pour construire le graphe.
 * @param filename Le chemin du fichier contenant la description du graphe.
 * @return La structure t_adj_list complétée.
 */
t_adj_list read_graph(const char *);

/**
 * @brief Affiche le contenu du graphe (listes d'adjacence) dans la console.
 * @param adj_list Le graphe à afficher.
 */
void print_adjlist(t_adj_list );

/**
 * @brief Vérifie si le graphe respecte les propriétés de Markov (somme des probas sortantes ~ 1).
 * @param adj_list Le graphe à analyser.
 */
void report_markov(t_adj_list);

/**
 * @brief Génère la description du graphe au format Mermaid dans un fichier.
 * @param adj_list Pointeur vers le graphe.
 * @param file Pointeur vers le fichier ouvert en écriture.
 */
void write_mermaid(t_adj_list *, FILE*);

#endif