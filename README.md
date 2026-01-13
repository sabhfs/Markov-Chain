# Markov Chain Analysis & Hasse Visualizer

Ce projet est dédiée à l'analyse structurelle et mathématique des **Chaînes de Markov**.
Il permet d'ingérer un graphe probabiliste, de décomposer ses états, de classifier ses composantes et de générer des visualisations graphiques via Mermaid.

## Fonctionnalités Clés

### 1. Analyse Topologique (Théorie des Graphes)
* **Algorithme de Tarjan** : Implémentation efficace pour identifier les **Composantes Fortement Connexes (CFC)** ou "Classes".
* **Diagramme de Hasse** : Construction de la hiérarchie des classes en supprimant les liens transitifs redondants.
* **Classification des États** :
    * *Transitoires* (on peut les quitter sans retour possible).
    * *Persistantes / Récurrentes* (une fois dedans, on y reste).
    * *Absorbantes* (états puits).
    * *Irréductibilité* du graphe complet.

### 2. Algèbre Linéaire (Calcul Matriciel)
* **Matrices de Transition** : Conversion Graphe -> Matrice.
* **Distribution Stationnaire** : Calcul de la convergence (état stable) par itération de puissances ($M^n$) ou méthodes limites.
* **Périodicité** : Calcul du PGCD des longueurs de cycles pour déterminer la période d'une classe.

## Architecture du Code

* **`main.c`** : Charge le graphe, lance Tarjan, analyse les propriétés et exporte les résultats.
* **`hasse.c`** : Contient l'implémentation de **Tarjan**, la gestion des piles (`stack`), et la logique de réduction transitive pour le diagramme de Hasse.
* **`matrix.c`** : Gestion dynamique de matrices, multiplication, calcul de convergence et périodicité.
* **`utils.c`** : Gestion basique du graphe.

