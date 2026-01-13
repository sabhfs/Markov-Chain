#include "hasse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

t_stack create_stack() {
    t_stack new_stack;
    new_stack.head = NULL;
    return new_stack;
}

void push(t_stack *stack, int value) {
    t_stack_cell *new_cell = malloc(sizeof(t_stack_cell));
    if (new_cell == NULL) exit(EXIT_FAILURE);

    new_cell->value = value;
    new_cell->next = stack->head;
    stack->head = new_cell;
}

int pop(t_stack *stack) {
    if (stack->head == NULL) return -1;

    t_stack_cell *temp = stack->head;
    int value = temp->value;
    stack->head = stack->head->next;
    free(temp);
    return value;
}

bool is_empty(t_stack stack) {
    return stack.head == NULL;
}

void free_stack(t_stack *stack) {
    t_stack_cell *temp = stack->head;
    t_stack_cell *next = NULL;

    while (temp != NULL) {
        next = temp->next;
        free(temp);
        temp = next;
    }

    stack->head = NULL;
}

t_tarjan_vertex *create_tarjan_array(t_adj_list *graph) {
    t_tarjan_vertex *new_tarjan_array = malloc(graph->length * sizeof(t_tarjan_vertex));
    if (new_tarjan_array == NULL) return NULL;

    for (int i = 0; i < graph->length; i++) {
        new_tarjan_array[i].id = i + 1;         
        new_tarjan_array[i].index = -1;          
        new_tarjan_array[i].lowlink = -1;          
        new_tarjan_array[i].on_stack = false;
    }

    return new_tarjan_array;
}

t_partition create_partition(int initial_capacity) {
    t_partition new_partition;
    new_partition.classes = malloc(initial_capacity * sizeof(t_classe));
    if (new_partition.classes == NULL) exit(EXIT_FAILURE);

    new_partition.class_count = 0;
    new_partition.capacity = initial_capacity;
    return new_partition;
}

int add_class(t_partition *partition, t_classe new_class) {
    if (partition->class_count >= partition->capacity) {
        partition->capacity *= 16;

        partition->classes = realloc(partition->classes, partition->capacity * sizeof(t_classe));
        if (partition->classes == NULL) exit(EXIT_FAILURE);
    }
    
    sprintf(new_class.name, "C%d", partition->class_count + 1);
    
    partition->classes[partition->class_count] = new_class;
    return partition->class_count++;
}

void add_vertex_to_class(t_classe *class, int vertex_id) {
    if (class->vertex_ids == NULL) {
        class->capacity = 2; 
        class->vertex_ids = malloc(class->capacity * sizeof(int));
        if (class->vertex_ids == NULL) exit(EXIT_FAILURE);
    }
    
    if (class->vertex_count >= class->capacity) {
        class->capacity *= 2;
        class->vertex_ids = realloc(class->vertex_ids, class->capacity * sizeof(int));
        if (class->vertex_ids == NULL) exit(EXIT_FAILURE); 
    }
    
    class->vertex_ids[class->vertex_count++] = vertex_id;
} 

void free_partition(t_partition *partition) {
    for (int i = 0; i < partition->class_count; i++) {
        if (partition->classes[i].vertex_ids != NULL) {
            free(partition->classes[i].vertex_ids);
        }
    }

    if (partition->classes != NULL) free(partition->classes);
    partition->classes = NULL;
    partition->class_count = 0;
}

void display_partition(t_partition partition) {
    for (int i = 0; i < partition.class_count; i++) {
        printf("Composante %s: {", partition.classes[i].name);

        for (int j = 0; j < partition.classes[i].vertex_count; j++) {
            printf("%d", partition.classes[i].vertex_ids[j]);
            if (j < partition.classes[i].vertex_count - 1) printf(", ");
        }
        printf("}\n");
    }
}

void parcours(int curr_vertex_index, t_adj_list *graph, t_tarjan_vertex tarjan_array[], t_stack *stack, t_partition *partition, int *timer) {
    tarjan_array[curr_vertex_index].index = tarjan_array[curr_vertex_index].lowlink = ++(*timer);
    push(stack, curr_vertex_index);
    tarjan_array[curr_vertex_index].on_stack = true;

    t_list temporary_list = graph->list[curr_vertex_index]; 
    t_cell *temp_edge = temporary_list.head;
    
    while (temp_edge != NULL) {
        int dest_vertex_index = temp_edge->dest;
        
        if (tarjan_array[dest_vertex_index].index == -1) {
            parcours(dest_vertex_index, graph, tarjan_array, stack, partition, timer); 
            tarjan_array[curr_vertex_index].lowlink = (tarjan_array[curr_vertex_index].lowlink < tarjan_array[dest_vertex_index].lowlink) ? tarjan_array[curr_vertex_index].lowlink : tarjan_array[dest_vertex_index].lowlink;
        }
        else if (tarjan_array[dest_vertex_index].on_stack) {
            tarjan_array[curr_vertex_index].lowlink = (tarjan_array[curr_vertex_index].lowlink < tarjan_array[dest_vertex_index].index) ? tarjan_array[curr_vertex_index].lowlink : tarjan_array[dest_vertex_index].index;
        }
        temp_edge = temp_edge->next;
    }

    if (tarjan_array[curr_vertex_index].lowlink == tarjan_array[curr_vertex_index].index) {
        
        t_classe new_class;
        new_class.vertex_ids = NULL;
        new_class.vertex_count = 0;
        new_class.capacity = 0; 
        
        int popped_vertex_index;
        do {
            popped_vertex_index = pop(stack);
            tarjan_array[popped_vertex_index].on_stack = false;
            add_vertex_to_class(&new_class, tarjan_array[popped_vertex_index].id); 
        } while (curr_vertex_index != popped_vertex_index);
        
        add_class(partition, new_class);
    }
}

t_partition tarjan(t_adj_list *graph) {
    
    t_tarjan_vertex *tarjan_array = create_tarjan_array(graph); 
    t_stack stack = create_stack();       
    t_partition partition = create_partition(graph->length / 2 + 1); 

    int timer_count = 0; 
    int class_id_counter = 0; 

    for (int curr_vertex_index = 0; curr_vertex_index < graph->length; curr_vertex_index++) {
        if (tarjan_array[curr_vertex_index].index == -1) {
            parcours(curr_vertex_index, graph, tarjan_array, &stack, &partition, &timer_count);
        }
    }

    free_stack(&stack);
    free(tarjan_array);
    return partition;
}

t_link_array create_link_array(int initial_capacity) {
    t_link_array link_array;
    link_array.links = malloc(initial_capacity * sizeof(t_link));
    if (link_array.links == NULL) exit(EXIT_FAILURE);
    link_array.link_count = 0;
    link_array.capacity = initial_capacity;
    return link_array;
}

void add_link(t_link_array *link_array, int from, int dest) {
    if (from == dest) return; 

    for (int i = 0; i < link_array->link_count; i++) {
        if (link_array->links[i].class_from == from && link_array->links[i].class_dest == dest) {
            return;
        }
    }

    if (link_array->link_count >= link_array->capacity) {
        link_array->capacity *= 2;
        link_array->links = realloc(link_array->links, link_array->capacity * sizeof(t_link));
        if (link_array->links == NULL) exit(EXIT_FAILURE);
    }

    link_array->links[link_array->link_count].class_from = from;
    link_array->links[link_array->link_count].class_dest = dest;
    link_array->link_count++;
}


int *create_class_map(t_partition *partition, int graph_length) {
    int *class_map = malloc(graph_length * sizeof(int));
    if (class_map == NULL) exit(EXIT_FAILURE);
    
    for (int i = 0; i < graph_length; i++) {
        class_map[i] = -1;
    }

    for (int class_index = 0; class_index < partition->class_count; class_index++) {
        t_classe *class = &partition->classes[class_index];
        
        for (int i = 0; i < class->vertex_count; i++) {
            int vertex_id = class->vertex_ids[i];
            if (vertex_id > 0 && vertex_id <= graph_length) {
                class_map[vertex_id - 1] = class_index;
            }
        }
    }

    return class_map;
}

t_link_array find_class_links(t_adj_list *graph, t_partition *partition, int *class_map) {
    t_link_array link_array = create_link_array(graph->length);
    
    for (int i = 0; i < graph->length; i++) {
        int from_class_index = class_map[i]; 
        
        t_cell *temp_edge = graph->list[i].head;

        while (temp_edge != NULL) {
            int dest_vertex_index = temp_edge->dest;
            int dest_class_index = class_map[dest_vertex_index]; 
            
            if (from_class_index != dest_class_index && from_class_index != -1 && dest_class_index != -1) {
                add_link(&link_array, from_class_index, dest_class_index);
            }
            temp_edge = temp_edge->next;
        }
    }
    return link_array;
}

void remove_transitive_links(t_link_array *link_array) {
    int i = 0;

    while (i < link_array->link_count) {
        t_link link1 = link_array->links[i];
        bool to_remove = false;
        for (int j = 0; j < link_array->link_count && !to_remove; j++) {
            if (j != i) {
                t_link link2 = link_array->links[j];
                if (link1.class_from == link2.class_from) {
                    for (int k = 0; k < link_array->link_count && !to_remove; k++) {
                        if (k != j && k != i) {
                            t_link link3 = link_array->links[k];
                            if ((link3.class_from == link2.class_dest) && (link3.class_dest == link1.class_dest)) {
                                to_remove = true;
                            }
                        }
                    }
                }
            }
        }
        if (to_remove) {
            link_array->links[i] = link_array->links[link_array->link_count - 1];
            link_array->link_count--;
        }
        else {
            i++;
        }
    }
}

void analyze_markov_properties(t_partition *partition, t_link_array *link_array) {
    bool *is_transient_map = calloc(partition->class_count, sizeof(bool));
    if (is_transient_map == NULL) return; 

    for (int i = 0; i < link_array->link_count; i++) {
        int class_from = link_array->links[i].class_from;
        int class_dest = link_array->links[i].class_dest;
        if (class_from != class_dest) {
            is_transient_map[class_from] = true;
        }
    }
  
    bool is_irreducible = (partition->class_count == 1);
    
    for (int i = 0; i < partition->class_count; i++) {
        t_classe *class = &partition->classes[i];
        printf("Classe %s {", class->name);
        for (int j = 0; j < class->vertex_count; j++) {
            printf("%d", class->vertex_ids[j]);
            if (j < class->vertex_count - 1) printf(",");
        }
        printf("}: ");

        if (is_transient_map[i]) {
            printf("est transitoire. Tous ses etats sont transitoires.\n");
        } else {
            printf("est persistant. Tous etats sont persistants.\n");
            if (class->vertex_count == 1) {
                printf("  -> L'etat %d est un etat absorbant.\n", class->vertex_ids[0]);
            }
        }
    }
    
    if (is_irreducible) printf("Le graphe est irreductible.\n");
    else printf("Le graphe n'est pas irreductible.\n");

    free(is_transient_map);
}

void write_hasse_mermaid(t_partition *partition, t_link_array *link_array, FILE *file) {
    if (file == NULL) return;

    fprintf(file, "%% Configuration pour le rendu\n");
    fprintf(file, "%% config: {\"layout\": \"elk\", \"theme\": \"neo\", \"look\": \"neo\"}\n");
    
    for (int i = 0; i < partition->class_count; i++) {
        t_classe *class = &partition->classes[i];
        char vertex_id_list_str[256] = ""; 
        char vertex_id_str[12];
        
        for (int j = 0; j < class->vertex_count; j++) {
            sprintf(vertex_id_str, "%d", class->vertex_ids[j]);
            strcat(vertex_id_list_str, vertex_id_str);
            if (j < class->vertex_count - 1) strcat(vertex_id_list_str, ",");
        }
        fprintf(file, "%s[\"{%s}\"]\n", class->name, vertex_id_list_str); 
    }

    for (int i = 0; i < link_array->link_count; i++) {
        char *from_name = partition->classes[link_array->links[i].class_from].name;
        char *dest_name = partition->classes[link_array->links[i].class_dest].name;
        fprintf(file, "%s --> %s\n", from_name, dest_name);
    }
    
}