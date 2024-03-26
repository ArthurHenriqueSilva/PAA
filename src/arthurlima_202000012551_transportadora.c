#include <stdlib.h>
#include <stdio.h>

typedef struct File {
    FILE* input;
    FILE* output;
} File;

typedef struct Vehicle {
    char plate[7];
    int weight;
    int volume;
} Vehicle;

typedef struct Package {
    char code[13];
    float value;
    int weight;
    int volume;
    int allocated;
} Package;

typedef struct Allocation {
    float value;
    int weight;
    int volume;
} Allocation;

File openFiles(char* input, char* output) {
    File file;
    file.input = fopen(input, "r");
    file.output = fopen(output, "w");
    return file;
}

int max(int a, int b){
    int out;
    out = (a>b) ? a:b;
    return out;
}

int read_number(FILE *file){
    int x;
    fscanf(file, "%d", &x);
    return x;
}

Vehicle* create_vehicle(FILE* input) {
    Vehicle* vehicle = malloc(sizeof(Vehicle));
    fscanf(input, "%s %d %d", vehicle->plate, &vehicle->weight, &vehicle->volume);
    return vehicle;
}

Vehicle* create_vehicle_arr(FILE* input, int quantity) {
    Vehicle* vs = malloc(quantity * sizeof(Vehicle));
    for (int i = 0; i < quantity; i++)
        vs[i] = *create_vehicle(input);
    return vs;
}

Package* create_package(FILE* input) {
    Package* package = malloc(sizeof(Package));
    fscanf(input, "%s %f %d %d", package->code, &package->value, &package->weight, &package->volume);
    package->allocated = 1;
    return package;
}

Package* create_package_arr(FILE* input, int quantity) {
    Package* ps = malloc(quantity * sizeof(Package));
    for (int i = 0; i < quantity; i++)
        ps[i] = *create_package(input);
    return ps;
}

void print_allocation_matrix(int*** matrix, int pn, int WL, int VL) {
    for (int i = 0; i < pn; i++) {
        printf("Package %d:\n", i + 1);
        for (int j = 0; j < WL; j++) {
            for (int k = 0; k < VL; k++) {
                printf("%d ", matrix[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

Allocation generate_allocation(Vehicle v, Package* p, int pn) {
    int WL = v.weight;
    int VL = v.volume;
    int*** matrix = (int***)malloc((pn+1) * sizeof(int**));
    for (int i = 0; i < pn; i++) {
        matrix[i] = (int**)malloc((WL+1) * sizeof(int*));
        for (int j = 0; j < WL; j++)
            matrix[i][j] = (int*)calloc((VL+1), sizeof(int));
    }
    for(int i = 1; i < i <= pn+1; i++){
        for(int w = 1; w <= WL+1; w++)
            for(int v = 1; v <= VL+1; v++){
                if (w - p[i].weight < 0 || v - p[i].volume < 0)
                    matrix[i, w, v] = matrix[i-1, w, v];
                else
                    matrix[i, w, v] = max(matrix[i-1, w, v], matrix[i-1][WL - p[i].weight][VL - p[i].volume] + p[i].value);
            }  
    }
    float total_value = matrix[pn+1][WL+1][VL+1];
    for(int i = pn+1; i > 0; i--){

    }
    print_allocation_matrix(matrix, pn, WL, VL);
}

void generate_all_allocations(Vehicle* v, int vn, Package* p, int pn) {
    for (int i = 0; i < vn; i++) {
        Allocation a = generate_allocation(v[i], p, pn);
    }
}

void run_tasks(FILE* input, FILE* output) {
    int v_num = read_number(input);
    Vehicle* vs = create_vehicle_arr(input, v_num);
    int p_num = read_number(input);
    Package* ps = create_package_arr(input, p_num);
    generate_all_allocations(vs, v_num, ps, p_num);
}

int main(int argc, char* argv[]) {
    File file = openFiles(argv[1], argv[2]);
    run_tasks(file.input, file.output);
    fclose(file.input);
    fclose(file.output);
    return 0;
}
