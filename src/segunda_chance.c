#include <stdio.h>
#include <stdlib.h>
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

int executar_segunda_chance(const int *seq, int n, int n_quadros, Traco *t) {
  
  Memoria *m = criar_memoria(n_quadros);
  Fila f = criar_fila(n_quadros);

  if (t->verbose == 1) {
    printf("\nAlgoritmo: SEGUNDA CHANCE\n");
    printf("Quadros: %d\n\n", n_quadros);
    printf("Ref\tQuadros\t\tStatus\n");
  }
  
  for (int i = 0; i < n; i++) {
    
    int pag = seq[i];
    int idx = buscar(m, pag);
    int falta = (idx == -1);
    int vitima = VAZIO;
    
    if (idx >= 0) {
      m->q[idx].bit_ref = 1;

    } else if (m->ocupados < n_quadros) {
      
      m->q[m->ocupados].bit_ref = 1;
      m->q[m->ocupados].pagina = pag;
      m->ocupados++;
      
      fila_inserir(&f, pag);
      
    } else {
      vitima = fila_remover_SC(&f, m);

      int indice = buscar(m, vitima);
     
      m->q[indice].pagina = pag;
      m->q[indice].bit_ref = 1;      

      fila_inserir(&f, pag);
    }
    registrar_passo(t, pag, m, falta, vitima);

    if (t->verbose == 1) {
      imprimir_verboseSC(m, pag, falta, vitima);
    }
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


void imprimir_verboseSC(const Memoria *m, int referencia, int falta, int vitima) {

    printf("%d\t[", referencia);

    for (int i = 0; i < m->n_quadros; i++) {
        if (i > 0) printf(",");

        if (m->q[i].pagina == VAZIO)
            printf("-");
        else
            printf("%d:%d", m->q[i].pagina, m->q[i].bit_ref);
    }

    printf("]\t\t");

    if (falta) {
        printf("FALTA");

        if (vitima != VAZIO)
            printf(" (sai %d)", vitima);
    } else {
        printf("ACERTO");
    }

    printf("\n");
}
