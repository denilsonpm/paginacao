#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include <limits.h>
#include "traco.h"

#define NUNCA INT_MAX

//Representa uma fila
typedef struct {
    int *v; // Vetor
    int ini; // indice do inicio
    int fim; // indice do fim
    int qtd; // quantidade armazenada
    int cap; // capadicade da fila
    int pont; // ponteiro
} Fila;

//Representa um NO
typedef struct No {
    int pagina; //numero da pagina
    int indice; //indice da ultima posicao da pagina
    struct No *prox; //Ponteiro para proximo
    struct No *prev; //Ponteiro para prévio
    struct No *prox_hash; // Próximo NO do bucket da tabela hash
} No;
//Representa uma combinação de uma lista duplamente encadeada com uma tabela hash.
typedef struct {
    No *ini; // indice do inicio
    No *fim; // indice do fim
    No **tabela; //tabela hash
    int cap_hash; // capacidade da tabela hash
} LRU;

Fila criar_fila(int n_quadros);
void fila_inserir(Fila *f, int x);
int fila_remover(Fila *f);
void destruir_fila(Fila *f);
int fila_remover_SC(Fila *f, Memoria *m);
int fila_remover_LFU(Fila *f, Memoria *m);

int hash(int pagina, int cap);
No *lru_buscar(LRU *l, int pagina);
void hash_inserir(LRU *l, No *n);
void hash_remover(LRU *l, No *n);

void listade_retirar(LRU *l, No *n);
void listade_inserir_inicio(LRU *l, No *n);

void lru_atualizar(LRU *l, No *n);
int lru_inserir(LRU *l, int pag, int idx);
No *lru_escolher_vitima(LRU *l);
void lru_substituir(LRU *l, Memoria *m, No *n, int pag, int passo);
void lru_destruir(LRU *l);

int proxima_ocorrencia(const int *seq, int n, int i, int pag);
int construir_proximas(const int *seq, int n, int *prox);

void imprimir_verboseSC(const Memoria *m, int referencia, int falta, int vitima);
void imprimir_verboseLFU(const Memoria *m, int referencia, int falta, int vitima);

int executar_fifo(const int *seq, int n, int n_quadros, Traco *t);
int executar_lru(const int *seq, int n, int n_quadros, Traco *t);
int executar_otimo(const int *seq, int n, int n_quadros, Traco *t);
int executar_segunda_chance(const int *seq, int n, int n_quadros, Traco *t);
int executar_lfu(const int *seq, int n, int n_quadros, Traco *t);

#endif
