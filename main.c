#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "hasse.h"

int main() {

    // PARTIE 1 : CHARGEMENT ET VERIFICATION

    t_adj_list graph = read_graph("data/exemple1.txt"); 
    
    printf("--- Contenu du Graphe ---\n");
    print_adjlist(graph);
    
    printf("--- Verification Markov ---\n");
    report_markov(graph);

    FILE *filename = fopen("graphe_complet.mmd", "w");
    write_mermaid(&graph, filename);
    fclose(filename);
    printf("\n Fichier 'graphe_complet.mmd' genere.\n");


    // PARTIE 2 : TARJAN & HASSE

    printf("\n Algorithme de Tarjan...\n");
    t_partition partition = tarjan(&graph); 
    display_partition(partition);

    printf("\n Diagramme de Hasse & Analyse...\n");

    int *class_map = create_class_map(&partition, graph.length);
    t_link_array links = find_class_links(&graph, &partition, class_map);

    printf("\n Suppression des liens transitifs...\n");
    remove_transitive_links(&links);

    printf("\n Analyse des proprietes de Markov du diagramme de Hasse\n");
    analyze_markov_properties(&partition, &links);

    FILE *output_name = fopen("hasse_output.mmd", "w");
    write_hasse_mermaid(&partition, &links, output_name);
    fclose(output_name);
    printf("\n Fichier 'hasse_output.mmd' genere.\n");

    free(class_map);

    return 0;
}