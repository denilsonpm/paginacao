/*#call functions here

typedef int (*AlgoritmoFn)(const int *seq, int n, int n_quadros, Traco *t);
static const struct { const char *nome; AlgoritmoFn fn; } ALGS[] = {
{"FIFO", executar_fifo}, {"LRU", executar_lru},
{"Otimo", executar_otimo}, {"SegundaChance", executar_segunda_chance},
};
...
for (int q = q_min; q <= q_max; q++)
for (size_t a = 0; a < sizeof ALGS / sizeof ALGS[0]; a++) {
TODO: executar, imprimir, gravar linha no CSV:
algoritmo,num_quadros,total_refs,total_faltas,taxa 

*/

// main a seguir executando somente um teste
#include <stdio.h>
#include <stdlib.h>
#include "leitor.h"
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

int main() {
    int tamanho;
    int n_quadros = 3;

    int *seq = carregar_sequencia("entradas/Belady.txt", &tamanho);

    if (seq == NULL) {
        return 1;
    }

    Traco t;

    iniciar_traco(&t, tamanho, n_quadros, 1);

    if (executar_otimo(seq, tamanho, n_quadros, &t) != 0) {
        destruir_traco(&t);
        free(seq);
        return 1;
    }

    imprimir_resumo(&t, "OTIMO");

    destruir_traco(&t);
    free(seq);

    return 0;
}
