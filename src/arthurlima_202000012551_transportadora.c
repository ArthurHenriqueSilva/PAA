#include <stdlib.h>
#include <stdio.h>

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

int max(int a, int b) {
    return (a > b) ? a : b;
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
    package->allocated = 0;
    return package;
}

Package* create_package_arr(FILE* input, int quantity) {
    Package* ps = malloc(quantity * sizeof(Package));
    for (int i = 0; i < quantity; i++)
        ps[i] = *create_package(input);
    return ps;
}

float generate_allocation(Vehicle v, Package* p, int pn) {
    int WL = v.weight;
    int VL = v.volume;

    int*** matrix = (int***)malloc((pn + 1) * sizeof(int**));
    for (int i = 0; i <= pn; i++) {
        matrix[i] = (int**)malloc((WL + 1) * sizeof(int*));
        for (int j = 0; j <= WL; j++)
            matrix[i][j] = (int*)calloc((VL + 1), sizeof(int));
    }

    for (int i = 1; i <= pn; i++) {
        for (int w = 1; w <= WL; w++) {
            for (int v = 1; v <= VL; v++) {
                if (p[i - 1].weight <= w && p[i - 1].volume <= v) {
                    matrix[i][w][v] = max(matrix[i - 1][w][v], matrix[i - 1][w - p[i - 1].weight][v - p[i - 1].volume] + p[i - 1].value);
                } else {
                    matrix[i][w][v] = matrix[i - 1][w][v];
                }
            }
        }
    }
    float total = matrix[pn][WL][VL];
    int j = WL, k = VL;
    for (int i = pn; i > 0 && j > 0 && k > 0; i--) {
        if ((matrix[i][j][k] != matrix[i - 1][j][k]) && p[i].allocated == 0) {
            printf("Package %s\n", p[i - 1].code);
            j -= p[i - 1].weight;
            k -= p[i - 1].volume;
            p[i].allocated = 1;
        }
    }

    for (int i = 0; i <= pn; i++) {
        for (int j = 0; j <= WL; j++)
            free(matrix[i][j]);
        free(matrix[i]);
    }
    free(matrix);
    return total;
}

void generate_all_allocations(Vehicle* v, int vn, Package* p, int pn) {
    for (int i = 0; i < vn; i++) {
        float val = generate_allocation(v[i], p, pn);
        printf("Allocations for Vehicle [%s] R$%f:\n", v[i].plate, val);
        printf("\n");
    }
}

void run_tasks(FILE* input, FILE* output) {
    int v_num, p_num;
    fscanf(input, "%d", &v_num);
    Vehicle* vs = create_vehicle_arr(input, v_num);
    fscanf(input, "%d", &p_num);
    Package* ps = create_package_arr(input, p_num);
    generate_all_allocations(vs, v_num, ps, p_num);
    free(vs);
    free(ps);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s input_file output_file\n", argv[0]);
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    if (!input || !output) {
        printf("Error opening files.\n");
        return 1;
    }

    run_tasks(input, output);

    fclose(input);
    fclose(output);

    return 0;
}
