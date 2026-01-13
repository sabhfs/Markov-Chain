#ifndef HASSE_H
#define HASSE_H

#include "utils.h" 
#include <stdbool.h>

// Structure pour stocker l'état d'un sommet pendant l'algorithme de Tarjan
typedef struct s_tarjan_vertex {
    int id;                         // Identifiant du sommet  
    int index;                      // Index de découverte
    int lowlink;                    // Plus bas index accessible
    bool on_stack;                  // Indique si le sommet est actuellement sur la pile
} t_tarjan_vertex;

// Maillon de la pile (implémentation par liste chaînée)
typedef struct s_stack_cell {  
    int value;                      // Stocke l'id du sommet
    struct s_stack_cell *next;      // Pointeur vers le maillon suivant
} t_stack_cell;

// Structure de la pile
typedef struct s_stack {
    t_stack_cell *head;             // Sommet de la pile
} t_stack;

// Structure représentant une Classe (Composante Fortement Connexe)
typedef struct s_class {
    char name[10];                  // Nom de la classe (ex: "C1", "C2", ...)       
    int vertex_count;               // Nombre de sommets dans la classe
    int capacity;                   // Capacité actuelle du tableau dynamique
    int *vertex_ids;                // Tableau dynamique des ids des sommets dans la classe      
} t_classe;

// Structure représentant la partition du graphe en classes        
typedef struct s_partition {
    t_classe *classes;              // Tableau dynamique des classes
    int class_count;                // Nombre de classes dans la partition
    int capacity;                   // Capacité actuelle du tableau dynamique     
} t_partition;

// Structure représentant un lien entre deux classes
typedef struct s_link {         
    int class_from;                 // Classe source
    int class_dest;                 // Classe destination
} t_link;

// Structure représentant un tableau dynamique de liens entre classes
typedef struct s_link_array {
    t_link *links;                  // Tableau dynamique des liens 
    int link_count;                 // Nombre de liens dans le tableau  
    int capacity;                   // Capacité actuelle du tableau dynamique
} t_link_array;


/**
 * @brief Initialise le tableau d'états pour l'algorithme de Tarjan.
 * @param adj_list Pointeur vers le graphe.
 * @return Un tableau de t_tarjan_vertex initialisé.
 */
t_tarjan_vertex *create_tarjan_array(t_adj_list *graph);

/**
 * @brief Initialise une pile vide.
 * @return Une structure t_stack vide.
 */
t_stack create_stack(); 

/**
 * @brief Empile un entier (ID de sommet).
 * @param stack Pointeur vers la pile.
 * @param value La valeur à empiler.
 */
void push(t_stack *stack, int value);

/**
 * @brief Dépile et retourne la valeur au sommet.
 * @param stack Pointeur vers la pile.
 * @return La valeur dépilée, ou -1 si vide.
 */
int pop(t_stack *stack);

/**
 * @brief Vérifie si la pile est vide.
 * @param stack La pile à vérifier.
 * @return true si vide, false sinon.
 */
bool is_empty(t_stack stack); 

/**
 * @brief Libère la mémoire allouée pour la pile.
 * @param stack Pointeur vers la pile à libérer.
 */
void free_stack(t_stack *stack);

/**
 * @brief Fonction récursive principale (DFS) de l'algorithme de Tarjan.
 * @param u Index du sommet courant.
 * @param adj_list Pointeur vers le graphe.
 * @param T Tableau des états Tarjan.
 * @param S Pointeur vers la pile.
 * @param P Pointeur vers la partition en cours de construction.
 * @param timer Pointeur vers le compteur global de découverte.
 */
void parcours(int curr_vertex_index, t_adj_list *graph, t_tarjan_vertex *tarjan_array, t_stack *stack, t_partition *partition, int *timer);

/**
 * @brief Exécute l'algorithme de Tarjan pour trouver les CFC du graphe.
 * @param adj_list Pointeur vers le graphe.
 * @return La partition ontenant toutes les classes identifiées
 */
t_partition tarjan(t_adj_list *graph);

/**
 * @brief Initialise une structure de partition vide.
 * @param initial_capacity Capacité initiale du tableau de classes.
 * @return Une partition initialisée.
 */
t_partition create_partition(int initial_capacity);

/**
 * @brief Ajoute une nouvelle classe à la partition.
 * @param partition Pointeur vers la partition.
 * @param new_class La structure classe à ajouter.
 * @return L'index de la classe ajoutée.
 */
int add_class(t_partition *partition, t_classe new_class);

/**
 * @brief Ajoute un sommet à une classe existante.
 * @param s_class Pointeur vers la classe.
 * @param vertex_id L'ID du sommet à ajouter.
 */
void add_vertex_to_class(t_classe *class, int vertex_index);

/**
 * @brief Libère toute la mémoire allouée pour la partition et ses classes.
 * @param partition Pointeur vers la partition.
 */
void free_partition(t_partition *partition); 

/**
 * @brief Affiche le contenu de la partition dans la console.
 * @param partition La partition à afficher.
 */
void display_partition(t_partition partition);

/**
 * @brief Initialise un tableau de liens vide.
 * @param initial_capacity Capacité initiale.
 * @return Une structure t_link_array initialisée.
 */
t_link_array create_link_array(int initial_capacity);

/**
 * @brief Crée un tableau de mappage (Index Sommet -> Index Classe).
 * @param partition Pointeur vers la partition.
 * @param graph_length Nombre total de sommets dans le graphe.
 * @return Un tableau d'entiers où tab[i] = index de la classe du sommet i+1.
 */
int *create_class_map(t_partition *partition, int graph_length);

/**
 * @brief Ajoute un lien entre deux classes s'il n'existe pas déjà.
 * @param link_array Pointeur vers le tableau de liens.
 * @param from Index de la classe source.
 * @param dest Index de la classe destination.
 */
void add_link(t_link_array *link_array, int from, int dest); 

/**
 * @brief Identifie tous les liens entre les classes du graphe.
 * @param adj_list Pointeur vers le graphe original.
 * @param partition Pointeur vers la partition.
 * @param class_map Tableau de mappage sommet->classe.
 * @return La structure contenant tous les liens trouvés.
 */
t_link_array find_class_links(t_adj_list *adj_list, t_partition *partition, int *class_map);

/**
 * @brief Supprime les liens transitifs (redondants) dans le tableau de liens.
 * @param p_link_array Pointeur vers le tableau de liens.
 */
void remove_transitive_links(t_link_array *link_array); 

/**
 * @brief Génère le code Mermaid pour le diagramme de Hasse.
 * @param partition Pointeur vers la partition (pour les noms).
 * @param link_array Pointeur vers les liens (pour les flèches).
 * @param file Fichier ouvert en écriture.
 */
void write_hasse_mermaid(t_partition *partition, t_link_array *link_array, FILE *file);

/**
 * @brief Analyse et affiche les propriétés (Transitoire, Persistant, Absorbant, Irréductible).
 * @param partition Pointeur vers la partition.
 * @param link_array Pointeur vers les liens inter-classes.
 */
void analyze_markov_properties(t_partition *partition, t_link_array *link_array);

#endif // HASSE_H