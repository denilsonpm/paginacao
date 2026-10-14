#include <stdlib.h>
#include <stdio.h>
#include "traco.h"

/*  Como chamar na main
    int tamanho;
    int *seq = carregar_sequencia("sequencia.txt", &tamanho);

    if (seq == NULL) {
        return 1;
    }

    Memoria *m = memoria_criar(3);

    if (m == NULL) {
        free(seq);
        return 1;
    }

    Traco t = {0, 0, 1}; // com verbose

    for (int i = 0; i < tamanho; i++) {
        int pagina = seq[i];

        int indice = memoria_buscar(m, pagina);

        int falta = (indice == -1);
        int vitima = VAZIO;

        if (falta == 1) {
            if (m->ocupados < m->n_quadros) {
                m->q[m->ocupados].pagina = pagina;
                m->ocupados++;
            } else {
                // Aqui entraria o algoritmo FIFO, LRU etc.
            }
        }

        registrar_passo(&t, pagina, m, falta, vitima);
    }

    imprimir_resumo(&t, "Algoritmo", m->n_quadros);

    memoria_destruir(m);
    free(seq);*/
void registrar_passo(Traco *t, int ref, const Memoria *m,
                    int falta, int vitima) {
    if (t == NULL) {
        return;
    }

    t->total_refs++;

    if (falta == 1) {
        t->total_faltas++;
    }

    if (!t->verbose) {
        return;
    }

    // Imprime o cabeçalho apenas na primeira referência
    if (t->total_refs == 1) {
        printf("%-5s %-12s %s\n", "Ref", "Quadros", "Status");
    }

    printf("%-5d [", ref);
    
    if (m != NULL) {
        for (int i = 0; i < m->n_quadros; i++) {
            if (i > 0) {
                printf(",");
            }

            if (m->q[i].pagina == VAZIO) {
                printf("-");
            } else {
                printf("%d", m->q[i].pagina);
            }
        }
    }

    printf("]");

    // Alinhamento da coluna Status
    // Considerando uma largura mínima de 12 caracteres
    int tamanho = 2;

    if (m != NULL) {
        tamanho += m->n_quadros - 1;

        for (int i = 0; i < m->n_quadros; i++) {
            if (m->q[i].pagina == VAZIO) {
                tamanho++;
            } else {
                tamanho += snprintf(NULL, 0, "%d", m->q[i].pagina);
            }
        }
    }

    for (int i = tamanho; i < 12; i++) {
        printf(" ");
    }

    // Coluna Status
    if (falta) {
        printf("FAULT");

        if (vitima != VAZIO) {
            printf(" (sai %d)", vitima);
        }
    } else {
        printf("HIT");
    }

    printf("\n");
}
void imprimir_resumo(const Traco *t, const char *algoritmo,
                    int n_quadros) {
    if (t == NULL) {
        return;
    }

    printf("\nAlgoritmo: %s\n", algoritmo);
    printf("Quadros: %d\n", n_quadros);
    printf("Total de referencias: %d\n", t->total_refs);
    printf("Faltas de pagina: %d\n", t->total_faltas);
    printf("Acertos de pagina: %d\n",
           t->total_refs - t->total_faltas);
}
