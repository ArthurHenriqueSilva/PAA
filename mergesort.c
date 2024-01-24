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
int compare_strings(const char *s1, const char *s2){
    while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2){
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}
// return number read from arg file OK
int read_number(FILE *file){
    int x;
    fscanf(file, "%d", &x);
    return x;
}
// Fill 2 containers arrays with data from file OK
void fill_containers(FILE *file, Container *c1, int n){
    for(int i = 0; i < n; i++) fscanf(file, "%s %s %d", c1[i].code, c1[i].cnpj, &c1[i].weight);
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
        else if (compare < 0) low = mid++;
        else high = mid--;
    }
    return -1;
}

void print_container_array(Container *a, int size){
    for(int i = 0; i < size; i++) printf("%s\n", a[i].code);
    printf("\n");
}
int main(int argc, char *argv[]){
    // abertura de arquivos
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    
    // definicao de array de containers 1
    int size_c1 = read_number(input);
    Container c1[size_c1];
    fill_containers(input, c1, size_c1);
    print_container_array(c1, size_c1);

    // definicao e ordenacao de array de containers 2
    int size_c2 = read_number(input);
    Container c2[size_c2];
    fill_containers(input, c2, size_c2);
    merge_sort_container(c2, 0, size_c2-1);
    
    // definicao e inicializacao do array de pesos
    Erro_peso peso_q[size_c2];
    for(int i = 0; i < size_c2; i++) peso_q[i].percent = 0;

    

}