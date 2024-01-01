#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int swaps;
    int calls;
} Counters;

void swap(int *x, int *y, Counters *counters){
    int temp = *x;
    *x = *y;
    *y = temp;

    counters->swaps++;
}

int read_number(FILE *file){
    int x;
    fscanf(file, "%d", &x);
    return x;
}

int find_median_3(int x, int y, int z){
    int min, max;
    min = (x < y) ? x : y;
    min = (z < min) ? z : min;
    max = (x > y) ? x : y;
    max = (z > max) ? z : max;
    return (x + y + z - min - max);
}

void fill_vector(FILE *file, int size, int *v){
    for(int i = 0; i < size; i++) fscanf(file, "%d", &v[i]);
}

void print_vector(int *v, int size){
    for(int i = 0; i < size; i++) printf("%d ", v[i]);
    printf("\n");
}

int lp(int *v, int i, int j, Counters *counters){
    int piv = v[j], x = i - 1, y;
    for(y = i; y < j; y++) 
        if (v[y] <= piv)  swap(&v[++x], &v[y], counters);
    swap(&v[++x], &v[j], counters);
    return x;
}

void lp_qs(int *v, int i, int j, Counters *counters){
    counters->calls++;
    if(i < j){
        int pivo = lp(v, i, j, counters);
        lp_qs(v, i, pivo - 1, counters);
        lp_qs(v, pivo + 1, j, counters);
    }
}

int lm(int *v, int i, int j, Counters *counters) {
    return 0;
}

void lm_qs(int *v, int i, int j, Counters *counters) {
}

void print_counters(const char *label, Counters counters){
    printf("%s (%d) ", label, counters.swaps + counters.calls);
}

int main(int argc, char *argv[]){
    FILE *input = fopen(argv[1], "r");
    int quant_vector = read_number(input);

    for(int i = 0; i < quant_vector; i++) {
        Counters counters;
        counters.swaps = 0;
        counters.calls = 0;

        int size = read_number(input);
        int v[size];
        fill_vector(input, size, v);

        printf("%d: N (%d) ", i, size);
        // lp Padrão (LP)
        Counters lp_counters = counters;
        lp_qs(v, 0, size - 1, &lp_counters);
        print_counters("LP", lp_counters);
        
        // LM
        // Counters lm_counters = counters;
        // lm_qs(v, 0, size - 1, &lm_counters);
        // print_counters("LM", lm_counters);
        printf("\n");

        print_vector(v, size);
    }

    return 0;
}
