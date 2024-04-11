#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLATE_LENGTH 8
#define MAX_CODE_LENGTH 14
#define MAX_PACKAGES 100

typedef struct {
    char code[MAX_CODE_LENGTH];
    float value;
    int weight;
    int volume;
    int allocated; // Flag para verificar se o pacote já foi alocado
} Package;

typedef struct {
    char plate[MAX_PLATE_LENGTH];
    int weight_capacity;
    int volume_capacity;
    float max_value;
    Package* packages[MAX_PACKAGES];
    int package_count;
} Vehicle;

// Função para ler os dados dos veículos do arquivo de entrada
void read_vehicles(FILE* input, Vehicle* vehicles, int num_vehicles) {
    for (int i = 0; i < num_vehicles; i++) {
        fscanf(input, "%s %d %d", vehicles[i].plate, &vehicles[i].weight_capacity, &vehicles[i].volume_capacity);
        vehicles[i].max_value = 0.0f; // Inicializa o valor máximo transportado como 0
        vehicles[i].package_count = 0;
    }
}

// Função para ler os dados dos pacotes do arquivo de entrada
void read_packages(FILE* input, Package* packages, int num_packages) {
    for (int i = 0; i < num_packages; i++) {
        fscanf(input, "%s %f %d %d", packages[i].code, &packages[i].value, &packages[i].weight, &packages[i].volume);
        packages[i].allocated = 0; // Inicializa a flag de alocação como 0
    }
}

// Função para alocar os pacotes nos veículos
void allocate_packages(Vehicle* vehicles, int num_vehicles, Package* packages, int num_packages) {
    for (int i = 0; i < num_vehicles; i++) {
        Vehicle* vehicle = &vehicles[i];
        float remaining_weight = vehicle->weight_capacity;
        float remaining_volume = vehicle->volume_capacity;

        for (int j = 0; j < num_packages; j++) {
            Package* pack = &packages[j];
            if (!pack->allocated && pack->weight <= remaining_weight && pack->volume <= remaining_volume) {
                remaining_weight -= pack->weight;
                remaining_volume -= pack->volume;
                vehicle->max_value += pack->value;
                vehicle->packages[vehicle->package_count++] = pack;
                pack->allocated = 1;
            }
        }
    }
}

// Função para imprimir a sequência de carregamento dos pacotes para cada veículo
void print_vehicle_loading_sequence(FILE* output, Vehicle* vehicles, int num_vehicles, Package* packages, int num_packages) {
    for (int i = 0; i < num_vehicles; i++) {
        Vehicle* vehicle = &vehicles[i];
        float remaining_weight = vehicle->weight_capacity;
        float remaining_volume = vehicle->volume_capacity;
        
        fprintf(output, "%d [ %s ] R$%.2f ,%.0f KG (%.0f%%) ,%.0f L (%.0f%%)\n", i + 1, vehicle->plate,
                vehicle->max_value, remaining_weight, (vehicle->weight_capacity - remaining_weight) / vehicle->weight_capacity * 100,
                remaining_volume, (vehicle->volume_capacity - remaining_volume) / vehicle->volume_capacity * 100);

        for (int j = 0; j < vehicle->package_count; j++) {
            Package* pack = vehicle->packages[j];
            fprintf(output, "%s\n", pack->code);
        }
    }
    
    for (int j = 0; j < num_packages; j++) {
        Package* pack = &packages[j];
        if (!pack->allocated) {
            fprintf(output, "[%s] R$%.2f ,%d KG ,%d L\n", pack->code, pack->value, pack->weight, pack->volume);
        }
    }
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

    int num_vehicles, num_packages;
    fscanf(input, "%d", &num_vehicles);

    Vehicle* vehicles = malloc(num_vehicles * sizeof(Vehicle));
    read_vehicles(input, vehicles, num_vehicles);

    fscanf(input, "%d", &num_packages);
    Package* packages = malloc(num_packages * sizeof(Package));
    read_packages(input, packages, num_packages);

    allocate_packages(vehicles, num_vehicles, packages, num_packages);
    print_vehicle_loading_sequence(output, vehicles, num_vehicles, packages, num_packages);

    fclose(input);
    fclose(output);

    free(vehicles);
    free(packages);

    return 0;
}
