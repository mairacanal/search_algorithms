// ========================================================================================================
//
// @file exercicio1a.c
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
 * @brief Funcão que realiza uma busca sequencial simples no vetor (int* Vetor->vetor) de tamanho (unsigned 
 * Vetor->tamanho) buscando o elemento (int elemento)
 * @return Retorna -1 se o elemento não existir no vetor e, caso contrário, retorna a posicão do elemento no 
 * vetor
*/

int busca_sequencial_simples(Vetor *tabela, int elemento) 
{
    for (int i = 0; i < tabela->tamanho; i++) 
        if (tabela->vetor[i] == elemento)
            return i;
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

    // realiza busca sequencial
    inicia_tempo();
    for (int i = 0; i < N; i++) {

        // buscar o elemento consultas[i] na entrada
        if (busca_sequencial_simples(&tabela, consultas[i]) != -1)
            encontrados++;

    }
    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    // Liberando memória alocada
    free(entradas);
    free(consultas);

    return 0;
}
