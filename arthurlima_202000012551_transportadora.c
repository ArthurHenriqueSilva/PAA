// Includes de bibliotecas necessárias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição de constantes
#define MAX_PLATE_LENGTH 9
#define MAX_CODE_LENGTH 13
#define MAX_WEIGHT 999999

// Definição das estruturas de dados
typedef struct {
    char placa[MAX_PLATE_LENGTH];
    int peso;
    int volume;
} Veiculo;

typedef struct {
    char codigo[MAX_CODE_LENGTH];
    float valor;
    int peso;
    int volume;
    int usado;
} Pacote;

typedef struct Node {
    char codigo[MAX_CODE_LENGTH];
    struct Node *next;
} Node;

typedef struct {
    char placa[MAX_CODE_LENGTH];
    float valor;
    int peso_total;
    int peso_maximo;
    int volume_total;
    int volume_maximo;
    Node *pacotes; // Lista encadeada para armazenar os códigos dos pacotes alocados
    int num_pacotes; // Número de pacotes alocados
} Alocacao;

// Protótipos de funções
int max(int a, int b);
Alocacao alocar_pacotes(Veiculo veiculo, Pacote pacotes[], int num_pacotes);
void print_alocacao(FILE *output, Alocacao a);
int read_number(FILE *input);
Veiculo read_veiculo(FILE *input);
Pacote read_pacote(FILE *input);
void print_veiculo(FILE *output, Veiculo v);
void print_pacote(FILE *output, Pacote p);
int my_round(float x);

// Função principal
int main(int argc, char *argv[]) {

    // Abertura do arquivo de entrada
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    printf("Opened files: %s / %s\n", argv[1], argv[2]);

    // Leitura do número de veículos
    int number_v = read_number(input);
    printf("Total de %d veiculos.\n", number_v);

    // Alocação de memória para os veículos
    Veiculo *veiculos = malloc(number_v * sizeof(Veiculo));
    // Leitura dos veículos
    for (int i = 0; i < number_v; i++)
        veiculos[i] = read_veiculo(input);

    // Leitura do número de pacotes
    int number_p = read_number(input);
    printf("Total de %d pacotes.\n", number_p);
    // Alocação de memória para os pacotes
    Pacote *pacotes = malloc(number_p * sizeof(Pacote));
    // Leitura dos pacotes
    for (int i = 0; i < number_p; i++)
        pacotes[i] = read_pacote(input);

    // Fechamento do arquivo de entrada
    fclose(input);

    // Aplicação do algoritmo alocar_pacotes para cada veículo
    for (int i = 0; i < number_v; i++) {
        Alocacao a = alocar_pacotes(veiculos[i], pacotes, number_p);
        print_alocacao(output, a);
        fprintf(output, "\n");
    }

    // Verificação de pacotes pendentes
    int weight_pendente = 0;
    int volume_pendente = 0;
    float valor_pendente = 0.0;
    fprintf(output, "[PENDENTE]");
    for (int i = 0; i < number_p; i++) {
        if (!pacotes[i].usado) {
            weight_pendente += pacotes[i].peso;
            volume_pendente += pacotes[i].volume;
            valor_pendente += pacotes[i].valor;
        }
    }
    fprintf(output, "%.2fR$, ", valor_pendente);
    fprintf(output, "%dkg, ", weight_pendente);
    fprintf(output, "%dL\n", volume_pendente);

    // Impressão dos códigos dos pacotes pendentes
    for(int i = 0; i < number_p; i++){
        if (!pacotes[i].usado) fprintf(output, "%s\n", pacotes[i].codigo);
    } 

    // Liberação de memória alocada
    free(veiculos);
    free(pacotes);

    // Fechamento do arquivo de saída
    fclose(output);

    return 0;
}

// Função de comparação para o máximo entre dois números
int max(int a, int b) {
    return (a > b) ? a : b;    
}

// Função para alocação de pacotes em um veículo
Alocacao alocar_pacotes(Veiculo veiculo, Pacote pacotes[], int num_pacotes) {
    // Inicialização de variáveis
    int W = veiculo.peso; // Peso máximo do veículo
    int V = veiculo.volume; // Volume máximo do veículo
    int n = num_pacotes;
    float ***k = malloc((n + 1) * sizeof(float **));
    for (int i = 0; i <= n; i++) {
        k[i] = malloc((W + 1) * sizeof(float *));
        for (int j = 0; j <= W; j++) {
            k[i][j] = malloc((V + 1) * sizeof(float));
        }
    }

    // Inicialização da matriz
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            for (int v = 0; v <= V; v++) {
                if (i == 0 || w == 0 || v == 0)
                    k[i][w][v] = 0;
                else if (pacotes[i-1].peso <= w && pacotes[i-1].volume <= v)
                    k[i][w][v] = max(pacotes[i-1].valor + k[i-1][w - pacotes[i-1].peso][v - pacotes[i-1].volume], k[i-1][w][v]);
                else
                    k[i][w][v] = k[i-1][w][v];
            }
        }
    }

    // Cálculo do valor máximo que pode ser levado pelo veículo
    float res = k[n][W][V];
    // Inicialização da estrutura de alocação
    Alocacao a;
    strcpy(a.placa, veiculo.placa);
    a.valor = res;
    a.peso_maximo = veiculo.peso;
    a.volume_maximo = veiculo.volume;
    a.peso_total = 0;
    a.volume_total = 0;
    a.num_pacotes = 0;
    a.pacotes = NULL;

    // Variável auxiliar para percorrer a lista de pacotes alocados
    Node *current = NULL;
    // Loop para determinar os pacotes alocados
    for (int i = n, w = veiculo.peso, v = veiculo.volume; i > 0 && res > 0; i--) {
        if (res != k[i-1][w][v] && pacotes[i-1].peso <= w && pacotes[i-1].volume <= v && !pacotes[i-1].usado) {
            Node *new_node = malloc(sizeof(Node));
            strcpy(new_node->codigo, pacotes[i-1].codigo);
            new_node->next = NULL;
            if (a.pacotes == NULL) {
                a.pacotes = new_node;
                current = a.pacotes;
            } else {
                current->next = new_node;
                current = current->next;
            }
            a.num_pacotes++;
            pacotes[i-1].usado = 1; // Marcando o pacote como usado
            res -= pacotes[i-1].valor;
            w -= pacotes[i-1].peso;
            v -= pacotes[i-1].volume;
            a.volume_total += pacotes[i-1].volume;
            a.peso_total += pacotes[i-1].peso;
        }
    }

    // Liberação de memória alocada para a matriz auxiliar
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= W; j++) {
            free(k[i][j]);
        }
        free(k[i]);
    }
    free(k);

    return a;
}

// Função para imprimir informações de alocação
void print_alocacao(FILE *output, Alocacao a) {
    fprintf(output, "[%s] ", a.placa);
    fprintf(output, "%.2fR$, ", a.valor);
    
    // Cálculo da porcentagem de peso e volume alocados
    int peso_percent = (int)my_round(((float)a.peso_total / a.peso_maximo) * 100);
    int volume_percent = (int)my_round(((float)a.volume_total / a.volume_maximo) * 100);

    fprintf(output, "%dkg(%d%%), ", a.peso_total, peso_percent);
    fprintf(output, "%dL (%d%%)\n", a.volume_total, volume_percent);
    
    // Impressão dos códigos dos pacotes alocados
    Node *current = a.pacotes;
    while (current != NULL) {
        fprintf(output, "%s\n", current->codigo);
        current = current->next;
    }
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

// Função para imprimir informações de um veículo
void print_veiculo(FILE *output, Veiculo v) {
    fprintf(output, "[%s] %dkg %dL\n", v.placa, v.peso, v.volume);
}

// Função para imprimir informações de um pacote
void print_pacote(FILE *output, Pacote p) {
    fprintf(output, "  [%s] %dkg %dL %.2fR$\n", p.codigo, p.peso, p.volume, p.valor);
}

// Função para arredondar um número float para o inteiro mais próximo
int my_round(float x){
    return (int)(x+0.5);
}
