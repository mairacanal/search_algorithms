// ========================================================================================================
//
// @file exercicio1d.c
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

/*
 * @brief Struct que representa a Tabela de índices que contém uma chave (int kindex) e um indicador de 
 * registro no arquivo que corresponde a chave (int pos)
 */
typedef struct {
    int kindex;
    int pos;
} TabelaIndice;

typedef struct {
    int* vetor;
    int tamanho;
} Vetor;

// ========================================================================================================

int* ler_inteiros(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");

    int* inteiros = (int *) malloc(sizeof(int) * n);

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
 * @brief Percorre o vetor linearmente buscando o maior elemento do vetor
 * @return Retorna o maior elemento do vetor
 */
int maximo_elemento(Vetor *tabela) 
{
    int max = 0;

    for (int i = 0; i < tabela->tamanho; i++) 
        if(tabela->vetor[i] > max) max = tabela->vetor[i];

    return max;
}

/*
 * @brief Funcão auxiliar do Radix Sort, a qual ordena individualmente cada dígito por meio do algoritmo de
 * ordenacão Counting Sort
 */
void radix_counting_sort(Vetor *tabela, int posicao) 
{
    int k = 10;
    long *contagem = (long *) calloc(k, sizeof(long));
    long *final = (long *) calloc(tabela->tamanho, sizeof(long));
    long chave;

    for(int i = 0; i < tabela->tamanho; i++) 
    {
        chave = tabela->vetor[i]/posicao;
        chave = chave % k;
        contagem[chave]++;
    }

    for(int i = 1; i < k; i++)
        contagem[i] += contagem[i - 1];

    for(int i = (tabela->tamanho - 1); i >= 0; i--) 
    {
        chave = tabela->vetor[i]/posicao;
        chave = chave % k;
        contagem[chave]--;
        final[contagem[chave]] = tabela->vetor[i];
    }

    for(int i = 0; i < tabela->tamanho; i++)
        tabela->vetor[i] = final[i];

    free(contagem);
    free(final);
}

/*
 * @brief Método de Ordenacão Radix Sort, utilizado para ordenar chaves em domínios discretos
 */
void radix_sort(Vetor *tabela) 
{
    int max = maximo_elemento(tabela);

    for (int posicao = 1; max/posicao > 0; posicao *= 10)
        radix_counting_sort(tabela, posicao);
}

/*
 * @brief Aloca a memória da Tabela de Indices e preenche a tabela com os elementos 0, N/T, 2 * N/T, etc. 
 * Dessa forma, cada elemento da Tabela de Indices representa N/T elementos da tabela.
 * @return Retorna a Tabela de Indices
 */
void criar_TabelaIndice(TabelaIndice **tabelaIndice, Vetor* tabela, int T) 
{
    *tabelaIndice = (TabelaIndice *) malloc(T * sizeof(TabelaIndice));

    for (int i = 0; i < T; i++) {
        (*tabelaIndice)[i].pos = i * (tabela->tamanho / T);
        (*tabelaIndice)[i].kindex = tabela->vetor[(*tabelaIndice)[i].pos];
    }
}

/*
 * @brief Realiza a busca sequencial indexada
 * @return Retorna -1 em caso de não encontrar o elemento. Caso contrário, retorna a posicão do elemento.
 */
int busca_sequencial_indexada(Vetor* tabela, TabelaIndice* tabelaIndice, int elemento, int T) 
{
    int i;

    for (i = 0; i < T && elemento > tabelaIndice[i].kindex; i++);

    if (i == 0) 
        return (elemento == tabelaIndice[i].kindex) ? i : -1;

    // Busca sequencial no vetor de elemento a partir da posicao indexada
    for (i = tabelaIndice[i - 1].pos; i < tabela->tamanho; i++)
        if (elemento == tabela->vetor[i]) return i;

    return -1;

}

// ========================================================================================================

int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int index_size = 10000;
    const int T = N/index_size;
    unsigned encontrados = 0;

    int* entradas = ler_inteiros("inteiros_entrada.txt", N);
    int* consultas = ler_inteiros("inteiros_busca.txt", N);
    
    TabelaIndice* tabelaIndice;
    Vetor tabela;

    // Inicializando tabela de busca
    tabela.tamanho = N;
    tabela.vetor = entradas;

    // ordenar entrada
    radix_sort(&tabela);

    // criar tabela de indice
    criar_TabelaIndice(&tabelaIndice, &tabela, T);

    // realizar consultas na tabela de indices 
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // buscar o elemento consultas[i] na entrada
        if (busca_sequencial_indexada(&tabela, tabelaIndice, consultas[i], T) != -1) 
            encontrados++;
    }
    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    // Liberando memória alocada
    free(entradas);
    free(consultas);
    free(tabelaIndice);

    return 0;
}
