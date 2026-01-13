#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

t_matrix create_empty_matrix(int size) {
    t_matrix matrix;
    matrix.size = size;
    
    matrix.data = malloc(size * sizeof(float *));
    if (matrix.data == NULL) exit(EXIT_FAILURE);
    
    for (int i = 0; i < size; i++) {
        matrix.data[i] = calloc(size, sizeof(float)); 
        if (matrix.data[i] == NULL) exit(EXIT_FAILURE); 
    }

    return matrix;
}

void copy_matrix(t_matrix dest_matrix, t_matrix from_matrix) {
    if (dest_matrix.size != from_matrix.size) return;

    for (int i = 0; i < from_matrix.size; i++) {
        for (int j = 0; j < from_matrix.size; j++) {
            dest_matrix.data[i][j] = from_matrix.data[i][j];
        }
    }
}

void free_matrix(t_matrix matrix) {
    for (int i = 0; i < matrix.size; i++) {
        free(matrix.data[i]);
    }

    free(matrix.data);
    matrix.size = 0;
}

void display_matrix(t_matrix matrix) {
    printf("Matrice (%d x %d) :\n", matrix.size, matrix.size);
    
    for (int i = 0; i < matrix.size; i++) {
        printf("[ ");
        for (int j = 0; j < matrix.size; j++) {
            printf("%6.3f", matrix.data[i][j]);
        }
        printf("\n");
    }
}

t_matrix create_matrix_from_graph(t_adj_list *graph) {
    int size = graph->length;
    t_matrix matrix = create_empty_matrix(size);
    
    for (int i = 0; i < size; i++) {
        t_cell *edge = graph->list[i].head;

        while (edge != NULL) {
            matrix.data[i][edge->dest] = edge->proba;
            edge = edge->next;
        }
    }
    return matrix;
}

t_matrix multiply_matrices(t_matrix matrix_A, t_matrix matrix_B) {
    if (matrix_A.size != matrix_B.size) exit(EXIT_FAILURE);
    
    t_matrix result_matrix = create_empty_matrix(matrix_A.size);
    
    for (int i = 0; i < matrix_A.size; i++) {

        for (int j = 0; j < matrix_A.size; j++) {
            float sum = 0.0f;

            for (int k = 0; k < matrix_A.size; k++) {
                sum += matrix_A.data[i][k] * matrix_B.data[k][j];
            }
            result_matrix.data[i][j] = sum;
        }
    }
    return result_matrix;
}

float diff_matrices(t_matrix matrix_A, t_matrix matrix_B) {
    if (matrix_A.size != matrix_B.size) return -1.0f;

    float diff = 0.0f;

    for (int i = 0; i < matrix_A.size; i++) {
        for (int j = 0; j < matrix_A.size; j++) {
            diff += fabs(matrix_A.data[i][j] - matrix_B.data[i][j]);
        }
    }
    return diff;
}

t_matrix power_matrix(t_matrix matrix, int p) {
    if (p < 1) return create_empty_matrix(matrix.size); 
    
    t_matrix result_matrix = create_empty_matrix(matrix.size);
    copy_matrix(result_matrix, matrix);
    
    if (p == 1) {
        return result_matrix;
    }
    
    t_matrix temp_matrix = create_empty_matrix(matrix.size);
    
    for (int i = 2; i <= p; i++) {
        copy_matrix(temp_matrix, result_matrix);
        t_matrix new_matrix_result = multiply_matrices(temp_matrix, matrix);
        free_matrix(result_matrix);
        result_matrix = new_matrix_result;
    }
    
    free_matrix(temp_matrix);
    return result_matrix;
}

t_matrix find_stationary_matrix(t_matrix matrix, float epsilon) {
    int size = matrix.size;
    int power = 1;
    float diff = INFINITY;
    
    t_matrix prev_matrix = create_empty_matrix(size);
    copy_matrix(prev_matrix, matrix);
    
    t_matrix curr_matrix = create_empty_matrix(size);
    copy_matrix(curr_matrix, matrix);
    
    while (diff > epsilon && power < 1000) { 
        power++;
        t_matrix next_matrix = multiply_matrices(curr_matrix, matrix);
        diff = diff_matrices(next_matrix, curr_matrix);
        free_matrix(prev_matrix);
        prev_matrix = curr_matrix;
        curr_matrix = next_matrix;
    }
    
    free_matrix(prev_matrix); 
    printf("Convergence trouvee a la puissance n=%d.\n", power);
    return curr_matrix;
}

t_matrix create_sub_matrix(t_matrix matrix, t_partition *partition, int class_index) {
    t_classe *class = &partition->classes[class_index];
    int sub_matrix_size = class->vertex_count;
    
    if (sub_matrix_size == 0) return create_empty_matrix(0);
    
    t_matrix sub_matrix = create_empty_matrix(sub_matrix_size);
    
    for (int local_row = 0; local_row < sub_matrix_size; local_row++) {
        int global_row = class->vertex_ids[local_row] - 1; 
        
        for (int local_col = 0; local_col < sub_matrix_size; local_col++) {
            int global_col = class->vertex_ids[local_col] - 1;
            sub_matrix.data[local_row][local_col] = matrix.data[global_row][global_col];
        }
    }
    
    return sub_matrix;
}

int gcd(int x, int y) {
    while (y != 0) {
        int temp = y;
        y = x % y;
        x = temp;
    }
    return x;
}

int get_period(t_matrix sub_matrix) {
    int size = sub_matrix.size;
    if (size == 0) return 0;
    
    int *periods = malloc(size * sizeof(int));
    if (periods == NULL) exit(EXIT_FAILURE); 
    int period_count = 0;
    
    t_matrix power_matrix = create_empty_matrix(size);
    t_matrix result_matrix = create_empty_matrix(size);
    copy_matrix(power_matrix, sub_matrix);
    
    for (int p = 1; p <= size * 2; p++) { 
        int diag_non_zero = 0;
        
        for (int i = 0; i < size; i++) {
            if (power_matrix.data[i][i] > 0.00001f) { 
                diag_non_zero = 1;
                break;
            }
        }
        
        if (diag_non_zero) {
            periods[period_count++] = p;
        }
    
    
        t_matrix temp = multiply_matrices(power_matrix, sub_matrix);
        copy_matrix(power_matrix, temp); 
        free_matrix(temp);
    }
    
    int result = 0;

    if(period_count > 0) {
        result = periods[0];
        for (int i = 1; i < period_count; i++) {
            result = gcd(result, periods[i]);
        }
    }
    
    free_matrix(power_matrix);
    free_matrix(result_matrix);
    free(periods);
    
    return result;
}