#include <stdio.h>
#include <pthread.h>

pthread_mutex_t step_mutex = PTHREAD_MUTEX_INITIALIZER;
int step_count = 0;

void swap(int *x, int *y){
    int temp = *x;
    *x = *y;
    *y = temp;

    pthread_mutex_lock(&step_mutex);
    step_count++;
    pthread_mutex_unlock(&step_mutex);
}

int read_number(FILE *file){
    int x;
    fscanf(file, "%d", &x);
    return x;
}

int fill_vector(FILE *file, int size, int *v){
    for(int i = 0; i < size; i++) fscanf(file, "%d", &v[i]);
}

void print_vector(int *v, int size){
    for(int i = 0; i < size; i++) printf("%d ", v[i]);
    printf("\n");
}

int lomuto(int *v, int i, int j){
    int p = v[j], x = i - 1, y = i;
    for(y = i; y < j; y++) 
        if (v[y] <= p) 
            swap(&v[++x], &v[y]);

    swap(&v[++x], &v[j]);
    return x;
}

void lomuto_qs(int *v, int i, int j){
    if(i < j){
        int pivo = lomuto(v, i, j);
        lomuto_qs(v, i, pivo - 1);
        lomuto_qs(v, pivo + 1, j);
    }
}

void main_routine(FILE *file){
    int size = read_number(file);
    int v[size];
    fill_vector(file, size, v);
    
    pthread_mutex_lock(&step_mutex);
    step_count = 0;
    pthread_mutex_unlock(&step_mutex);
    
    lomuto_qs(v, 0, size - 1);
    print_vector(v, size);

    
    pthread_mutex_lock(&step_mutex);
    printf("Number of steps: %d\n", step_count);
    pthread_mutex_unlock(&step_mutex);
}

int main(int argc, char *argv[]){
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    int quant_vector = read_number(input);

    for(int i = 0; i < quant_vector; i++) 
        main_routine(input);

    return 0;
}
