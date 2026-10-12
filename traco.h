#ifndef TRACO_H
#define TRACO_H

typedef struct {
int total_refs;
int total_faltas;
int verbose; /* 0 = só totais (útil em traços longos) */
} Traco;
void registrar_passo(Traco *t, int ref, const Memoria *m,
int falta, int vitima /* ou VAZIO */);

void imprimir_resumo(const Traco *t, const char *algoritmo, int n_quadros);

#endif
