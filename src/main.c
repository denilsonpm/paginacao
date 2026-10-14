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

int main() {
    int tamanho;
    int n_quadros = 3;
    int proxima_vitima = 0;

    int *seq = carregar_sequencia("entradas/Belady.txt", &tamanho);

    if (seq == NULL) {
        return 1;
    }

    Memoria *m = memoria_criar(n_quadros);

    if (m == NULL) {
        free(seq);
        return 1;
    }

    Traco t = {0, 0, 1};

    for (int i = 0; i < tamanho; i++) {

        int pagina = seq[i];
        int indice = memoria_buscar(m, pagina);

        int falta = (indice == -1);
        int vitima = VAZIO;

        if (falta) {

             if (m->ocupados < m->n_quadros) {

                m->q[m->ocupados].pagina = pagina;
                m->ocupados++;

            } else {

                vitima = m->q[proxima_vitima].pagina;

                m->q[proxima_vitima].pagina = pagina;

                proxima_vitima =
                    (proxima_vitima + 1) % m->n_quadros;
            }
        }

        registrar_passo(&t, pagina, m, falta, vitima);
    }

   imprimir_resumo(&t, "FIFO", n_quadros);

    salvar_resultado(
        "FIFO",
        n_quadros,
        t.total_faltas,
        t.total_refs - t.total_faltas,
        t.total_faltas - n_quadros
    );
    
    memoria_destruir(m);
    free(seq);
    
    return 0;
}
