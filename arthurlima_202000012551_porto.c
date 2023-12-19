#include <stdio.h>

#define CODE_SIZE 12
#define CNPJ_SIZE 18

typedef struct {
    char code[CODE_SIZE];
    char cnpj[CNPJ_SIZE];
    int weight;
}Container;

int read_number(FILE *file){
    int x;
    fscanf(file, "%d", &x);
    return x;
}

void fillCOntainers(FILE *file, Container *cont, int n){
    for(int i = 0; i < n; i++   ){
        fscanf(file, "%s %s %d", cont[i].code, cont[i].cnpj, cont[i].weight);
    }
}

Container *create_vector_containers(int n, FILE *file){
    Container *cont = (Container *)malloc(n*sizeof(Container));
    fillContainers(file, cont, n);
    return cont
}

void merge(int* S, int* E, int i, int m, int j){
    int i1 = i, i2 = m + 1, k = i;
    while(i1 <= m && i2 <= j){
        if(E[i1] <= E[i2]) S[k++] = E[i1++];
        else S[k++] = E[i2++];
    }
    if(i1 > m) 
}
void mergeSort(int* S, int* E, int i, int j){
    if (i < j){
        int m = i + (j - 1) / 2;
        mergeSort(S, E, i, m);
        mergeSort(S, E, m + 1, j);
        merge(S,E,i,m,j);
    }
}
int main(int argc, char *argv[]){
    FILE *input = fopen(argv[1], 'r');
    FILE *output = fopen(argv[2], 'w');

    int size_1 = read_number(input);
    Container *container_1 = create_vector_containers(size_1, input);

    int size_2;
    Container *container_2 = create_vector_containers(size_2, input);
}