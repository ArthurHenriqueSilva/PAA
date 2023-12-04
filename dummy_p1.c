//
#include <stdio.h>

int main(int argc, char *argv[]){
    FILE *input = fopen(argv[1], "r");
    int quant_lines;
    char input_code[12];
    char input_cnpj[18];
    int input_weight;

    fscanf(input, "%d", &quant_lines);

    for(int i = 0; i < quant_lines; i++){
        fscanf(input, "%s %s %d", input_code, input_cnpj, &input_weight);
        printf("%s %s %d\n", input_code, input_cnpj, input_weight);
    }
    // printf("%d\n", quant_lines);
    fclose(input);
}