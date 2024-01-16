#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int swaps;
    int calls;
} Counters;

// General rules and actions
// OK
void swap(int *x, int *y, Counters *counters){
    int temp = *x;
    *x = *y;
    *y = temp;

    counters->swaps++;
}
// OK
int read_number(FILE *file){
    int x;
    fscanf(file, "%d", &x);
    return x;
}

int my_rand(int *v, int ini, int n){
    return 0;
}

// median methods
// OK
int find_median_3(int x, int y, int z){
    int min, max, median;
    min = (x < y) ? x : y;
    min = (z < min) ? z : min;
    max = (x > y) ? x : y;
    max = (z > max) ? z : max;
    median = x + y + z - min - max;
    return (median);
}
int find_median_index(int *v, int i, int j){
    int n = j - i + 1, x = v[i + n/4], y = v[i + n/2], z = v[i + 3*n/4];
    int median, index;
    median = find_median_3(x,y,z);
    if (median == x) index = i + n/4;
    else if (median == y) index = i + n /2;
    else index = i + 3*n/4;

    int mid = (i + j) / 2;
    for(int k = i; k <= j; ++k) {
        if(v[k] == median){
            if(abs(k - mid) < abs(index - mid)) index = k;
        }
    }
    return index;
 
}

// Print and Fill methods
// OK
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
// OK
void print_vector(int *v, int size){
    printf("\n");
    for(int i = 0; i < size; i++) printf("%d ", v[i]);
    // printf("\n");
}
// OK
void print_vectors(int *v1, int *v2, int *v3, int *v4, int *v5, int *v6, int size){
    print_vector(v1, size);
    print_vector(v2, size);
    print_vector(v3, size);
    print_vector(v4, size);
    print_vector(v5, size);
    print_vector(v6, size);
}
// OK

// Sort methods
int lp(int *v, int i, int j, Counters *counters){
    int piv = v[j], x = i - 1, y;
    for(y = i; y < j; y++) 
        if (v[y] <= piv)  swap(&v[++x], &v[y], counters);
    swap(&v[++x], &v[j], counters);
    return x;
}
// OK
void lp_qs(int *v, int i, int j, Counters *counters){
    counters->calls++;
    if(i < j){
        int pivo = lp(v, i, j, counters);
        lp_qs(v, i, pivo - 1, counters);
        lp_qs(v, pivo + 1, j, counters);
    }
}

int lm(int *v, int i, int j, Counters *counters) {
    int median_index, n;
    median_index = find_median_index(v, i, j);
    swap(&v[j], &v[median_index], counters);
    return lp(v, i, j, counters);
}

void lm_qs(int *v, int i, int j, Counters *counters) {
    counters->calls++;
    if(i < j){
        int pivo = lm(v, i, j, counters);
        lm_qs(v,i, pivo-1, counters);
        lm_qs(v, pivo+1, j, counters);
    }
}


int lr(int *v, int i, int j, Counters *counters){
    swap(&v[j], &v[i+(rand() % (j - i + 1))], counters);
    return lp(v, i, j, counters);
}

void lr_qs(int *v, int i, int j, Counters *counters){
    counters->calls++;
    if(i < j){
        int pivo = lr(v, i, j, counters);
        lr_qs(v, i, pivo, counters);
        lr_qs(v, pivo + 1, j, counters);
    }
}

// OK
int hp(int *v, int i, int j, Counters *counters){
    int p = v[i], x = i - 1, y = j + 1;
    while(1){
        while(v[--y] > p);
        while(v[++x] < p);
        if(x < y) swap(&v[x], &v[y], counters);
        else return y;
    }
}
// OK
void hp_qs(int *v, int i, int j, Counters *counters){
    counters->calls++;
    if(i < j){
        int pivo = hp(v, i, j, counters);
        hp_qs(v, i, pivo, counters);
        hp_qs(v, pivo + 1, j, counters);
    }
}

int hm(int *v, int i, int j, Counters *counters) {return 0;}

void hm_qs(int*v, int i, int j, Counters *counters){}


int hr(int *v, int i, int j, Counters *counters){
    swap(&v[j], &v[i+(rand() % (j - i + 1))], counters);
    return hp(v, i, j, counters);
}
void hr_qs(int*v, int i, int j, Counters *counters){
    counters->calls++;
    if(i < j){
        int pivo = hr(v, i, j, counters);
        hr_qs(v, i, pivo, counters);
        hr_qs(v, pivo + 1, j, counters);
    }   
}
// OK
void print_counters(const char *label, Counters counters){
    printf("%s (%d) ", label, counters.swaps + counters.calls);
}

int main(int argc, char *argv[]){
    FILE *input = fopen(argv[1], "r");
    int quant_vector = read_number(input);
    char methods[6][2] = {
        "LP",
        "LM",
        "LR",
        "HP",
        "HA",
        "HM"
    };
    
    for(int i = 0; i < quant_vector; i++) {
        Counters lp_counters, lm_counters, lr_counters, hp_counters, hr_counters;
        lp_counters.swaps = 0;
        lp_counters.calls = 0;
        
        lr_counters.swaps = 0;
        lr_counters.calls = 0;

        lm_counters.swaps = 0;
        lm_counters.calls = 0;
        
        hp_counters.swaps = 0;
        hp_counters.calls = 0;

        hr_counters.swaps = 0;
        hr_counters.calls = 0;



        int size = read_number(input);
        int v1[size], v2[size], v3[size], v4[size], v5[size], v6[size];
        int real_size = size - 1;
        fill_vectors(input, size, v1, v2, v3, v4, v5, v6);
        // print_vectors(v1,v2,v3,v4,v5, v6, size);

        printf("%d: N(%d) ", i, size);
        // // // lp Padrão (LP) - Passa no input de exemplo
        lp_qs(v1, 0, real_size, &lp_counters);
        print_counters("LP", lp_counters);
        
        // LM - ordena, erra a contagem
        // print_vector(v2, size);
        lm_qs(v2, 0, real_size, &lm_counters);
        print_counters("LM", lm_counters);

        // // LR - Ordena mas falta implementar o rand
        lr_qs(v3, 0, real_size, &lr_counters);
        print_counters("LA", lr_counters);

        // // Hoare Padrao(HP) - Passa no input de exemplo
        hp_qs(v4, 0, real_size, &hp_counters);
        print_counters("HP", hp_counters);

        // // Hoare Aleatorio
        hr_qs(v5, 0, real_size, &hr_counters);
        print_counters("HA", hr_counters);

        // Hm - Falta fazer a funcao hm_qs
        // hm_qs(v6, 0, real_size, &hm_counters);

        printf("\n");
    }
    return 0;
}
