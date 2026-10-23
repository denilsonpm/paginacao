#include <stdio.h>
#include <stdlib.h>
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

int executar_fifo(const int *seq, int n, int n_quadros, Traco *t) {
  
  Memoria *m = criar_memoria(n_quadros);
  Fila f = criar_fila(n_quadros);
  
  for (int i = 0; i < n; i++) {
    
    int pag = seq[i];
    int idx = buscar(m, pag);
    int falta = (idx == -1);
    int vitima = VAZIO;
    
    if (idx >= 0) {
      /* ACERTO: FIFO não faz nada*/
      
    } else if (m->ocupados < n_quadros) {
      
      m->q[m->ocupados].pagina = pag;
      m->ocupados++;
      
      fila_inserir(&f, pag);
      
    } else {

      vitima = fila_remover(&f);

      int indice = buscar(m, vitima);

      m->q[indice].pagina = pag;

      fila_inserir(&f, pag);
      
    }
    registrar_passo(t, pag, m, falta, vitima);
  }
  
  destruir_fila(&f);
  destruir_memoria(m);

  return 0;
}

Fila criar_fila(int n_quadros){
  Fila f;

  f.v = malloc(n_quadros * sizeof(int));

  f.ini = 0;
  f.fim = 0;
  f.qtd = 0;
  f.cap = n_quadros;
  f.pont = 0;

  return f;
} 

void fila_inserir(Fila *f, int x) {
  f->v[f->fim] = x;
  f->fim = (f->fim + 1) % f->cap;
  f->qtd++;
}

int fila_remover(Fila *f) {
  int x = f->v[f->ini];
  f->ini = (f->ini + 1) % f->cap;
  f->qtd--;
  
  return x;
}

void destruir_fila(Fila *f){
  free(f->v);
  f->v = NULL;
}
