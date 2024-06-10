#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int Posicao;
    char Nome[50];
} Cidade;

typedef struct {
    int T;
    int N;
    Cidade *C;
} Estrada;

Estrada *getEstrada(const char *arquivo) {
    FILE *entrada = fopen(arquivo, "r");
    if (entrada == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    Estrada *novaEstrada = (Estrada *)malloc(sizeof(Estrada));
    if (novaEstrada == NULL) {
        printf("Erro ao criar uma nova estrada.\n");
        exit(1);
    }

    fscanf(entrada, "%d\n", &novaEstrada->T);
    fscanf(entrada, "%d\n", &novaEstrada->N);

    if (novaEstrada->T < 3 || novaEstrada->T > 1000000) return NULL;
    if (novaEstrada->N < 2 || novaEstrada->N > 10000) return NULL;

    novaEstrada->C = (Cidade *)malloc(novaEstrada->N * sizeof(Cidade));

    int index;
    for (index = 0; index < novaEstrada->N; index++) {
        fscanf(entrada, "%d %[^\n]s", &novaEstrada->C[index].Posicao, novaEstrada->C[index].Nome);
        if (novaEstrada->C[index].Posicao > novaEstrada->T) return NULL;
    }

    fclose(entrada);
    return novaEstrada;
}

void mostrarEstrada(Estrada *estradaAtual) {

    int index;
    for (index = 0; index < estradaAtual->N; index++) {
        printf("%d %s\n", estradaAtual->C[index].Posicao, estradaAtual->C[index].Nome);
    }
}

void sortEstradaPorDist(Estrada *estradaAtual) {

    int i;
    for (i = 0; i < estradaAtual->N; i++) {
        int j;
        for (j = i + 1; j < estradaAtual->N; j++) {
            if (estradaAtual->C[i].Posicao > estradaAtual->C[j].Posicao) {
                Cidade temp = estradaAtual->C[i];
                estradaAtual->C[i] = estradaAtual->C[j];
                estradaAtual->C[j] = temp;
            }
        }
    }
}

double calcularMenorVizinhanca(const char *arquivo) {
    Estrada *estradaAtual = getEstrada(arquivo);
    sortEstradaPorDist(estradaAtual);

    double minValue = estradaAtual->T + 1;

    int i;
    for (i = 0; i < estradaAtual->N; i++) {
        double val = 0;

        if (i == 0) {
            val += estradaAtual->C[i].Posicao;
            val += (double)(estradaAtual->C[i + 1].Posicao - estradaAtual->C[i].Posicao) / 2;
        } else {
            val += (double)(estradaAtual->C[i].Posicao - estradaAtual->C[i - 1].Posicao) / 2;
            if (i == estradaAtual->N - 1) {
                val += (double)(estradaAtual->T - estradaAtual->C[i].Posicao);
            } else {
                val += (double)(estradaAtual->C[i + 1].Posicao - estradaAtual->C[i].Posicao) / 2;
            }
        }

        if (val < minValue) {
            minValue = val;
        }
    }

    free(estradaAtual->C);
    free(estradaAtual);

    return minValue;
}

char *cidadeMenorVizinhanca(const char *arquivo) {
    Estrada *estradaAtual = getEstrada(arquivo);
    sortEstradaPorDist(estradaAtual);

    double minValue = estradaAtual->T + 1;
    char *minCidade = NULL;

    int i;
    for (i = 0; i < estradaAtual->N; i++) {
        double val = 0;

        if (i == 0) {
            val += estradaAtual->C[i].Posicao;
            val += (double)(estradaAtual->C[i + 1].Posicao - estradaAtual->C[i].Posicao) / 2;
        } else {
            val += (double)(estradaAtual->C[i].Posicao - estradaAtual->C[i - 1].Posicao) / 2;
            if (i == estradaAtual->N - 1) {
                val += (double)(estradaAtual->T - estradaAtual->C[i].Posicao);
            } else {
                val += (double)(estradaAtual->C[i + 1].Posicao - estradaAtual->C[i].Posicao) / 2;
            }
        }

        if (val < minValue) {
            minValue = val;
            minCidade = estradaAtual->C[i].Nome;
        }
    }

    free(estradaAtual->C);
    free(estradaAtual);

    return minCidade;
}

int main() {
    Estrada *testando = getEstrada("arquivoteste.txt");

    printf("tamanho da estrada = %d\n", testando->T);
    printf("quantidade de cidades = %d\n", testando->N);
    printf("\n");
    mostrarEstrada(testando);
    printf("\n");
    printf("menor vizinhança = %.2lf\n", calcularMenorVizinhanca("arquivoteste.txt"));
    printf("cidade com a menor vizinhança = %s\n", cidadeMenorVizinhanca("arquivoteste.txt"));

    free(testando->C);
    free(testando);

    return 0;
}
