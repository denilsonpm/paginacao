#include <stdio.h>
#include <stdlib.h>
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

int executar_otimo(const int *seq, int n, int n_quadros, Traco *t) {
  
  Memoria *m = criar_memoria(n_quadros);
  
  for (int i = 0; i < n; i++) {
    
    int pag = seq[i];
    int idx = buscar(m, pag);
    int falta = (idx == -1);
    int vitima = VAZIO;
    
    if (idx >= 0) {
      /* ACERTO: otimo não faz nada*/
      
    } else if (m->ocupados < n_quadros) {

      int pos = m->ocupados;

      m->q[pos].pagina = seq[i];
      m->ocupados++;
      
    } else {
      int maior_distancia = -1;
      int idv = -1;
      
      for (int j = 0; j < m->n_quadros; j++) {
        int prox = proxima_ocorrencia(seq, n, i, m->q[j].pagina);
      
        if (prox == NUNCA) {
             idv = j;
             break;
        }
      
        if (prox > maior_distancia) {
            maior_distancia = prox;
            idv = j;
        }  
      }
        int vitima = m->q[idv].pagina;
  
        m->q[idv].pagina = seq[i];      
    }
    
    registrar_passo(t, pag, m, falta, vitima);
  }

  destruir_memoria(m);

  return 0;
}

/* Distancia da próxima ocorrência de ’pag’ em seq[i+1 .. n-1], ou NUNCA. */
int proxima_ocorrencia(const int *seq, int n, int i, int pag){
  for(int j = 1; j < (n - i); j++){
    if(seq[i+j] == pag){ return j;}
  }
  return NUNCA;
}
