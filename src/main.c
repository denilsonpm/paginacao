
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitor.h"
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

int main(int argc, char *argv[]) {

    if (argc < 4 || argc > 6) return 1;

    const char *arquivo = argv[1];
    const char *algoritmo = "Todos";
    int q_min = atoi(argv[2]);
    int q_max = q_min;
    int verbose = atoi(argv[argc - 1]);

    if (argc == 5) {
        if (atoi(argv[3]) != 0) q_max = atoi(argv[3]);
        else algoritmo = argv[3];
    }

    if (argc == 6) {
        q_max = atoi(argv[3]);
        algoritmo = argv[4];
    }

    typedef int (*AlgoritmoFn)(const int *seq, int n, int n_quadros, Traco *t);

    static const struct {
        const char *nome;
        AlgoritmoFn fn;
    } ALGS[] = {
        {"FIFO", executar_fifo},
        {"LRU", executar_lru},
        {"Otimo", executar_otimo},
        {"SegundaChance", executar_segunda_chance},
        {"LFU", executar_lfu}
    };

    size_t total_algs = sizeof ALGS / sizeof ALGS[0];

    int tamanho;
    int *seq = carregar_sequencia(arquivo, &tamanho);

    if (seq == NULL) {
        return 1;
    }

    FILE *csv = fopen("resultados/resultados.csv", "w");

    if (csv == NULL) {
        free(seq);
        return 1;
    }

    fprintf(csv, "Algoritmo,Quadros,TotalRefs,Faltas,Taxa\n");

    for (size_t a = 0; a < total_algs; a++) {

        if (strcmp(algoritmo, "Todos") != 0 &&
            strcmp(algoritmo, ALGS[a].nome) != 0) {
            continue;
        }

        for (int q = q_min; q <= q_max; q++) {

            printf("\nExecutando %s com %d quadros...\n", ALGS[a].nome, q);
            fflush(stdout);

            Traco t;
            iniciar_traco(&t, tamanho, q, verbose);

            int resultado = ALGS[a].fn(seq, tamanho, q, &t);

            if (resultado != 0) {
                destruir_traco(&t);
                continue;
            }

            imprimir_resumo(&t, ALGS[a].nome);

            double taxa = t.total_refs > 0
                ? (double)t.total_faltas / t.total_refs
                : 0.0;

            fprintf(csv, "%s,%d,%d,%d,%.4f\n",
                ALGS[a].nome, q, t.total_refs, t.total_faltas, taxa);

            destruir_traco(&t);
        }
    }

    fclose(csv);
    free(seq);

    int resultado = system("python3 analise.py");

    if (resultado != 0) {
        return 1;
    }

    return 0;
}
