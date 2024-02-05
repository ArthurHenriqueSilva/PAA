#include <stdio.h>
#include <stdlib.h>


typedef struct{
    char placa[7];
    int peso;
    int volume;
}Veiculo;

typedef struct{
    char codigo[13];
    float valor;
    int peso;
    int volume;
}Pacote;

int read_number(FILE *file){
    int x;
    fscanf(file, "%d", &x);
    return x;
}

Veiculo read_veiculo(FILE *file){
    Veiculo v;
    fscanf(file, "%7s %d %d", v.placa, &v.peso, &v.volume);
    return v;
}

void print_veiculos(Veiculo v[], int size){
    for(int i = 0; i < size; i++){
        printf("Veiculo %d: %s, %d, %d\n", i+1, v[i].placa, v[i].peso, v[i].volume);
    }

}

Pacote read_pacote(FILE *file){
    Pacote p;
    fscanf(file, "%13s %f %d %d", p.codigo, &p.valor, &p.peso, &p.volume);
    return p;
}

void print_pacotes(Pacote p[], int size){
    for(int i = 0; i < size; i++) {
        printf("Pacote %d: %s, %.2f, %d, %d\n",i+1, p[i].codigo, p[i].valor, p[i].peso, p[i].volume);
    }
}
int main(int argc, char *argv[]){
    FILE *input = fopen(argv[1], "r");
    int number_v = read_number(input);
    Veiculo veiculos[number_v];
    for(int i = 0; i < number_v; i++) veiculos[i] =  read_veiculo(input);
    int number_p = read_number(input);
    Pacote pacotes[number_p];
    for(int i = 0; i < number_p; i++) pacotes[i] = read_pacote(input);
    fclose(input);
    return 0;
    
}