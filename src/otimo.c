#include <stdio.h>
#include <stdlib.h>
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

int executar_otimo(const int *seq, int n, int n_quadros, Traco *t) {
  Memoria *m = memoria_criar(n_quadros);
  for (int i = 0; i < n; i++) {
    int pag = seq[i];
    int idx = memoria_buscar(m, pag);
    if (idx >= 0) {
      /* ACERTO: FIFO não faz nada; LRU/SC/LFU atualizam algo */
    } else if (m->ocupados < n_quadros) {
      /* FALTA com quadro livre: carregar no primeiro vazio */
    } else {
      /* FALTA com memória cheia: escolher vítima (varia!) */
    }
    /* registrar_passo(...) */
  }
  ...
}
