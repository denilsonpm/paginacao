#include <stdlib.h>
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

/* Executa o fifo, dado um ponteiro para sequencia *seq*, o tamanho da sequencia *n*,
número de quadros *n_quadros* e um ponteiro para um Traco *t*. Retorna 0 se sucesso.

Exemplos:
int sequencia[] = {1, 2, 3, 3, 1, 2, 5, 1, 2, 3, 4, 5};
int tamanho_seq = 12;
int n_quadros = 3;
Traco t;
int total_refs_previstas = 4;
iniciar_traco(&t,total_refs_previstas,n_quadros,1);
executar_fifo(sequencia, tamanho_seq, n_quadros, &t);
destruir_traco(&t);
*/
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

/* Cria uma fila dado um número de quadros *n_quadros*,
retornando um struct da fila preenchdia. 

Exemplos:
Fila f = criar_fila(0);
f, tal f.cap == 0, f.qtd == 0, f.ini == 0, f.fim == 0, f.pont == 0
Fila f = criar_fila(3);
f, tal f.cap == 3, f.qtd == 0, f.ini == 0, f.fim == 0, f.pont == 0
*/
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

/* Coloca um elemento *x* no final de uma fila *f*. 

Exemplos:
Fila f = criar_fila(0);
fila_inserir(&f, 10);
fila_inserir(&f, 3);
printf("%d", f.qtd);
2
*/
void fila_inserir(Fila *f, int x) {
  f->v[f->fim] = x;
  f->fim = (f->fim + 1) % f->cap;
  f->qtd++;
}

/* Remove o elemento do inicio de uma fila *f* e o retorna. 

Exemplos:
Fila f = criar_fila(0);
fila_inserir(&f, 10);
fila_inserir(&f, 3);
fila_remover(&f);
fila_remover(&f);
printf("%d", f.qtd);
0
*/
int fila_remover(Fila *f) {
  int x = f->v[f->ini];
  f->ini = (f->ini + 1) % f->cap;
  f->qtd--;
  
  return x;
}
/* Destroi uma fila *f*. 

Exemplos:
Fila f = criar_fila(0);
fila_inserir(&f, 10);
fila_remover(&f);
destruir_fila(&f);
*/
void destruir_fila(Fila *f){
  free(f->v);
  f->v = NULL;
}
