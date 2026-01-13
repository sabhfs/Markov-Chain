#include "utils.h"

char *getID(int i) {
    char *buffer = malloc(10 * sizeof(char));
    char temp[10];
    int index = 0;
    i--;
 
    while (i >= 0) {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }
 
    for (int j = 0; j < index; j++) {
        buffer[j] = temp[index - j - 1];
    }
 
    buffer[index] = '\0';
    return buffer;
}

t_cell *create_cell(int dest, float proba){
    t_cell *new_cell;
    new_cell = (t_cell *) malloc(sizeof(t_cell));
    new_cell->dest = dest;
    new_cell->proba = proba;
    new_cell->next = NULL;
    return new_cell;
}

t_list create_empty_list(){
    t_list list;
    list.head = NULL;
    return list;
}

void list_add_front(t_list *L, int dest, float proba){
    t_cell *new_cell = create_cell(dest, proba);
    new_cell->next = L->head;
    L->head = new_cell;
   return;
}

void display_list(t_list L) {
   t_cell *curr;
   curr = L.head;

   printf("[head @] ");
   
   if (curr == NULL){
        return;
    }

   while (curr != NULL) {   
        if (curr == L.head) {
            printf(" -> (%d , %.2f) ", curr->dest+1, curr->proba);
            curr = curr->next;
        } else {
            printf(" @ -> (%d , %.2f) ", curr->dest+1, curr->proba);
            curr = curr->next;
        }
   }
}

t_adj_list create_empty_adjlist(int length){
    t_adj_list adj_list;
    adj_list.length = length;
    adj_list.list = malloc(length * sizeof(t_list));

    for (int i = 0; i < length; i++) {
        adj_list.list[i] = create_empty_list();
    }

    return adj_list;
}

t_adj_list read_graph(const char *filename) {
 
    FILE* file = fopen(filename, "rt");
    int nbvert, depart, arrivee;
    float proba;
 
    if (file == NULL) {
 
        perror("Le fichier n'a pas pu etre ouvert");
        exit(EXIT_FAILURE);
    }
 
    if (fscanf(file, "%d", &nbvert) != 1) {
 
        perror("Erreur de lecture du nombre de sommets");
        fclose(file);
        exit(EXIT_FAILURE);
    }
 
    t_adj_list adj_list = create_empty_adjlist(nbvert);
 
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
 
        depart -= 1;
        arrivee -= 1;
 
        t_cell *new_cell = create_cell(arrivee, proba);
 
        new_cell->next = adj_list.list[depart].head;
        adj_list.list[depart].head = new_cell;
    }
 
    fclose(file);
    return adj_list;
}

void print_adjlist(t_adj_list adj_list) {
    if (adj_list.list == NULL) {
        printf("La liste est nulle.\n");
        return;
    }

    for (int i = 0; i < adj_list.length; i++) {
        printf("Liste pour le sommet %d  :", i+1);
        display_list(adj_list.list[i]);
        printf("\n");
    }  
    printf("\n");
}

void report_markov(t_adj_list adj_list){

    for(int i=0; i < adj_list.length; i++) {
        
        double sum = 0;
        t_cell *curr = adj_list.list[i].head;

        while(curr !=NULL ) {
            sum += curr->proba;
            curr = curr->next;
        }

        if (sum < 0.99 || sum > 1.01){
            printf("Ce n'est pas un graph de markov.\n");
            printf("Sommet %d : somme = %.4f (≠ 1)\n", i+1, sum );
            return;
        }
    }

    printf("C'est un graph de markov. \n");
}

void write_mermaid(t_adj_list *adj_list, FILE *file) {
    if (adj_list == NULL || adj_list->list == NULL || file == NULL) {
        fprintf(stderr, "Liste d'adjacence ou flux de fichier invalide.\n");
        return;
    }

    fprintf(file, "config:\n");
    fprintf(file, "layout: elk\n");
    fprintf(file, "theme: neo\n");
    fprintf(file, "look: neo\n");
    fprintf(file, "flowchart LR\n"); 
    
    char *id_name = NULL;

    for (int i = 1; i <= adj_list->length; i++) {
        id_name = getID(i);
        if (id_name != NULL) {
            fprintf(file, "%s((%d))\n", id_name, i); 
            free(id_name);
            id_name = NULL;
        }
    }

    for (int i = 0; i < adj_list->length; i++) {
        t_cell *curr = adj_list->list[i].head;
        
        char *from_id = getID(i + 1);
        
        if (from_id != NULL) {
            while (curr != NULL) {
                char *to_id = getID(curr->dest + 1);
                
                if (to_id != NULL) {
                    fprintf(file, "%s-->|%.2f|%s\n", 
                            from_id, curr->proba, to_id);
                    free(to_id);
                }
                curr = curr->next;
            }
            free(from_id);
        }
    }
}

    

