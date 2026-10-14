#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "traco.h"

typedef struct {int *v; int ini, fim, qtd, cap;} Fila;

Fila criar_fila(int n_quadros);

void fila_inserir(Fila *f, int x);

int fila_remover(Fila *f);

void destruir_fila(Fila *f);

int executar_fifo(const int *seq, int n, int n_quadros, Traco *t);

#endif
