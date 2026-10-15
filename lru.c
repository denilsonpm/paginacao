#include <stdio.h>
#include <stdlib.h>
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

int executar_lru(const int *seq, int n, int n_quadros, Traco *t) {
  
  Memoria *m = criar_memoria(n_quadros);
  Fila f = criar_fila(n_quadros);
  int tempo = 0;
  
  for (int i = 0; i < n; i++) {
    
    int pag = seq[i];
    int idx = buscar(m, pag);
    int falta = (idx == -1);
    int vitima = VAZIO;
    tempo++;
    
    if (idx >= 0) {

      m->q[idx].ultimo_uso = tempo;

    } else {
      int vit = escolher_vitima_lru(m);
      /* TODO: substituir, gravar ultimo_uso = tempo, contar falta */
    }
    registrar_passo(t, pag, m, falta, vitima);
  }
}

No criar_no(int n){
  No no;

  no.valor = n;
  no.prox = 0;
  no.prev = 0;
}

void lru_inserir(Fila f, No n){
  n.prox = f.ini;
  if (f.ini != NULL){
    f.ini.prev = n;
  }
  f.ini = n;
  n.prev = NULL;
}

static int escolher_vitima_lru(const Memoria *m) {

  int menor = 0;
  /* TODO: varrer os quadros e guardar o indice do menor ultimo_uso */
  return menor;
}
