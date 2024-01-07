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

void fill_vectors(FILE *file, int size, int *v1, int *v2, int *v3, int *v4, int *v5, int *v6){
    for(int i = 0; i < size; i++) {
        int value;
        fscanf(file, "%d", &value);
        v1[i] = value;
        v2[i] = value;
        v3[i] = value;
        v4[i] = value;
        v5[i] = value;
        v6[i] = value;
        
    } 
}


void print_vector(int *v, int size){
    printf("\n");
    for(int i = 0; i < size; i++) printf("%d ", v[i]);
    printf("\n");
}

void print_vectors(int *v1, int *v2, int *v3, int *v4, int *v5, int *v6, int size){
    print_vector(v1, size);
    print_vector(v2, size);
    print_vector(v3, size);
    print_vector(v4, size);
    print_vector(v5, size);
    print_vector(v6, size);
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

int lr(int *v, int i, int j, Counters *counters){
    swap(&v[j], &v[i+(rand() % (j - i + 1))], counters);
    return lr(v, i, j, counters);
}

void lr_qs(int *v, int i, int j, Counters *counters){
    counters->calls++;
    if(i < j){
        int pivo = lr(v, i, j, counters);
        lr_qs(v, i, pivo, counters);
        lr_qs(v, pivo + 1, j, counters);
    }
}

int hoare(int *v, int i, int j, Counters *counters){
    int p = v[i], x = i - 1, y = j + 1;
    while(1){
        while(v[--y] > p);
        while(v[++x] < p);
        if(x < y) swap(&v[x], &v[y], counters);
        else return y;
    }
}

void hoare_qs(int *v, int i, int j, Counters *counters){
    counters->calls++;
    if(i < j){
        int pivo = hoare(v, i, j, counters);
        hoare_qs(v, i, pivo, counters);
        hoare_qs(v, pivo + 1, j, counters);
    }
}


void print_counters(const char *label, Counters counters){
    printf("%s (%d) ", label, counters.swaps + counters.calls);
}

int main(int argc, char *argv[]){
    FILE *input = fopen(argv[1], "r");
    int quant_vector = read_number(input);

    for(int i = 0; i < quant_vector; i++) {
        Counters lp_counters, lm_counters, lr_counters, hp_counters;
        lp_counters.swaps = 0;
        lp_counters.calls = 0;
        
        hp_counters.swaps = 0;
        hp_counters.calls = 0;

        lr_counters.swaps = 0;
        lr_counters.calls = 0;

        int size = read_number(input);
        int v1[size], v2[size], v3[size], v4[size], v5[size], v6[size];
        fill_vectors(input, size, v1, v2, v3, v4, v5, v6);
        // print_vectors(v1,v2,v3,v4,v5, v6, size);

        printf("%d: N(%d) ", i, size);
        // // lp Padrão (LP) - Passa no input de exemplo
        lp_qs(v1, 0, size - 1, &lp_counters);
        print_counters("LP", lp_counters);
        
        // LM - A FAZER
        // lm_qs(v2, 0, size - 1, &lm_counters);
        // print_counters("LM", lm_counters);

        // LR - Ordena, mas, a contagem est[a errada
        lr_qs(v3, 0, size - 1, &lr_counters);
        print_counters("LR", lr_counters);

        // Hoare Padrao(HP) - Passa no input de exemplo
        hoare_qs(v4, 0, size - 1, &hp_counters);
        print_counters("HP", hp_counters);
        printf("\n");



    }

    return 0;
}
