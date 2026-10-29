#include <stdio.h>
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

/* Executa o LFU, dado um ponteiro para sequencia *seq*, o tamanho da sequencia *n*,
número de quadros *n_quadros* e um ponteiro para um Traco *t*. Retorna 0 se sucesso.

Exemplos:
int sequencia[] = {1, 2, 3, 3, 1, 2, 5, 1, 2, 3, 4, 5};
int tamanho_seq = 12;
int n_quadros = 3;
Traco t;
int total_refs_previstas = 4;
iniciar_traco(&t,total_refs_previstas,n_quadros,1);
executar_segunda_chance(sequencia, tamanho_seq, n_quadros, &t);
destruir_traco(&t);
*/

int executar_lfu(const int *seq, int n, int n_quadros, Traco *t) {
  
  Memoria *m = criar_memoria(n_quadros);
  Fila f = criar_fila(n_quadros);

  if (t->verbose == 1) {
    printf("\nAlgoritmo: LFU\n");
    printf("Quadros: %d\n\n", n_quadros);
    printf("Ref\tQuadros\t\tStatus\n");
  }
  
  for (int i = 0; i < n; i++) {
    
    int pag = seq[i];
    int idx = buscar(m, pag);
    int falta = (idx == -1);
    int vitima = VAZIO;
    
    if (idx >= 0) {
      m->q[idx].contador += 1;

    } else if (m->ocupados < n_quadros) {
      
      m->q[m->ocupados].contador = 0;
      m->q[m->ocupados].pagina = pag;
      m->ocupados++;
      
      fila_inserir(&f, pag);
      
    } else {
      vitima = fila_remover_LFU(&f, m);

      int indice = buscar(m, vitima);
     
      m->q[indice].pagina = pag;
      m->q[indice].contador = 0;      

      fila_inserir(&f, pag);
    }
    registrar_passo(t, pag, m, falta, vitima);

    if (t->verbose == 1) {
      imprimir_verboseLFU(m, pag, falta, vitima);
    }
  }
  
  destruir_fila(&f);
  destruir_memoria(m);

  return 0;
}

int fila_remover_LFU(Fila *f, Memoria *m) {
    int menor = INT_MAX;
    int pos_vitima = f->ini;

    for (int i = 0; i < f->qtd; i++) {
        int pos = (f->ini + i) % f->cap;
        int idx = buscar(m, f->v[pos]);

        if (m->q[idx].contador < menor) {
            menor = m->q[idx].contador;
            pos_vitima = pos;
        }
    }

    int vitima = f->v[pos_vitima];
    int ultimo = (f->fim - 1 + f->cap) % f->cap;

    while (pos_vitima != ultimo) {
        int prox = (pos_vitima + 1) % f->cap;
        f->v[pos_vitima] = f->v[prox];
        pos_vitima = prox;
    }

    f->fim = ultimo;
    f->qtd--;

    return vitima;
}

/* Imprime a verbose do LFU, dado um ponteiro para Memoria *m*,a pagina *referencia*,
a *falta* que só pode ser 0 ou 1, e a pagina removida *vitima*. 

Exemplos:
Memoria *m = criar_memoria(3);
m->q[0].pagina = 2;
m->q[0].contador = 4;
m->q[1].pagina = 3;
m->q[1].contador = 1;
m->ocupados = 2;
imprimir_verboseLFU(m, 3, 0, VAZIO);
m->q[2].pagina = 4;
m->q[2].contador = 1;
m->ocupados = 3;
imprimir_verboseLFU(m, 4, 1, 2);
destruir_memoria(m);
*/
void imprimir_verboseLFU(const Memoria *m, int referencia, int falta, int vitima) {

    printf("%d\t[", referencia);

    for (int i = 0; i < m->n_quadros; i++) {
        if (i > 0) printf(",");

        if (m->q[i].pagina == VAZIO)
            printf("-");
        else
            printf("%d:%d", m->q[i].pagina, m->q[i].contador);
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
