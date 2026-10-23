#ifndef TRACO_H
#define TRACO_H

#include "quadros.h"

// representa um passo, um acesso na memória (um log).
typedef struct {
    int referencia; // o número do pagina.
    int *quadros; // estado atual dos quadros da memória
    int falta; // indica 1 se houve fault, caso contrario 0.
    int vitima; //pagina removida da memória
} Passo;

// representa o histórico, onde cada Passo é "salvo".
typedef struct {
    int total_refs;
    int total_faltas;
    int verbose; // 1 imprimi tudo, 0 = só totais (útil em traços longos)
    int n_quadros; // quadros disponíveis na memória.
    Passo *passos;
} Traco;

void iniciar_traco(Traco *t, int n, int n_quadros, int verbose);
void registrar_passo(Traco *t, int ref, const Memoria *m, int falta, int vitima);
void imprimir_resumo(const Traco *t, const char *algoritmo);
void destruir_traco(Traco *t);

#endif
