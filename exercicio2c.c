// ========================================================================================================
//
// @file exercicio2c.c
// @authors Guilherme Mafra (N USP: 11272015), Luigi Quaglio (N USP: 11800563) and Maíra Canal (N USP: 11819403)
//
// ========================================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <math.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
typedef unsigned char bool;
#define TRUE  1
#define FALSE 0

// Definição do tipo string
typedef char * string;

#define MAX_STRING_LEN 20

// DESENVOLVIDA PELO GRUPO ================================================================================

typedef struct no {
    char key[MAX_STRING_LEN];
    struct no *proximo;
} No;

typedef struct {
    unsigned B;
    No **tabela;
} Hash;

// ========================================================================================================

unsigned converter(string s) {
   unsigned h = 0;
   for (int i = 0; s[i] != '\0'; i++) 
      h = h * 256 + s[i];
   return h;
}

string* ler_strings(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");
    
    string* strings = (string *) malloc(sizeof(string) * n);

    for (int i = 0; !feof(f); i++) {
        strings[i] = (string) malloc(sizeof(char) * MAX_STRING_LEN);
        fscanf(f, "%s\n", strings[i]);
    }

    fclose(f);

    return strings;
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

unsigned h_div(unsigned x, unsigned B)
{
    return x % B;
}

unsigned h_mul(unsigned x, unsigned B)
{
    const double A = 0.6180;
    return fmod(x * A, 1) * B;
}

// DESENVOLVIDA PELO GRUPO ================================================================================

/*
 * @brief Aloca memória da tabela hash e preenche todos os enderecos com NULL.
 */
void criar_hash(Hash *hash, unsigned B)
{
    hash->B = B;
    hash->tabela = (No **) malloc(B * sizeof(No*));
    for (int i = 0; i < B; i++) 
        hash->tabela[i] = NULL;
}

/*
 * @brief Insere um novo nó na lista encadeada linear
 */
void inserir_node(No** raiz, string elemento) 
{
    No* novo_no = (No*) malloc(sizeof(No));

    strcpy(novo_no->key, elemento);
    novo_no->proximo = *raiz;
    *raiz = novo_no;
}

/*
 * @brief Busca de forma recursiva um nó em uma lista encadeada
 * @return Retorna -1 caso o nó não seja encontrado e retorna 0 caso o nó seja encontrado
 */
int buscar_node(No *no, string elemento) 
{
    if (no == NULL)
        return -1;
    if (!strcmp(no->key, elemento))
        return 0;
    return buscar_node(no->proximo, elemento);
}

/*
 * @brief Insere elemento na tabela hash utilizando a funcao hash (unsigned *funcao_hash). Além disso, também
 * realiza o tratamento de colisões por meio da técnica de encadeamento em lista linear não ordenada.
 * @return Retorna 0 se não houve colisão e retorna 1 caso tenha havido colisão
 */
int inserir_hash(Hash *hash, string elemento, unsigned (*funcao_hash)(unsigned, unsigned)) 
{
    int colisao = 0;
    unsigned key = converter(elemento);
    unsigned pos = funcao_hash(key, hash->B);

    if (!buscar_node(hash->tabela[pos], elemento))
        return colisao;

    if (hash->tabela[pos] == NULL) {
        hash->tabela[pos] = (No *) malloc(sizeof(No));
        hash->tabela[pos]->proximo = NULL;
    } else colisao = 1; 

    inserir_node(&(hash->tabela[pos]), elemento);

    return colisao;
}

/*
 * @brief Busca um elemento na tabela hash utilizando a funcão hash (unsigned *funcao_hash). Conforme a funcão inserir_hash,
 * é utilizado a técnica de encadeamento em lista linear não ordenada.
 * @return Retorna -1 se o elemento não existir. Caso o elemento exista, retorna 0.
 */
int buscar_hash(Hash *hash, string elemento, unsigned (*funcao_hash)(unsigned, unsigned)) 
{
    unsigned key = converter(elemento);
    unsigned pos = funcao_hash(key, hash->B);

    return buscar_node(hash->tabela[pos], elemento);
}

/*
 * @brief Libera a memória alocada na tabela hash e nas listas encadeadas.
 */
void liberar_hash(Hash *hash)
{
    for (int i = 0; i < hash->B; i++) 
    {
        while (hash->tabela[i])
        {
            No* tmp = hash->tabela[i];
            hash->tabela[i] = hash->tabela[i]->proximo;
            free(tmp);
        }
    }
    free(hash->tabela);
}

// ========================================================================================================

int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int M = 70000;
    const int B = 150001;

    unsigned colisoes_h_div = 0;
    unsigned colisoes_h_mul = 0;

    unsigned encontrados_h_div = 0;
    unsigned encontrados_h_mul = 0;

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);

    Hash hash;

    // cria tabela hash com hash por divisão
    criar_hash(&hash, B);

    // inserção dos dados na tabela hash usando hash por divisão
    inicia_tempo();
    for (int i = 0; i < N; i++) {

        // inserir insercoes[i] na tabela hash
        if (inserir_hash(&hash, insercoes[i], &h_div))
            colisoes_h_div++;

    }
    double tempo_insercao_h_div = finaliza_tempo();

    // busca dos dados na tabela hash com hash por divisão
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        
        // buscar consultas[i] na tabela hash
        if (buscar_hash(&hash, consultas[i], &h_div) != -1)
            encontrados_h_div++;
        
    }
    double tempo_busca_h_div = finaliza_tempo();

    // destroi tabela hash com hash por divisão
    liberar_hash(&hash);

    // cria tabela hash com hash por multiplicação
    criar_hash(&hash, B);

    // inserção dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        
        // inserir insercoes[i] na tabela hash
        if (inserir_hash(&hash, insercoes[i], &h_mul))
            colisoes_h_mul++;

    }
    double tempo_insercao_h_mul = finaliza_tempo();

    // busca dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        
        // buscar consultas[i] na tabela hash
        if (buscar_hash(&hash, consultas[i], &h_mul) != -1)
            encontrados_h_mul++;

    }
    double tempo_busca_h_mul = finaliza_tempo();

    // destroi tabela hash com hash por multiplicação
    liberar_hash(&hash);

    printf("Hash por Divisão\n");
    printf("Colisões na inserção: %d\n", colisoes_h_div);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_div);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_div);
    printf("Itens encontrados   : %d\n", encontrados_h_div);
    printf("\n");
    printf("Hash por Multiplicação\n");
    printf("Colisões na inserção: %d\n", colisoes_h_mul);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_mul);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_mul);
    printf("Itens encontrados   : %d\n", encontrados_h_mul);

    // Desaloca memória previamente alocada
    free(insercoes);
    free(consultas);

    return 0;
}
