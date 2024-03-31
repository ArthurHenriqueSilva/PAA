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

typedef struct Node {
    Package* package;
    struct Node* next;
} Node;

typedef struct LinkedList {
    Node* head;
} LinkedList;

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
    package->allocated = 1;
    return package;
}

Package* create_package_arr(FILE* input, int quantity) {
    Package* ps = malloc(quantity * sizeof(Package));
    for (int i = 0; i < quantity; i++)
        ps[i] = *create_package(input);
    return ps;
}

float generate_allocation(Vehicle veiculo, Package* p, int pn) {
    LinkedList allocated_list;
    allocated_list.head = NULL;
    
    int WL = veiculo.weight;
    int VL = veiculo.volume;

    int*** matrix = (int***)malloc((pn + 1) * sizeof(int**));
    for (int i = 0; i <= pn; i++) {
        matrix[i] = (int**)malloc((WL + 1) * sizeof(int*));
        for (int j = 0; j <= WL; j++)
            matrix[i][j] = (int*)calloc((VL + 1), sizeof(int));
    }

    int w_total = 0;
    int v_total = 0;
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
        if ((matrix[i][j][k] != matrix[i - 1][j][k]) && p[i - 1].allocated == 1) {
            w_total += p[i - 1].weight;
            v_total += p[i - 1].volume;
            j -= p[i - 1].weight;
            k -= p[i - 1].volume;
            p[i - 1].allocated = 0;            
            Node* new_node = malloc(sizeof(Node));
            new_node->package = &p[i - 1];
            new_node->next = allocated_list.head;
            allocated_list.head = new_node;
        }
    }

    printf("[%s] R$%.2f %dKg(%%) %dL(%%)\n", veiculo.plate, total, w_total, v_total);
    Node* current = allocated_list.head;
    while (current != NULL) {
        printf("%s\n", current->package->code);
        current = current->next;
    }
    
    // Libera a memória alocada para a matriz
    for (int i = 0; i <= pn; i++) {
        for (int j = 0; j <= WL; j++)
            free(matrix[i][j]);
        free(matrix[i]);
    }
    free(matrix);
    
    // Libera a memória alocada para a lista encadeada
    Node* temp;
    while (allocated_list.head != NULL) {
        temp = allocated_list.head;
        allocated_list.head = allocated_list.head->next;
        free(temp);
    }
    
    return total;
}


void generate_all_allocations(Vehicle* v, int vn, Package* p, int pn) {
    for (int i = 0; i < vn; i++) {
        float val = generate_allocation(v[i], p, pn);
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
