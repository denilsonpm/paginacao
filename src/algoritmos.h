#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include <limits.h>
#include "traco.h"

#define NUNCA INT_MAX

typedef struct {int *v; int ini, fim, qtd, cap, pont;} Fila;

typedef struct No {int pagina; int indice; struct No *prox; struct No *prev;struct No *prox_hash;} No;

typedef struct {No *ini; No *fim; No **tabela; int cap_hash;} LRU;

Fila criar_fila(int n_quadros);
void fila_inserir(Fila *f, int x);
int fila_remover(Fila *f);
void destruir_fila(Fila *f);
int fila_remover_SC(Fila *f, Memoria *m);

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

int executar_fifo(const int *seq, int n, int n_quadros, Traco *t);
int executar_lru(const int *seq, int n, int n_quadros, Traco *t);
int executar_otimo(const int *seq, int n, int n_quadros, Traco *t);
int executar_segunda_chance(const int *seq, int n, int n_quadros, Traco *t);
  
#endif
