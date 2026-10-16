#include <stdio.h>
#include <stdlib.h>
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

int executar_otimo(const int *seq, int n, int n_quadros, Traco *t) {
    if (n < 0 || n_quadros <= 0 || (n > 0 && seq == NULL))
        return -1;

    if (n == 0)
        return 0;

    Memoria *m = criar_memoria(n_quadros);
    int *prox = malloc((size_t)n * sizeof(int));

    if (m == NULL || prox == NULL) {
        free(prox);

        if (m != NULL)
            destruir_memoria(m);

        return -1;
    }

    if (construir_proximas(seq, n, prox) != 0) {
        free(prox);
        destruir_memoria(m);
        return -1;
    }

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

int construir_proximas(const int *seq, int n, int *prox) {
    if (n < 0 || (n > 0 && (seq == NULL || prox == NULL)))
        return -1;

    if (n == 0)
        return 0;

    No **tabela = calloc((size_t)n, sizeof(No *));

    if (tabela == NULL)
        return -1;

    int resultado = 0;

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

            if (novo == NULL) {
                resultado = -1;
                break;
            }

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

    return resultado;
}
