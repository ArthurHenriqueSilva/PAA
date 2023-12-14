#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char code[12];
    char cnpj[18];
    int weight;
} Container;

typedef struct {
    int precedence;
    char code[12];
    char cnpj1[18];
    char cnpj2[18];
    int diff;
    float percent;
} Erro;

int my_round(float x){
    return (int)(x + 0.5);
} 

int compare_strings(const char *s1, const char *s2 ){
    while (*s1 != '\0' && *s2 != '\0'){
        if(*s1 != *s2) return 0;
        s1++;
        s2++;
    }
    return (*s1 == '\0' && *s2 == '\0');

}
int read_number(FILE *input){
    int out;
    fscanf(input, "%d", &out);
    return out;
}

void printContainer(Container *container, int i){
    printf("Container %d: Code: %s CNPJ: %s Weight: %d\n", i+1, container->code, container->cnpj, container->weight);
}

void printContainers(Container *containers, int quant){
    for(int i = 0; i < quant; i++){
        printContainer(&containers[i], i);
    }
    printf("\n");
}

void fillContainers(FILE *input, Container *containers, int times){
    for(int i = 0; i < times; i++){
        fscanf(input, "%s %s %d", containers[i].code, containers[i].cnpj, &containers[i].weight);
    }
}

Container *create_vector_containers(int quant, FILE *input){
    Container *cont = (Container *)malloc(quant * sizeof(Container));
    fillContainers(input, cont, quant);
    return cont;
}


int main(int argc, char *argv[]){
    FILE *input = fopen(argv[1], "r");

    int number_lines_1 = read_number(input);
    
    Container *containers = create_vector_containers(number_lines_1, input);
    int number_lines_2 = read_number(input);
    Container *containers_2 = create_vector_containers(number_lines_2, input);
    // printContainers(containers, number_lines_1);
    // printContainers(containers_2, number_lines_2);

    free(containers);
    fclose(input);
    return 0;
}