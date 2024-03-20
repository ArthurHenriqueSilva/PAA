#include <stdlib.h>
#include <stdio.h>

typedef struct File
{
    FILE* input;
    FILE* output;
}File;

typedef struct Vehicle
{
    char* plate[7];
    int weight;
    int volume;
}Vehicle;

typedef struct Package
{
    char* code[13];
    float value;
    int weight;
    int volume;
}Package;

File openFiles(char* input[], char* output)
{
    File file;
    FILE* open_input = fopen(input, "r");
    FILE* open_output = fopen(input, "w");
    file.input = open_input;
    file.output = open_output;
}

int read_number(FILE *file){
    int x;
    fscanf(file, "%d", &x);
    return x;
}

Vehicle* create_vehicle(FILE* input)
{
    Vehicle* vehicle;
    fscanf(input, "%s %d %d", vehicle->plate, vehicle->weight, vehicle->volume);
    return vehicle;
}

Vehicle* create_vehicle_arr(FILE* input, int quantity)
{
    Vehicle* vs = (Vehicle*)malloc(quantity*sizeof(Vehicle));
    for(int i = 0; i < quantity; i++) vs[i] = *create_vehicle(input);
    return vs;
}

Package* create_package(File* input)
{
    Package* package;
    fscanf(input, "%s %.2f %d %d", package->code, package->value, package->weight, package->volume);
    return package;
}

Package* create_package_arr(FILE* input, int quantity)
{
    Package* ps = (Package*)malloc(quantity*sizeof(Package));
    for(int i = 0; i < quantity; i++) ps[i] = *create_package(input);
    return ps;
}

void run_tasks(FILE* input, FILE* output)
{
    int v_num = read_number(input);
    Vehicle* vs = create_vehicle_arr(input, v_num);
    int p_num = read_number(input);
    Package* ps = create_package_arr(input, p_num);
}

int main(int argc, char* argv[])
{
    File file = openFiles(argv[1], argv[2]);
}