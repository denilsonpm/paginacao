#include <stdio.h>
#include <stdlib.h>
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

int executar_lru(const int *seq, int n, int n_quadros, Traco *t) {
  
  Memoria *m = memoria_criar(n_quadros);
  
  for (int i = 0; i < n; i++) {
    
    int pag = seq[i];
    int idx = memoria_buscar(m, pag);
    
    if (idx >= 0) {
      /* ACERTO: FIFO não faz nada*/
    } else if (m->ocupados < n_quadros) {
      /* FALTA com quadro livre: carregar no primeiro vazio */
    } else {
      /* FALTA com memória cheia: escolher vítima (varia!) */
    }
    /* registrar_passo(...) */
  }
  ...
}

typedef struct { int *v; int ini, fim, qtd, cap; } Fila;

Fila criar_fila(int n_quadros){
  Fila f;

  f.v = malloc(n_quadros * sizeof(int));
} 

void fila_inserir(Fila *f, int x) {
  f->v[f->fim] = x;
  f->fim = (f->fim + 1) % f->cap;
  f->qtd++;
}

int fila_remover(Fila *f) {
  if (f == NULL || f->qtd == 0) {
    return -1;
  }
  
  f->v[f->ini] = x;
  f->ini = (f->ini + 1) % f->cap;
  f->qtd--;
  
  return x;
}
