#include <stdio.h>
#include <stdlib.h>
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

int executar_segunda_chance(const int *seq, int n, int n_quadros, Traco *t) {
  
  Memoria *m = criar_memoria(n_quadros);
  Fila f = criar_fila(n_quadros);
  
  for (int i = 0; i < n; i++) {
    
    int pag = seq[i];
    int idx = buscar(m, pag);
    int falta = (idx == -1);
    int vitima = VAZIO;
    
    if (idx >= 0) {
      m->q[idx].bit_ref = 1;

    } else if (m->ocupados < n_quadros) {
      
      m->q[m->ocupados].pagina = pag;
      m->ocupados++;
      
      fila_inserir(&f, pag);
      
    } else {
      vitima = fila_remover_SC(&f, m);

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
int fila_remover_SC(Fila *f, Memoria *m) {
    while (m->q[f->pont].bit_ref == 1) {
        m->q[f->pont].bit_ref = 0;
        f->pont = (f->pont + 1) % m->n_quadros;
    }

    f->fim = f->pont;
    f->ini = f->pont;
    
    f->pont = (f->pont + 1) % m->n_quadros;

    return fila_remover(f);
}
