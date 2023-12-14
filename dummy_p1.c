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
} Error;

int my_round(float x) {
    return (int)(x + 0.5);
}

int compare_strings(const char *s1, const char *s2) {
    while (*s1 != '\0' && *s2 != '\0') {
        if (*s1 != *s2) return 0;
        s1++;
        s2++;
    }
    return (*s1 == '\0' && *s2 == '\0');
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

void printContainers(Container *containers, int quant) {
    for (int i = 0; i < quant; i++) {
        printContainer(&containers[i], i);
    }
    printf("\n");
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

Error define_Error(Container container1, Container container2) {
    Error this_error;
    copy_string(this_error.code, container1.code);
    if ((this_error.precedence = compare_strings(container1.cnpj, container2.cnpj)) == 0) {
        copy_string(this_error.cnpj1, container1.cnpj);
        copy_string(this_error.cnpj2, container2.cnpj);
    } else {
        int diff = container1.weight - container2.weight;
        this_error.diff = diff < 0 ? -diff : diff;
        this_error.percent = my_round(((float)this_error.diff / (float)container1.weight) * 100);

    }

    return this_error;
}


Error *find_sameCode_containers(Container *container1, int nl1, Container *container2, int nl2, int *num_errors) {
    Error *errors = (Error *)malloc(nl1 * sizeof(Error));
    int error_index = 0;
    for (int i = 0; i < nl1; i++) {
        for (int j = 0; j < nl2; j++) {
            if (compare_strings(container1[i].code, container2[j].code)) {
                errors[error_index++] = define_Error(container1[i], container2[j]);
            }
        }
    }
    *num_errors = error_index;
    return errors;
}

void printErrors(Error *errors, int num_errors) {
    for (int i = 0; i < num_errors; i++) {
        printf("Error %d: Code: %s Precedence: %d ", i+1, errors[i].code,errors[i].precedence);


        if (errors[i].precedence == 1) {
            printf("Diff: %d Percent: %0.f%%", errors[i].diff, errors[i].percent);
        } else {
            printf("CNPJ1: %s CNPJ2: %s ",
               errors[i].cnpj1, errors[i].cnpj2);
        }
        printf("\n");
    }
}


void freeErrors(Error *errors) {
    free(errors);
}

int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[1], "r");

    int number_lines_1 = read_number(input);
    Container *containers = create_vector_containers(number_lines_1, input);

    int number_lines_2 = read_number(input);
    Container *containers_2 = create_vector_containers(number_lines_2, input);

    int num_errors;
    Error *errors = find_sameCode_containers(containers, number_lines_1, containers_2, number_lines_2, &num_errors);

    printErrors(errors, num_errors);

    freeErrors(errors);
    free(containers);
    free(containers_2);
    fclose(input);

    return 0;
}
