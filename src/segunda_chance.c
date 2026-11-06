#include <stdio.h>
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

extern FILE *historico;

/* Executa o Segunda Chance, dado um ponteiro para sequencia *seq*, o tamanho da sequencia *n*,
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

int executar_segunda_chance(const int *seq, int n, int n_quadros, Traco *t) {
  
  Memoria *m = criar_memoria(n_quadros);
  Fila f = criar_fila(n_quadros);

  if (t->verbose == 1) {
    printf("\nAlgoritmo: SEGUNDA CHANCE\n");
    printf("Quadros: %d\n\n", n_quadros);
    printf("Ref\tQuadros\t\tStatus\n");

    if (historico != NULL) {
        fprintf(historico, "\nAlgoritmo: SEGUNDA CHANCE\n");
        fprintf(historico, "Quadros: %d\n\n", n_quadros);
        fprintf(historico, "Ref\tQuadros\t\tStatus\n");
    }
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

/* Remove e retorna a pagina com base na lógica do segunda chance, dado um ponteiro para Fila *f* 
e um ponteiro para Memória *m*. 

Exemplos:
Memoria *m = criar_memoria(2);
Fila f = criar_fila(2);
m->q[0].pagina = 2;
m->q[0].bit_ref = 0;
m->q[1].pagina = 3;
m->q[1].bit_ref = 1;
f.pont = 0;
int vitima = fila_remover_SC(&f, m);
2
*/
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

/* Imprime a verbose do segunda chance, dado um ponteiro para Memoria *m*,a pagina *referencia*,
a *falta* que só pode ser 0 ou 1, e a pagina removida *vitima*. 

Exemplos:
Memoria *m = criar_memoria(2);
m->q[0].pagina = 2;
m->q[0].bit_ref = 1;
m->q[1].pagina = VAZIO;
imprimir_verboseSC(m, 10, 0, VAZIO);
destruir_memoria(m);
*/


void imprimir_verboseSC(const Memoria *m, int referencia, int falta, int vitima) {

    printf("%d\t[", referencia);

    if (historico != NULL)
        fprintf(historico, "%d\t[", referencia);

    for (int i = 0; i < m->n_quadros; i++) {
        if (i > 0) {
            printf(",");
            if (historico != NULL)
                fprintf(historico, ",");
        }

        if (m->q[i].pagina == VAZIO) {
            printf("-");
            if (historico != NULL)
                fprintf(historico, "-");
        } else {
            printf("%d:%d", m->q[i].pagina, m->q[i].bit_ref);

            if (historico != NULL)
                fprintf(historico, "%d:%d",
                        m->q[i].pagina, m->q[i].bit_ref);
        }
    }

    printf("]\t\t");

    if (historico != NULL)
        fprintf(historico, "]\t\t");

    if (falta) {
        printf("FALTA");

        if (historico != NULL)
            fprintf(historico, "FALTA");

        if (vitima != VAZIO) {
            printf(" (sai %d)", vitima);

            if (historico != NULL)
                fprintf(historico, " (sai %d)", vitima);
        }
    } else {
        printf("ACERTO");

        if (historico != NULL)
            fprintf(historico, "ACERTO");
    }

    printf("\n");

    if (historico != NULL) {
        fprintf(historico, "\n");
        fflush(historico);
    }
}
