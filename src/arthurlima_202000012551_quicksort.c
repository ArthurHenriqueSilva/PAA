#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int swaps;
    int calls;
    char label[3];
} Counters;

Counters init_Counters(const char *label) {
    Counters newCounters;
    newCounters.swaps = 0;
    newCounters.calls = 0;
    strncpy(newCounters.label, label, sizeof(newCounters.label) - 1);
    newCounters.label[sizeof(newCounters.label) - 1] = '\0';
    
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
    int n = j - i + 1, a = n/4, b = n/2, c = 3*n/4, median;
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
        if (method == 0) {
            swap(&v[j], &v[i+abs(v[i]) % (j - i + 1)], counters);
        }
        else if(method == 1){
            int median_index = find_median_index(v, i, j);
            swap(&v[j], &v[median_index], counters);
        }
        int pivo = v[j], y = i, x =  i - 1;
        for(y = i; y < j; y++) 
            if (v[y] <= pivo)  swap(&v[++x], &v[y], counters);
        swap(&v[++x], &v[j], counters);
        pivo = x;
        lomuto_quicksort(v, i, pivo - 1, method, counters);
        lomuto_quicksort(v, pivo + 1, j, method, counters);
    }
}
// 0=HA, 1=HM, 2=HP
void hoare_quicksort(int *v, int i, int j, int method, Counters *counters){
    counters->calls++;
    if(i < j){
        if (method == 0) {
            swap(&v[i], &v[i+abs(v[i]) % (j - i + 1)], counters);
        }
        else if(method == 1){
            int median_index = find_median_index(v, i, j);
            swap(&v[i], &v[median_index], counters);
        }
        int pivo = v[i];
        int x = i - 1;
        int y = j + 1;
        while(1){
            while(v[--y] > pivo);
            while(v[++x] < pivo);
            if (x < y) swap(&v[x], &v[y], counters);
            else {
                pivo = y;
                break;
            }
        }
        hoare_quicksort(v, i, pivo, method, counters);
        hoare_quicksort(v, pivo + 1, j, method, counters);

    }
}

void merge(Counters counters_array[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    Counters L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = counters_array[l + i];
    for (j = 0; j < n2; j++)
        R[j] = counters_array[m + 1 + j];
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        int sum1 = L[i].swaps + L[i].calls;
        int sum2 = R[j].swaps + R[j].calls;

        if (sum1 <= sum2) {
            counters_array[k] = L[i];
            i++;
        } else {
            counters_array[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        counters_array[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        counters_array[k] = R[j];
        j++;
        k++;
    }
}
void merge_sort(Counters counters_array[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort(counters_array, l, m);
        merge_sort(counters_array, m + 1, r);
        merge(counters_array, l, m, r);
    }
}


int main(int argc, char *argv[]){
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    int quant_vector = read_number(input);
    Counters counters_array[6];
    for(int i = 0; i < quant_vector; i++) {
        int size = read_number(input);
        int real_size = size - 1;
        int v1[size], v2[size], v3[size], v4[size], v5[size], v6[size];
        fill_vectors(input, size, v1,v2,v3,v4,v5,v6);
        
        Counters lp_counters = init_Counters("LP");
        Counters la_counters = init_Counters("LA");
        Counters lm_counters = init_Counters("LM");
        Counters hp_counters = init_Counters("HP");
        Counters ha_counters = init_Counters("HA");
        Counters hm_counters = init_Counters("HM");
        lomuto_quicksort(v1,0, real_size, 2, &lp_counters);
        lomuto_quicksort(v2, 0, real_size, 0, &la_counters);
        lomuto_quicksort(v3, 0, real_size, 1, &lm_counters);
        hoare_quicksort(v4, 0, real_size, 2, &hp_counters);
        hoare_quicksort(v5, 0, real_size, 0, &ha_counters);
        hoare_quicksort(v6, 0, real_size, 1, &hm_counters);

        counters_array[0] = lp_counters;
        counters_array[1] = la_counters;
        counters_array[2] = lm_counters;
        counters_array[3] = hp_counters;
        counters_array[4] = ha_counters;
        counters_array[5] = hm_counters;

        merge_sort(counters_array, 0, 5);
        fprintf(output, "%d: N(%d) ", i, size);
        for (int j = 0; j < 6; j++)
            fprintf(output, "%s (%d) ", counters_array[j].label, counters_array[j].swaps + counters_array[j].calls);
        fprintf(output, "\n");
    }
    fclose(input);
    fclose(output);
    return 0;
}