
#include <stdio.h>
#include <stdlib.h>
#include "leitor.h"
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

int main(int argc, char *argv[]) {

    if (argc != 3 && argc != 4) {
        printf("Uso: %s <arquivo> <q_min> [q_max]\n", argv[0]);
        return 1;
    }

    const char *arquivo = argv[1];
    int q_min = atoi(argv[2]);
    int q_max = (argc == 4) ? atoi(argv[3]) : q_min;

    if (q_min < 1 || q_max < q_min) {
        printf("Intervalo de quadros invalido.\n");
        return 1;
    }

    int tamanho;
    int *seq = carregar_sequencia(arquivo, &tamanho);

    if (seq == NULL) {
        return 1;
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

    FILE *csv = fopen("resultados/resultados.csv", "w");

    if (csv == NULL) {
        perror("Erro ao abrir CSV");
        free(seq);
        return 1;
    }

    fprintf(csv, "Algoritmo,Quadros,TotalRefs,Faltas,Taxa\n");

    for (size_t a = 0; a < sizeof ALGS / sizeof ALGS[0]; a++) {
        for (int q = q_min; q <= q_max; q++) {

            printf("\nExecutando %s com %d quadros...\n", ALGS[a].nome, q);
            fflush(stdout);

            Traco t;
            iniciar_traco(&t, tamanho, q, 1);

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

    return 0;
}
