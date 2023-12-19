#include <stdio.h>
#include <stdlib.h>

#define CODE_LENGTH 12
#define CNPJ_LENGTH 18

typedef struct {
    char code[CODE_LENGTH];
    char cnpj[CNPJ_LENGTH];
    int weight;
} Container;

typedef struct {
    char code[CODE_LENGTH];
    int diff;
    int percent;

}WEIGHT_Error;

int my_round(float x) {
    return (int)(x + 0.5);
}


int compare_strings(const char *str1, const char *str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return 1;
        }
        str1++;
        str2++;
    }

    if (*str1 != '\0' || *str2 != '\0') {
        return 1;
    }

    return 0;
}

char *copy_string(char *dest, char *src) {
    char *original_dest = dest;
    while ((*dest++ = *src++) != '\0');
    return original_dest;
}

int read_number(FILE *input) {
    int out;
    fscanf(input, "%d", &out);
    return out;
}

void printContainer(Container *container, int i) {
    printf("Container %d: Code: %s CNPJ: %s Weight: %d\n", i + 1, container->code, container->cnpj, container->weight);
}

void fillContainers(FILE *input, Container *containers, int times) {
    for (int i = 0; i < times; i++) {
        fscanf(input, "%s %s %d", containers[i].code, containers[i].cnpj, &containers[i].weight);
    }
}

Container *create_vector_containers(int quant, FILE *input) {
    Container *cont = (Container *)malloc(quant * sizeof(Container));
    fillContainers(input, cont, quant);
    return cont;
}


WEIGHT_Error *create_vector_weight(int quant){
    WEIGHT_Error *weight_q = (WEIGHT_Error *)malloc(quant * sizeof(WEIGHT_Error));
    return weight_q;
}

int define_error(Container c1, Container c2, int* percent, int* diff){
    if(compare_strings(c1.cnpj, c2.cnpj)) return 0;
    *diff = c1.weight - c2.weight;
    *diff = *diff < 0 ? (-*diff):*diff;
    *percent = my_round(((float)*diff / (float)c1.weight) * 100);
    if (*percent > 10) return 1;
    return 2;
    
}

void find_same_code(Container *c1, int n1, Container *c2, int n2, WEIGHT_Error *weight_q, FILE *output) {
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            if(!compare_strings(c1[i].code, c2[j].code)) {
                int percent, diff;
                int error_code = define_error(c1[i], c2[j], &percent, &diff);
                
                if (!error_code) {
                    fprintf(output, "%s: %s<->%s\n", c1[i].code, c1[i].cnpj, c2[j].cnpj);
                }
                if (error_code == 1) {
                    fprintf(output, "%s: %dkg (%d%%)\n", c1[i].code, diff, percent);
                    // copy_string(weight_q->code, c1[i].code);
                    // weight_q->percent = percent;
                    // weight_q->diff = diff;
                    // weight_q++;
                }
                break;
            }
        }
    }
}



void printWeightErrors(FILE *output, WEIGHT_Error *weight_q, int n) {
    for (int i = 0; i < n; i++) {
        if (weight_q[i].code[0] != '\0') {
            fprintf(output, "%s: %dkg (%d%%)\n", weight_q[i].code, weight_q[i].diff, weight_q[i].percent);
        }
    }
}


int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    int number_lines_1 = read_number(input);
    Container *containers = create_vector_containers(number_lines_1, input);

    int number_lines_2 = read_number(input);
    Container *containers_2 = create_vector_containers(number_lines_2, input);

    WEIGHT_Error *weight_q = create_vector_weight(number_lines_2);

    find_same_code(containers, number_lines_1, containers_2, number_lines_2, weight_q, output);


    printWeightErrors(output, weight_q, number_lines_2);

    free(containers);
    free(containers_2);
    free(weight_q);
    fclose(input);

    return 0;
}
