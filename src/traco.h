#ifndef TRACO_H
#define TRACO_H

#include "quadros.h"

typedef struct {
    int referencia;
    int *quadros;
    int falta;
    int vitima;
} Passo;

typedef struct {
    int total_refs;
    int total_faltas;
    int verbose;
    int n_quadros;
    Passo *passos;
} Traco;

void iniciar_traco(Traco *t, int n, int n_quadros, int verbose);
void registrar_passo(Traco *t, int ref, const Memoria *m,
                    int falta, int vitima);
void imprimir_resumo(const Traco *t, const char *algoritmo);
void destruir_traco(Traco *t);

#endif
