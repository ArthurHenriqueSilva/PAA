#include <stdio.h>
#include <stdlib.h>

#define CODE_SIZE 12
#define CNPJ_SIZE 18
#define MAX_CONTAINERS 100

typedef struct {
    char code[CODE_SIZE];
    char cnpj[CNPJ_SIZE];
    int weight;
} Container;

typedef struct{
    char code[CODE_SIZE];
    int diff;
    int percent;
} Erro_peso;

// Round float number to nearest integer
int my_round(float x){
    return (int)(x+0.5);
}
// Copy string to another place OK
char *copy_string(char *dest, char *src){
    char *original_dest = dest;
    while (((*dest++ = *src++) != '\0'));
    return original_dest;
}
// Compare strings OK
// return different from 0 means that strings are different
int compare_strings(const char *s1, const char *s2) {
    while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }

    if (*s1 == *s2) {
        return 0;  // Strings are equal
    } else if (*s1 < *s2) {
        return -1; // s1 is less than s2
    } else {
        return 1;  // s1 is greater than s2
    }
}

// return number read from arg file OK
int read_number(FILE *file){
    int x;
    (void)fscanf(file, "%d", &x);
    return x;
}
// Fill 2 containers arrays with data from file OK
void fill_containers(FILE *file, Container *c1, int n){
    for(int i = 0; i < n; i++) (void)fscanf(file, "%s %s %d", c1[i].code, c1[i].cnpj, &c1[i].weight);
}
// Merge Containers array OK
void merge_container(Container *arr, int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    Container *larr = malloc(n1*sizeof(Container));
    Container *rarr = malloc(n2*sizeof(Container));

    for(i = 0; i < n1; i++) larr[i] = arr[l+i];
    for(j = 0; j < n2; j++) rarr[j] = arr[m + 1 + j];
    
    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2){
        if(compare_strings(larr[i].code, rarr[j].code) <= 0){
            arr[k] = larr[i];
            i++;
        }
        else{
            arr[k] = rarr[j];
            j++;
        } 
        k++;
    }

    while(i < n1){
        arr[k] = larr[i];
        k++;
        i++;
    } 
    while (j < n2){
        arr[k] = rarr[j];
        k++;
        j++;
    } 

    free(larr);
    free(rarr);
}
// Sort Containers array
void merge_sort_container(Container *arr, int l, int r){
    if (l < r){
        int m = l + (r - l) / 2;

        merge_sort_container(arr, l, m);
        merge_sort_container(arr, m+1,r);

        merge_container(arr, l, m, r);
    }
}
// Search container code in array
int binary_search(Container *c, int low, int high, const char *code){
    while (low <= high){
        int mid = low + (high - low) / 2;
        int compare = compare_strings(c[mid].code, code);
        if(compare == 0) return mid;
        else if (compare == 1) high = mid - 1;
        else low = mid + 1;
    }
    return -1;
}


void merge_peso(Erro_peso *arr, int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    Erro_peso *larr = malloc(n1 * sizeof(Erro_peso));
    Erro_peso *rarr = malloc(n2 * sizeof(Erro_peso));

    for (i = 0; i < n1; i++)
        larr[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        rarr[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
        if (larr[i].percent >= rarr[j].percent) {
            arr[k] = larr[i];
            i++;
        } else {
            arr[k] = rarr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = larr[i];
        k++;
        i++;
    }

    while (j < n2) {
        arr[k] = rarr[j];
        k++;
        j++;
    }

    free(larr);
    free(rarr);

}

void merge_sort_peso(Erro_peso *arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        merge_sort_peso(arr, l, m);
        merge_sort_peso(arr, m + 1, r);

        merge_peso(arr, l, m, r);
    }
}

void print_container_array(Container *a, int size){
    for(int i = 0; i < size; i++) printf("%s :::: %s ::::: %d\n", a[i].code, a[i].cnpj, a[i].weight);
    printf("\n");
}

// Judge if there is some error between 2 given containers
// args -> c2 is the new container, c1 is the source container
int define_error(Container c1, Container c2, int *percent, int *diff){
    if(compare_strings(c1.cnpj, c2.cnpj) != 0) return 0;
    *diff = abs(c1.weight - c2.weight);

    // Imprimir a operação
    // printf("%d / %d * 100 = ", *diff, c1.weight);

    *percent = my_round(((float)*diff / (float)c1.weight) * 100);

    // Imprimir o resultado
    // printf("%d\n", *percent);

    if (*percent > 10) return 1;
    return -1;
}

// Search c2 container in c1 array
void search_container(Container *c1, int n1, Container *c2, int n2, FILE *output, Erro_peso *q){
    int percent, diff, q_index = 0;
    char *code = NULL;
    for(int i = 0; i < n1; i++){
        code = c1[i].code;
        
        int c2_index = binary_search(c2, 0, n2 - 1, code);
        if (c2_index == -1) continue;
        int error_code = define_error(c1[i], c2[c2_index], &percent, &diff);
        if(error_code == 0) fprintf(output, "%s: %s<->%s\n", c2[c2_index].code, c1[i].cnpj, c2[c2_index].cnpj);
        if (error_code == 1){
            copy_string(q[q_index].code, code);
            q[q_index].percent = percent;
            q[q_index].diff = diff;
            q_index++;
        }
    }
}

void print_erros_peso(FILE *output, Erro_peso *erros_peso, int size) {
    for (int i = 0; i < size; i++) {
        if(erros_peso[i].percent != 0){
            fprintf(output, "%s: %dkg (%d%%)\n", erros_peso[i].code, erros_peso[i].diff, erros_peso[i].percent);
        }
        
    }
}
int main(int argc, char *argv[]){
    printf("Started!\n");
    // FIle opening
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    if (input == NULL || output == NULL) {
        printf("Erro na leitura\n");
        return 1;
    }
    printf("Opened files: %s / %s\n", argv[1], argv[2]);
    
    // Container 1 array definition
    int size_c1 = read_number(input);
    Container c1[size_c1];
    fill_containers(input, c1, size_c1);
    printf("Create Container 1.\n");

    // Container 2 definition and sorting
    int size_c2 = read_number(input);
    Container c2[size_c2];
    fill_containers(input, c2, size_c2);
    printf("Create Container 2.\n");
    merge_sort_container(c2, 0, size_c2-1);
    printf("Sorted the Container 2\n");
    
    // print_container_array(c1, size_c1);
    // print_container_array(c2, size_c2);
    
    Erro_peso peso_q[size_c2];
    for(int i = 0; i < size_c2; i++) peso_q[i].percent = 0;
    printf("Create and initialize the peso array.\n");
    
    search_container(c1, size_c1, c2, size_c2, output, peso_q);
    merge_sort_peso(peso_q, 0, size_c2 - 1);
    print_erros_peso(output, peso_q, size_c2-1);
    fclose(input);
    fclose(output);
    return 0;
} 