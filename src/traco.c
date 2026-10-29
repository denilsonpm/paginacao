#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "traco.h"

/* Prepara o ambiente com a memória certa alocada, dado um ponteiro traco *t*,
número de referencias *n*, numero de quadros disponíveis *n_quadros* e uma flag *verbose*.

Exemplos:
Traco t;
int n_quadros = 3;
int total_refs_previstas = 4;
int verbose = 1;
iniciar_traco(&t,total_refs_previstas,n_quadros,1);
destruir_traco(&t);
*/
void iniciar_traco(Traco *t, int n, int n_quadros, int verbose) {
    *t = (Traco){0};
    t->verbose = verbose;
    t->n_quadros = n_quadros;

    if (verbose) {
        t->passos = calloc(n, sizeof(Passo));

    }
}

/* Registra um passo nno histórico, atualizando quantas referências e quantas faltas
de página ocorreram, dado um ponteiro traco *t*, número da página solicitada *ref*,
um ponteiro para a memoria *m*, indicador se houve *falta* (0 ou 1)
e o número da página que foi removida *vitima*.

Exemplos:
Traco t;
int n_quadros = 3;
int total_refs_previstas = 4;
int verbose = 1;
iniciar_traco(&t,total_refs_previstas,n_quadros,verbose);
Memoria *mem = criar_memoria(n_quadros);
mem->q[0].pagina = 1;
mem->ocupados = 1;
registrar_passo(&t, 1, mem, 1, VAZIO);
destruir_traco(&t);
*/
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

    p->quadros = malloc(t->n_quadros * sizeof(int));

    p->referencia = ref;
    p->falta = falta;
    p->vitima = vitima;

    for (int j = 0; j < t->n_quadros; j++) {
        p->quadros[j] = m->q[j].pagina;
    }
}

/* Imprime um resumo, dado um ponteiro para Traco *t*, e um ponteiro para o algoritmo *algoritmo*.
Esse resumo contem, o nome do algoritmo, o número de quadros, se verbose verdadeira cada passo,
o total de referencias, as faltas e os acertos de pagina.

Exemplos:
Traco t;
int n_quadros = 3;
int total_refs_previstas = 4;
int verbose = 1;
iniciar_traco(&t,total_refs_previstas,n_quadros,verbose);
Memoria *mem = criar_memoria(n_quadros);
mem->q[0].pagina = 1;
mem->ocupados = 1;
registrar_passo(&t, 1, mem, 1, VAZIO);
imprimir_resumo(&t,"FIFO");

Algoritmo: FIFO
Quadros: 3

Ref	Quadros		Status
1	[1,-,-]		FALTA

Total de referencias: 1
Faltas de pagina: 1
Acertos de pagina: 0

destruir_traco(&t);
destruir_memoria(mem);
*/

void imprimir_resumo(const Traco *t, const char *algoritmo) {

    if (strcmp(algoritmo, "SegundaChance") == 0 || strcmp(algoritmo, "LFU") == 0) {
        /* Implementado dentro de seus respectivos arquivos */
    } else {

        printf("\nAlgoritmo: %s\n", algoritmo);
        printf("Quadros: %d\n\n", t->n_quadros);

        if (t->verbose) {
            printf("Ref\tQuadros\t\tStatus\n");

            for (int i = 0; i < t->total_refs; i++) {
                const Passo *p = &t->passos[i];

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
                    printf("FALTA");

                    if (p->vitima != VAZIO)
                        printf(" (sai %d)", p->vitima);
                } else {
                    printf("ACERTO");
                }

                printf("\n");
            }
        }

        printf("\n");
    }

    double taxa = t->total_refs > 0
        ? (double)t->total_faltas / t->total_refs * 100
        : 0.0;

    printf("Total de referencias: %d\n", t->total_refs);
    printf("Faltas de pagina: %d (%.0f%%)\n", t->total_faltas, taxa);
    printf("Acertos de pagina: %d\n", t->total_refs - t->total_faltas);
}

/*Destroi um Traco liberando espaço, dado um ponteiro *t* passado.

Exemplos:
Traco t;
int n_quadros = 3;
int total_refs_previstas = 4;
int verbose = 1;
iniciar_traco(&t,total_refs_previstas,n_quadros,verbose);
destruir_traco(&t);
*/
void destruir_traco(Traco *t) {
    if (t->passos != NULL) {
        for (int i = 0; i < t->total_refs; i++) {
            free(t->passos[i].quadros);
        }

        free(t->passos);
    }

    *t = (Traco){0};
}
