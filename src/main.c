
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitor.h"
#include "quadros.h"
#include "traco.h"
#include "algoritmos.h"

int main(int argc, char *argv[]) {

    if (argc < 3 || argc > 5) {
        printf("Uso: %s <arquivo> <q_min> [q_max] [algoritmo]\n", argv[0]);
        printf("Algoritmos disponiveis: FIFO, LRU, Otimo, SegundaChance, LFU\n");
        printf("Respeite letras maiusculas e minusculas, sem espacos.\n");
        printf("Se nenhum algoritmo for informado, todos serao executados.\n");
        return 1;
    }

    const char *arquivo = argv[1];
    int q_min = atoi(argv[2]);
    int q_max = q_min;
    const char *algoritmo = "Todos";

    if (argc == 4) {
        char *fim;
        long valor = strtol(argv[3], &fim, 10);

        if (*fim == '\0') {
            q_max = atoi(argv[3]);
        } else {
            algoritmo = argv[3];
        }
    }

    if (argc == 5) {
        q_max = atoi(argv[3]);
        algoritmo = argv[4];
    }

    if (q_min < 1 || q_max < q_min) {
        printf("Intervalo de quadros invalido.\n");
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

    size_t total_algs = sizeof ALGS / sizeof ALGS[0];

    int encontrado = strcmp(algoritmo, "Todos") == 0;

    for (size_t a = 0; a < total_algs; a++) {
        if (strcmp(algoritmo, ALGS[a].nome) == 0) {
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Algoritmo '%s' invalido.\n", algoritmo);
        printf("Algoritmos disponiveis: ");

        for (size_t a = 0; a < total_algs; a++) {
            printf("%s%s", ALGS[a].nome,
                a + 1 < total_algs ? ", " : "\n");
        }

        printf("Respeite letras maiusculas e minusculas, sem espacos.\n");
        return 1;
    }

    int tamanho;
    int *seq = carregar_sequencia(arquivo, &tamanho);

    if (seq == NULL) {
        return 1;
    }

    FILE *csv = fopen("resultados/resultados.csv", "w");

    if (csv == NULL) {
        perror("Erro ao abrir CSV");
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

    int resultado = system("python3 analise.py");

    if (resultado != 0) {
        fprintf(stderr, "Erro ao gerar o grafico.\n");
        return 1;
    }

    return 0;
}
