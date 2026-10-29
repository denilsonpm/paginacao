#include <stdlib.h>
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

/* Executa o Ótimo, dado um ponteiro para sequencia *seq*, o tamanho da sequencia *n*,
número de quadros *n_quadros* e um ponteiro para um Traco *t*. Retorna 0 se sucesso.

Exemplos:
int sequencia[] = {1, 2, 3, 3, 1, 2, 5, 1, 2, 3, 4, 5};
int tamanho_seq = 12;
int n_quadros = 3;
Traco t;
int total_refs_previstas = 4;
iniciar_traco(&t,total_refs_previstas,n_quadros,1);
executar_otimo(sequencia, tamanho_seq, n_quadros, &t);
destruir_traco(&t);
*/
int executar_otimo(const int *seq, int n, int n_quadros, Traco *t) {
    Memoria *m = criar_memoria(n_quadros);
    int *prox = malloc((size_t)n * sizeof(int));

    construir_proximas(seq, n, prox);

    for (int i = 0; i < n; i++) {
        int pag = seq[i];
        int idx = buscar(m, pag);
        int falta = (idx == -1);
        int vitima = VAZIO;

        if (idx >= 0) {

            m->q[idx].ultimo_uso = i;

        } else if (m->ocupados < n_quadros) {

            idx = m->ocupados;

            m->q[idx].pagina = pag;
            m->q[idx].ultimo_uso = i;
            m->ocupados++;

        } else {

            int idv = 0;
            int maior = -1;

            for (int j = 0; j < m->n_quadros; j++) {
                idx = m->q[j].ultimo_uso;

                if (prox[idx] > maior) {
                    maior = prox[idx];
                    idv = j;
                }
            }

            vitima = m->q[idv].pagina;
            m->q[idv].pagina = pag;
            m->q[idv].ultimo_uso = i;
        }

        registrar_passo(t, pag, m, falta, vitima);
    }

    free(prox);
    destruir_memoria(m);

    return 0;
}

/* Constrói o vetor auxiliar 'prox', varrendo a sequência de trás para frente 
usando uma tabela hash para determinar quando cada página será referenciada novamente,
dado um ponteiro para sequencia *seq*,o tamanho da sequencia *n* e um ponteiro para um Traco *t*.
  Retorna 0 se sucesso.

Exemplos:
int sequencia[] = {1, 2, 3, 3, 1, 2, 5, 1, 2, 3, 4, 5};
int tamanho_seq = 12;
int *prox = malloc(n * sizeof(int));
construir_proximas(seq, n, prox);
*/
int construir_proximas(const int *seq, int n, int *prox) {
    No **tabela = calloc((size_t)n, sizeof(No *));

    for (int i = n - 1; i >= 0; i--) {
        int pag = seq[i];
        int indice = hash(pag, n);

        No *atual = tabela[indice];

        while (atual != NULL && atual->pagina != pag) {
            atual = atual->prox_hash;
        }

        if (atual != NULL) {
            prox[i] = atual->indice;
            atual->indice = i;

        } else {
            prox[i] = NUNCA;

            No *novo = malloc(sizeof(No));

            novo->pagina = pag;
            novo->indice = i;
            novo->prox = NULL;
            novo->prev = NULL;
            novo->prox_hash = tabela[indice];

            tabela[indice] = novo;
        }
    }

    for (int j = 0; j < n; j++) {
        No *atual = tabela[j];

        while (atual != NULL) {
            No *proximo = atual->prox_hash;
            free(atual);
            atual = proximo;
        }
    }

    free(tabela);

    return 0;
}
