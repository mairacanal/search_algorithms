// ========================================================================================================
//
// @file exercicio1c.c
// @authors Guilherme Mafra (N USP: 11272015), Luigi Quaglio (N USP: 11800563) and Maíra Canal (N USP: 11819403)
//
// ========================================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
unsigned char typedef bool;
#define TRUE  1
#define FALSE 0

// DESENVOLVIDA PELO GRUPO ================================================================================

typedef struct {
    int* vetor;
    unsigned tamanho;
} Vetor;

// ========================================================================================================

int* ler_inteiros(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");

    int * inteiros = (int *) malloc(sizeof(int) * n);

    for (int i = 0; !feof(f); i++)
        fscanf(f, "%d\n", &inteiros[i]);

    fclose(f);

    return inteiros;
}

void inicia_tempo()
{
    srand(time(NULL));
    _ini = clock();
}

double finaliza_tempo()
{
    _fim = clock();
    return ((double) (_fim - _ini)) / CLOCKS_PER_SEC;
}

// DESENVOLVIDA PELO GRUPO ================================================================================

/*
 * @brief Funcao de busca sequencial que realoca um elemento para uma posicao anterior durante as buscas
 * @return Retorna -1 para valores não encontrados. Retorna a posicao de valores encontrados.
 */
int transposicao(Vetor *tabela, int buscado) 
{
    // guarda o valor fora do vetor para a realocacao
    int aux;

    for (int posicao = 0; posicao < tabela->tamanho; posicao++) {

        // elemento encontrado, na posicao "posicao"
        if (tabela->vetor[posicao] == buscado) {

            // troca o valor pesquisado com o anterior para deixa-lo mais proximo ao inicio e acelerar futuras pesquisas
            aux = tabela->vetor[posicao];
            tabela->vetor[posicao] = tabela->vetor[posicao - 1];
            tabela->vetor[posicao - 1] = aux;

            // busca concluida e elemento encontrado
            return posicao;

        }

    }

    return -1;
}

// ========================================================================================================

int main(int argc, char const *argv[])
{
    const int N = 50000;
    unsigned encontrados = 0;

    int* entradas = ler_inteiros("inteiros_entrada.txt", N);
    int* consultas = ler_inteiros("inteiros_busca.txt", N);

    // Inicializa a struct Vetor
    Vetor tabela;

    tabela.vetor = entradas;
    tabela.tamanho = N;

    // realiza busca sequencia com realocação
    inicia_tempo();
    for (int i = 0; i < N; i++) {

        // buscar o elemento consultas[i] na entrada
        if (transposicao(&tabela, consultas[i]) != -1)
            encontrados++;

    }
    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    free(entradas);
    free(consultas);

    return 0;
}
