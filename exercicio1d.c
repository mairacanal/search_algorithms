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
    int kindex;
    int pos;
} TabelaIndice;

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

int maximo_elemento(int* vetor, unsigned tamanho) 
{
    int max = 0;

    for (int i = 0; i < tamanho; i++) 
        if(vetor[i] > max) max = vetor[i];

    return max;
}

void radix_counting_sort(int* vetor, unsigned tamanho, int posicao) 
{
    int k = 10;
    long *contagem = (long *) calloc(k, sizeof(long));
    long *final = (long *) calloc(tamanho, sizeof(long));
    long chave;

    for(int i = 0; i < tamanho; i++) 
    {
        chave = vetor[i]/posicao;
        chave = chave % k;
        contagem[chave]++;
    }

    for(int i = 1; i < k; i++)
        contagem[i] += contagem[i - 1];

    for(int i = (tamanho - 1); i >= 0; i--) 
    {
        chave = vetor[i]/posicao;
        chave = chave % k;
        contagem[chave]--;
        final[contagem[chave]] = vetor[i];
    }

    for(int i = 0; i < tamanho; i++)
        vetor[i] = final[i];

    free(contagem);
    free(final);
}

void radix_sort(int* vetor, unsigned tamanho) 
{
    int max = maximo_elemento(vetor, tamanho);

    for (int posicao = 1; max/posicao > 0; posicao *= 10)
        radix_counting_sort(vetor, tamanho, posicao);
}

TabelaIndice* criar_TabelaIndice(int* tabela, int N, int T) 
{
    TabelaIndice* tabelaIndice;

    tabelaIndice = (TabelaIndice *) malloc(T * sizeof(TabelaIndice));

    for (int i = 0; i < T; i++) {
        tabelaIndice[i].pos = i * (N / T);
        tabelaIndice[i].kindex = tabela[tabelaIndice[i].pos];
    }

    return tabelaIndice;
}

int busca_sequencial_indexada(int* tabela, TabelaIndice* tabelaIndice, int elemento, int tamanho, int T) 
{
    int i;

    for (i = 0; i < T && elemento > tabelaIndice[i].kindex; i++);

    if (i == 0) 
        return (elemento == tabelaIndice[i].kindex) ? i : -1;

    // Busca sequencial no vetor de elemento a partir da posicao indexada
    for (i = tabelaIndice[i - 1].pos; i < tamanho; i++)
        if (elemento == tabela[i]) return i;

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

    // ordenar entrada
    radix_sort(entradas, N);

    // criar tabela de indice
    tabelaIndice = criar_TabelaIndice(entradas, N, T);

    // realizar consultas na tabela de indices 
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // buscar o elemento consultas[i] na entrada
        if (busca_sequencial_indexada(entradas, tabelaIndice, consultas[i], N, T) != -1) 
            encontrados++;
    }
    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    free(entradas);
    free(consultas);
    free(tabelaIndice);

    return 0;
}
