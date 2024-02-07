#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição das structs
typedef struct {
    char placa[9]; // Aumentado para acomodar "PENDENTE"
    int peso;
    int volume;
} Veiculo;

typedef struct {
    char codigo[13];
    float valor;
    int peso;
    int volume;
    int usado; // Flag para marcar se o pacote já foi alocado
} Pacote;

// Função que implementa o algoritmo de knapsack
void knapsack(Veiculo veiculo, Pacote pacotes[], int num_pacotes) {
    int** dp;
    dp = (int**)malloc((num_pacotes + 1) * sizeof(int*));

    for (int i = 0; i <= num_pacotes; i++)
        dp[i] = (int*)malloc((veiculo.peso + 1) * sizeof(int));

    for (int i = 0; i <= num_pacotes; i++) {
        for (int j = 0; j <= veiculo.peso; j++) {
            if (i == 0 || j == 0)
                dp[i][j] = 0;
            else if (pacotes[i - 1].peso <= j && pacotes[i - 1].volume <= veiculo.volume)
                dp[i][j] = (pacotes[i - 1].valor + dp[i - 1][j - pacotes[i - 1].peso] > dp[i - 1][j]) ? 
                            (pacotes[i - 1].valor + dp[i - 1][j - pacotes[i - 1].peso]) : dp[i - 1][j];
            else
                dp[i][j] = dp[i - 1][j];
        }
    }

    // Recupera os pacotes escolhidos
    int peso_atual = veiculo.peso;
    int volume_atual = veiculo.volume;
    float valor_acumulado = 0;

    // Imprime os valores acumulados dos pacotes
    if (strcmp(veiculo.placa, "PENDENTE") != 0) {
        for (int i = num_pacotes; i >= 1; i--) {
            if (dp[i][peso_atual] != dp[i - 1][peso_atual] && !pacotes[i - 1].usado) {
                valor_acumulado += pacotes[i - 1].valor;
                pacotes[i - 1].usado = 1; // Marca o pacote como usado
                peso_atual -= pacotes[i - 1].peso;
                volume_atual -= pacotes[i - 1].volume;
            }
        }
        printf("[%s] R$%.2f, %d KG (%.0f%%), %d L (%.0f%%)\n", veiculo.placa, valor_acumulado, 
               (veiculo.peso - peso_atual), (float)(veiculo.peso - peso_atual) / veiculo.peso * 100, 
               (veiculo.volume - volume_atual), (float)(veiculo.volume - volume_atual) / veiculo.volume * 100);
    } else {
        // Imprime os valores acumulados dos pacotes sem a porcentagem
        for (int i = num_pacotes; i >= 1; i--) {
            if (dp[i][peso_atual] != dp[i - 1][peso_atual] && !pacotes[i - 1].usado) {
                valor_acumulado += pacotes[i - 1].valor;
                pacotes[i - 1].usado = 1; // Marca o pacote como usado
                peso_atual -= pacotes[i - 1].peso;
                volume_atual -= pacotes[i - 1].volume;
            }
        }
        printf("[%s] R$%.2f, %d KG, %d L\n", veiculo.placa, valor_acumulado, 
               (veiculo.peso - peso_atual), (veiculo.volume - volume_atual));
    }

    // Imprime os códigos dos pacotes escolhidos
    for (int i = 0; i < num_pacotes; i++) {
        if (pacotes[i].usado) {
            printf("%s\n", pacotes[i].codigo);
            pacotes[i].usado = 0; // Resetando a flag usado
        }
    }

    // Libera memória alocada para a tabela dp
    for (int i = 0; i <= num_pacotes; i++)
        free(dp[i]);
    free(dp);
}

// Função para ler um número do arquivo de entrada
int read_number(FILE *input) {
    int number;
    fscanf(input, "%d", &number);
    return number;
}

// Função para ler um veículo do arquivo de entrada
Veiculo read_veiculo(FILE *input) {
    Veiculo veiculo;
    fscanf(input, "%s %d %d", veiculo.placa, &veiculo.peso, &veiculo.volume);
    return veiculo;
}

// Função para ler um pacote do arquivo de entrada
Pacote read_pacote(FILE *input) {
    Pacote pacote;
    fscanf(input, "%s %f %d %d", pacote.codigo, &pacote.valor, &pacote.peso, &pacote.volume);
    pacote.usado = 0; // Inicialmente, o pacote não foi usado
    return pacote;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        printf("Error: Could not open input file %s\n", argv[1]);
        return 1;
    }

    int number_v = read_number(input);
    Veiculo veiculos[number_v + 1]; // +1 para o veículo PENDENTE
    for(int i = 0; i < number_v; i++) veiculos[i] =  read_veiculo(input);
    strcpy(veiculos[number_v].placa, "PENDENTE");
    veiculos[number_v].peso = veiculos[number_v].volume = 999999; // Valor arbitrário grande

    int number_p = read_number(input);
    Pacote pacotes[number_p];
    for(int i = 0; i < number_p; i++) pacotes[i] = read_pacote(input);

    fclose(input);

    // Aplica o algoritmo de knapsack para cada veículo
    for (int i = 0; i <= number_v; i++) {
        knapsack(veiculos[i], pacotes, number_p);
        printf("\n");
    }

    return 0;
}
