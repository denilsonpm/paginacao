#include <stdlib.h>
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

/* Executa o LRU, dado um ponteiro para sequencia *seq*, o tamanho da sequencia *n*,
número de quadros *n_quadros* e um ponteiro para um Traco *t*. Retorna 0 se sucesso.

Exemplos:
int sequencia[] = {1, 2, 3, 3, 1, 2, 5, 1, 2, 3, 4, 5};
int tamanho_seq = 12;
int n_quadros = 3;
Traco t;
int total_refs_previstas = 4;
iniciar_traco(&t,total_refs_previstas,n_quadros,1);
executar_lru(sequencia, tamanho_seq, n_quadros, &t);
destruir_traco(&t);
*/
int executar_lru(const int *seq, int n, int n_quadros, Traco *t) {
    Memoria *m = criar_memoria(n_quadros);

    LRU l = {0};

    l.cap_hash = n_quadros * 2 + 1;
    l.tabela = calloc(l.cap_hash, sizeof(No *));


    for (int i = 0; i < n; i++) {
        int pag = seq[i];
        No *no = lru_buscar(&l, pag);

        int falta = (no == NULL);
        int vitima = VAZIO;

        if (no != NULL) {
            lru_atualizar(&l, no);
            m->q[no->indice].ultimo_uso = i;

        } else if (m->ocupados < n_quadros) {
            int idx = m->ocupados;

            lru_inserir(&l, pag, idx);

            m->q[idx].pagina = pag;
            m->q[idx].ultimo_uso = i;
            m->ocupados++;

        } else {
            No *removido = lru_escolher_vitima(&l);

            vitima = removido->pagina;

            lru_substituir(&l, m, removido, pag, i);
        }

        registrar_passo(t, pag, m, falta, vitima);
    }

    lru_destruir(&l);
    destruir_memoria(m);

    return 0;
}
/* Calcula o índice hash para uma dada página *pagina* com base na capacidade *cap* da tabela

Exemplos:
int cap = 2;
int pagina = 10;
int h = hash(pagina, cap);
5
*/
int hash(int pagina, int cap) {
    return (int)((unsigned int)pagina % (unsigned int)cap);
}

/* Busca por uma página *pagina* na estrutura LRU via ponteiro *l* utilizando a tabela hash.
Retorna um ponteiro para o No correspondente se encontrar, se não NULL.

Exemplos:
LRU l;
l.cap_hash = 5;
l.ini = NULL;
l.fim = NULL;
l.tabela = calloc(l.cap_hash, sizeof(No *));
lru_inserir(&l, 9, 0);
No *encontrado = lru_buscar(&l, 9);
free(l.tabela);
*/
No *lru_buscar(LRU *l, int pagina) {
    int h = hash(pagina, l->cap_hash);
    No *n = l->tabela[h];

    while (n != NULL) {
        if (n->pagina == pagina)
            return n;

        n = n->prox_hash;
    }

    return NULL;
}
/* Insere um nó *n* na tabela hash estrutura LRU via ponteiro *l* 

Exemplos:
No *n = malloc(sizeof(No));
n->pagina = 9;
n->prox_hash = NULL;
hash_inserir(&l, n);
*/
void hash_inserir(LRU *l, No *n) {
    int h = hash(n->pagina, l->cap_hash);

    n->prox_hash = l->tabela[h];
    l->tabela[h] = n;
}

/* Remove um nó *n* na tabela hash estrutura LRU via ponteiro *l* 

Exemplos:
No *n = malloc(sizeof(No));
n->pagina = 9;
n->prox_hash = NULL;
hash_inserir(&l, n);
hash_remover(&l, n);
*/
void hash_remover(LRU *l, No *n) {
    int h = hash(n->pagina, l->cap_hash);
    No **atual = &l->tabela[h];

    while (*atual != NULL) {
        if (*atual == n) {
            *atual = n->prox_hash;
            n->prox_hash = NULL;
            return;
        }

        atual = &(*atual)->prox_hash;
    }
}

/* Remove um nó *n* da lista duplamente encadeada via ponteiro *l* da estrutura LRU 

Exemplos:
LRU l;
l.ini = NULL;
l.fim = NULL;
No *n = malloc(sizeof(No));
n->pagina = 5; 
n->prox = NULL; 
n->prev = NULL;
l.ini = n; 
l.fim = n;
listade_retirar(&l, n);
free(n);
*/
void listade_retirar(LRU *l, No *n) {
    if (n->prev != NULL)
        n->prev->prox = n->prox;
    else
        l->ini = n->prox;

    if (n->prox != NULL)
        n->prox->prev = n->prev;
    else
        l->fim = n->prev;

    n->prox = NULL;
    n->prev = NULL;
}

/* Insere um nó *n* no inicio da lista duplamente encadeada via ponteiro *l* estrutura LRU,
marcando como o elemento mais recentemente usado.

Exemplos:
LRU l;
l.cap_hash = 10;
l.ini = NULL;
l.fim = NULL;
l.tabela = calloc(l.cap_hash, sizeof(No *));
lru_inserir(&l, 3, 1);
No *n = lru_buscar(&l, 3);
lru_atualizar(&l, n);
lru_destruir(&l);
*/
void listade_inserir_inicio(LRU *l, No *n) {
    n->prev = NULL;
    n->prox = l->ini;

    if (l->ini != NULL)
        l->ini->prev = n;
    else
        l->fim = n;

    l->ini = n;
}

/* Atualiza a posição de um nó *n* no inicio da lista via ponteiro *l* estrutura LRU,
movendo para o início para indicar que acabou de ser acessado hit.

Exemplos:
LRU l;
l.ini = NULL;
l.fim = NULL;
No *n = malloc(sizeof(No));
n->pagina = 5; 
n->prox = NULL; 
n->prev = NULL;
listade_inserir_inicio(&l, n);
free(n);
*/

void lru_atualizar(LRU *l, No *n) {
    if (l->ini == n)
        return;

    listade_retirar(l, n);
    listade_inserir_inicio(l, n);
}

/* Aloca e insere um novo nó via ponteiro *l* na estrutura LRU 
(tanto na lista encadeada quanto na tabela hash), dado a ppagina *pag* e indice no 
vetor de quadros da memória*idx*.
Retorna 0 em caso de sucesso. 

Exemplos:
LRU l;
l.cap_hash = 5;
l.ini = NULL;
l.fim = NULL;
l.tabela = calloc(l.cap_hash, sizeof(No *));
int pagina = 4;
int indice_quadro = 0;
lru_inserir(&l, pagina, indice_quadro);
lru_destruir(&l);
*/
int lru_inserir(LRU *l, int pag, int idx) {
    No *n = malloc(sizeof(No));

    n->pagina = pag;
    n->indice = idx;
    n->prox = NULL;
    n->prev = NULL;
    n->prox_hash = NULL;

    listade_inserir_inicio(l, n);
    hash_inserir(l, n);

    return 0;
}
/*Seleciona a página vítima para remoção, que se encontra no final da lista duplamente encadeada,
dado via ponteiro *l* na estrutura LRU, retornado um ponteiro para um no

Exemplos:
LRU l;
l.cap_hash = 5;
l.ini = NULL;
l.fim = NULL;
l.tabela = calloc(l.cap_hash, sizeof(No *));
lru_inserir(&l, 1, 0);
lru_inserir(&l, 2, 1);
No *vitima = lru_escolher_vitima(&l);
vitima->pagina será 10
lru_destruir(&l);
*/
No *lru_escolher_vitima(LRU *l) {
    return l->fim;
}

/* Substitui uma página antiga por uma nova *pag* e o passo lógico atual *passo* na estrutura LRU
 via ponteiro *l*, utilizando o ponteiro para o no *n* e atualiza a memória via ponteiro *m* 

Exemplos:
Memoria *m = criar_memoria(2);
LRU l;
l.cap_hash = 2;
l.ini = NULL;
l.fim = NULL;
l.tabela = calloc(l.cap_hash, sizeof(No *));
lru_inserir(&l, 8, 0);
No *vitima = lru_escolher_vitima(&l);

int nova_pagina = 9;
int passo_atual = 3;
lru_substituir(&l, m, vitima, nova_pagina, passo_atual);

destruir_memoria(m);
lru_destruir(&l);
*/
void lru_substituir(LRU *l, Memoria *m, No *n, int pag, int passo) {
    hash_remover(l, n);
    listade_retirar(l, n);

    n->pagina = pag;

    m->q[n->indice].pagina = pag;
    m->q[n->indice].ultimo_uso = passo;

    listade_inserir_inicio(l, n);
    hash_inserir(l, n);
}

/* Destroi os nós da lista e a tabela hash de uma estrutura LRU liberando espaço,
dado um ponteiro *l* passado. 

Exemplos:
LRU l;
l.cap_hash = 2;
l.ini = NULL;
l.fim = NULL;
l.tabela = calloc(l.cap_hash, sizeof(No *));
lru_inserir(&l, 5, 0);
lru_destruir(&l);
*/
void lru_destruir(LRU *l) {
    No *n = l->ini;

    while (n != NULL) {
        No *prox = n->prox;
        free(n);
        n = prox;
    }

    free(l->tabela);
}
