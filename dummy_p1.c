// To compile: gcc  dummy_p1.c -o dummy_p1
// To execute: ./dummy_p1 text.txt

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

Erro *aux_survey_container(Container container, Container *containers, int size_containers){
    Erro *erros = (Erro *)malloc(size_containers * sizeof(Container));
    int erro_index = 0;
    for(int i = 0; i < size_containers; i++){
        if(compare_strings(container.code, containers[i].code)){
            if (!(compare_strings(container.cnpj, containers[i].cnpj))){
                erros[erro_index].precedence = 0;
                erros[erro_index].code = container.code;
                erros[erro_index].cnpj1 = container.cnpj;
                erros[erro_index].cnpj2 = containers[i].cnpj;
                erro_index++;
                printf("%s: %s <-> %s\n", container.code, container.cnpj, containers[i].cnpj);
                continue;
            }
            int diff = container.weight - containers[i].weight;
            float percent = (float)diff / containers[i].weight;
            percent *= 100;
            percent = my_round(percent);
            if (percent > 10){
                erros[erro_index].code = 1;
                erros[erro_index].code = container.code;
                erros[erro_index].diff = diff;
                erros[erro_index].percent = percent;

            } printf("%s: %dkg (%.0f%%)\n", container.code, diff, percent);

        }

    }
}

void survey_containers(Container *containers_2, int size_containers_2, Container *containers_1, int size_containers_1){
    for(int i = 0; i < size_containers_2; i++){
       aux_survey_container(containers_2[i], containers_1, size_containers_1); 
    }
}

int main(int argc, char *argv[]){
    FILE *input = fopen(argv[1], "r");

    int quant_lines;
    fscanf(input, "%d", &quant_lines);

    Container *containers = (Container *)malloc(quant_lines * sizeof(Container));
    fillContainers(input, containers, quant_lines);
    printContainers(containers, quant_lines); 
    free(containers);
    fclose(input);
    return 0;
}
