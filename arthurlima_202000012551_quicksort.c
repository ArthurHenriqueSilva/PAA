#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int swaps;
    int calls;
} Counters;

Counters init_Counters() {
    Counters newCounters;
    newCounters.swaps = 0;
    newCounters.calls = 0;
    return newCounters;
}

void swap(int *x, int *y, Counters *counters){
    int temp = *x;
    *x = *y;
    *y = temp;
    if(counters != NULL) counters->swaps++;
}

int read_number(FILE *file){
    int x;
    fscanf(file, "%d", &x);
    return x;
}

void print_counters(const char *label, Counters counters){
    printf("%s (%d) ", label, counters.swaps + counters.calls);
}

int find_median_index(int *v, int i, int j){
    int n = j - i + 1, a = n/4, b = n/2, c = 3*n/4, min, max, median;
    median = v[a];
    int median_idx = a;

    if(median < v[b]){
        median = v[b];
        median_idx = b;
    }

    if (median > v[c]){
        median = v[c];
        median_idx = c;
    }

    if (median < v[a]){
        median = v[a];
        median_idx = a;
    }
    return median_idx;
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
}

void print_vectors(int *v1, int *v2, int *v3, int *v4, int *v5, int *v6, int size){
    print_vector(v1, size);
    print_vector(v2, size);
    print_vector(v3, size);
    print_vector(v4, size);
    print_vector(v5, size);
    print_vector(v6, size);
    printf("\n");
}

// 0=LA, 1=LM, 2=LP
void lomuto_quicksort(int *v, int i, int j, int method, Counters *counters){
    counters->calls++;
    if(i < j){
        int pivo, y = i, x =  i - 1;
        if (method == 0) {
            swap(&v[j], &v[i+abs(v[i]) % (j - i + 1)], counters);
        }
        else if(method == 1){
            int median_index = find_median_index(v, i, j);
            swap(&v[j], &v[median_index], counters);
        }
        pivo = v[j];
        for(y = i; y < j; y++) 
            if (v[y] <= pivo)  swap(&v[++x], &v[y], counters);
        swap(&v[++x], &v[j], counters);
        pivo = x;
        lomuto_quicksort(v, i, pivo - 1, method, counters);
        lomuto_quicksort(v, pivo + 1, j, method, counters);
    }
}
// 0=HM, 1=HA, 2=HP
void hoare_quicksort(int *v, int i, int j, int method, Counters *counters){
    counters->calls++;
    if(i < j){
        int pivo;
        int x = i - 1;
        int y = j + 1;
        if (method == 0) {
            swap(&v[j], &v[i+abs(v[i]) % (j - i + 1)], counters);
        }
        else if(method == 1){
            
        }
        pivo = v[i];
        for(y = i; y < j; y++) 
            if (v[y] <= pivo)  swap(&v[++x], &v[y], counters);
        swap(&v[++x], &v[j], counters);
        pivo = x;

        hoare_quicksort(v, i, pivo - 1, method, counters);
        hoare_quicksort(v, pivo + 1, j, method, counters);

    }
}

int main(int argc, char *argv[]){
    FILE *input = fopen(argv[1], "r");
    int quant_vector = read_number(input);
    for(int i = 0; i < quant_vector; i++) {
        int size = read_number(input);
        int real_size = size - 1;
        int v1[size], v2[size], v3[size], v4[size], v5[size], v6[size];
        fill_vectors(input, size, v1,v2,v3,v4,v5,v6);
        
        Counters lp_counters = init_Counters();
        Counters la_counters = init_Counters();
        Counters lm_counters = init_Counters();
        Counters hp_counters = init_Counters();
        Counters ha_counters = init_Counters();
        Counters hm_counters = init_Counters();

        printf("%d: N(%d) ", i, size);
        lomuto_quicksort(v1,0, real_size, 2, &lp_counters);
        print_counters("LP", lp_counters);
        lomuto_quicksort(v2, 0, real_size, 0, &la_counters);
        print_counters("LA", la_counters);
        lomuto_quicksort(v3, 0, real_size, 1, &lm_counters);
        print_counters("LM", lm_counters);

        printf("\n");
       
    }
}