#include <stdlib.h>
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

int executar_lru(const int *seq, int n, int n_quadros, Traco *t) {
    Memoria *m = criar_memoria(n_quadros);

    LRU l = {0};

    l.cap_hash = n_quadros * 2 + 1;
    l.tabela = calloc(l.cap_hash, sizeof(No *));


    for (int i = 0; i < n; i++) {
        int pag = seq[i];
        No *no = lru_buscar(&l, pag);

        int falta = (no == NULL);
        int vitima = VAZIO;

        if (no != NULL) {
            lru_atualizar(&l, no);
            m->q[no->indice].ultimo_uso = i;

        } else if (m->ocupados < n_quadros) {
            int idx = m->ocupados;

            lru_inserir(&l, pag, idx);

            m->q[idx].pagina = pag;
            m->q[idx].ultimo_uso = i;
            m->ocupados++;

        } else {
            No *removido = lru_escolher_vitima(&l);

            vitima = removido->pagina;

            lru_substituir(&l, m, removido, pag, i);
        }

        registrar_passo(t, pag, m, falta, vitima);
    }

    lru_destruir(&l);
    destruir_memoria(m);

    return 0;
}

int hash(int pagina, int cap) {
    return (int)((unsigned int)pagina % (unsigned int)cap);
}

No *lru_buscar(LRU *l, int pagina) {
    int h = hash(pagina, l->cap_hash);
    No *n = l->tabela[h];

    while (n != NULL) {
        if (n->pagina == pagina)
            return n;

        n = n->prox_hash;
    }

    return NULL;
}

void hash_inserir(LRU *l, No *n) {
    int h = hash(n->pagina, l->cap_hash);

    n->prox_hash = l->tabela[h];
    l->tabela[h] = n;
}

void hash_remover(LRU *l, No *n) {
    int h = hash(n->pagina, l->cap_hash);
    No **atual = &l->tabela[h];

    while (*atual != NULL) {
        if (*atual == n) {
            *atual = n->prox_hash;
            n->prox_hash = NULL;
            return;
        }

        atual = &(*atual)->prox_hash;
    }
}


void listade_retirar(LRU *l, No *n) {
    if (n->prev != NULL)
        n->prev->prox = n->prox;
    else
        l->ini = n->prox;

    if (n->prox != NULL)
        n->prox->prev = n->prev;
    else
        l->fim = n->prev;

    n->prox = NULL;
    n->prev = NULL;
}

void listade_inserir_inicio(LRU *l, No *n) {
    n->prev = NULL;
    n->prox = l->ini;

    if (l->ini != NULL)
        l->ini->prev = n;
    else
        l->fim = n;

    l->ini = n;
}


void lru_atualizar(LRU *l, No *n) {
    if (l->ini == n)
        return;

    listade_retirar(l, n);
    listade_inserir_inicio(l, n);
}

int lru_inserir(LRU *l, int pag, int idx) {
    No *n = malloc(sizeof(No));

    n->pagina = pag;
    n->indice = idx;
    n->prox = NULL;
    n->prev = NULL;
    n->prox_hash = NULL;

    listade_inserir_inicio(l, n);
    hash_inserir(l, n);

    return 0;
}

No *lru_escolher_vitima(LRU *l) {
    return l->fim;
}

void lru_substituir(LRU *l, Memoria *m, No *n, int pag, int passo) {
    hash_remover(l, n);
    listade_retirar(l, n);

    n->pagina = pag;

    m->q[n->indice].pagina = pag;
    m->q[n->indice].ultimo_uso = passo;

    listade_inserir_inicio(l, n);
    hash_inserir(l, n);
}

void lru_destruir(LRU *l) {
    No *n = l->ini;

    while (n != NULL) {
        No *prox = n->prox;
        free(n);
        n = prox;
    }

    free(l->tabela);
}
