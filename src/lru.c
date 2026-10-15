#include <stdlib.h>      
#include "quadros.h"     
#include "traco.h"       
#include "algoritmos.h"  

int executar_lru(const int *seq, int n, int n_quadros, Traco *t) {
    Memoria *m = criar_memoria(n_quadros);
    int faltas = 0;

    if (m == NULL)
        return -1;

    for (int i = 0; i < n; i++) {
        int pag = seq[i];
        int idx = buscar(m, pag);
        int falta = (idx == -1);
        int vitima = VAZIO;
      
        if (idx >= 0) {
            m->q[idx].ultimo_uso = i;

        } else if (m->ocupados < n_quadros) {
            lru_inserir(m, pag, i,m->ocupados);
            faltas++;
            m->ocupados++;

        } else {
            int idv = lru_escolher_vitima(m);

            vitima = m->q[idv].pagina;

            lru_inserir(m, pag, i, idv);

            faltas++;
        }

        registrar_passo(t, pag, m, falta, vitima);
    }
    destruir_memoria(m);

    return 0;
}

void lru_inserir(Memoria *m, int pag, int passo, int idx) {
    m->q[idx].pagina = pag;
    m->q[idx].ultimo_uso = passo;
}

int lru_escolher_vitima(Memoria *m){
  int menor = 0;

   for (int i = 1; i < m->ocupados; i++) {
        if (m->q[i].ultimo_uso < m->q[menor].ultimo_uso) {
            menor = i;
        }
    }

  
  return menor;
}
