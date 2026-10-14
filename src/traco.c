#include <stdio.h>
#include <stdlib.h>
#include "traco.h"

void iniciar_traco(Traco *t, int n, int n_quadros, int verbose) {
    *t = (Traco){0};
    t->verbose = verbose;
    t->n_quadros = n_quadros;

    if (verbose) {
        t->passos = calloc(n, sizeof(Passo));

        if (t->passos == NULL) {
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < n; i++) {
            t->passos[i].quadros = malloc(n_quadros * sizeof(int));

            if (t->passos[i].quadros == NULL) {
                destruir_traco(t);
                exit(EXIT_FAILURE);
            }
        }
    }
}

void registrar_passo(Traco *t, int ref, const Memoria *m,
                    int falta, int vitima) {

    int i = t->total_refs++;

    if (falta) {
        t->total_faltas++;
    }

    if (!t->verbose) {
        return;
    }

    Passo *p = &t->passos[i];

    p->referencia = ref;
    p->falta = falta;
    p->vitima = vitima;

    for (int j = 0; j < t->n_quadros; j++) {
        p->quadros[j] = m->q[j].pagina;
    }
}

void imprimir_resumo(const Traco *t, const char *algoritmo) {

    printf("\nAlgoritmo: %s\n", algoritmo);
    printf("Quadros: %d\n\n", t->n_quadros);

    if (t->verbose) {
        printf("Ref\tQuadros\t\tStatus\n");

        for (int i = 0; i < t->total_refs; i++) {
            Passo *p = &t->passos[i];

            printf("%d\t[", p->referencia);

            for (int j = 0; j < t->n_quadros; j++) {
                if (j > 0) printf(",");

                if (p->quadros[j] == VAZIO)
                    printf("-");
                else
                    printf("%d", p->quadros[j]);
            }

            printf("]\t\t");

            if (p->falta) {
                printf("FAULT");

                if (p->vitima != VAZIO)
                    printf(" (sai %d)", p->vitima);
            } else {
                printf("HIT");
            }

            printf("\n");
        }

        printf("\n");
    }

    printf("Total de referencias: %d\n", t->total_refs);
    printf("Faltas de pagina: %d\n", t->total_faltas);
    printf("Acertos de pagina: %d\n",
           t->total_refs - t->total_faltas);
}

void destruir_traco(Traco *t) {
    if (t->passos != NULL) {
        for (int i = 0; i < t->total_refs; i++) {
            free(t->passos[i].quadros);
        }

        free(t->passos);
    }
}
