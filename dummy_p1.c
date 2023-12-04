#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char code[12];
    char cnpj[18];
    int weight;
} Container;

void printContainer(Container *container, int i){
    printf("Container %d: Code: %s CNPJ: %s Weight: %d\n", i+1, container->code, container->cnpj, container->weight);
}

void printContainers(Container *containers, int quant){
    for(int i = 0; i < quant; i++){
        printContainer(&containers[i], i);
    }
}

void fillContainers(FILE *input, Container *containers, int times){
    for(int i = 0; i < times; i++){
        fscanf(input, "%s %s %d", containers[i].code, containers[i].cnpj, &containers[i].weight);
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
