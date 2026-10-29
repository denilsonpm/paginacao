
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitor.h"
#include "traco.h"
#include "algoritmos.h"

int main(int argc, char *argv[]) {

    const char *arquivo = NULL;
    const char *algoritmo = "Todos";
    int q_min = 0;
    int q_max = 0;
    int verbose = 0;
    int max_informado = 0;
    int entrada_manual = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) { //verbose
            verbose = 1;
        } else if (strcmp(argv[i], "-f") == 0) { //arquivo
            arquivo = argv[++i];
        } else if (strcmp(argv[i], "-m") == 0) { //entrada manual
            entrada_manual = 1;
        } else if (strcmp(argv[i], "-q") == 0) { //min_quadros
            q_min = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-Q") == 0) { //max_quadros
            q_max = atoi(argv[++i]);
            max_informado = 1;
        } else if (strcmp(argv[i], "-a") == 0) { //algoritmo
            algoritmo = argv[++i];
        }
    }

    if (max_informado == 0) {
        q_max = q_min;
    }

    if (entrada_manual) {
        char *sequencia = NULL;
        size_t capacidade = 0;

        arquivo = "entradas/sequencia2.txt";
        printf("Digite a sequencia de paginas separada por virgulas: ");
        if (getline(&sequencia, &capacidade, stdin) == -1) {
            free(sequencia);
            return 1;
        }

        FILE *entrada = fopen(arquivo, "w");
        if (entrada == NULL) {
            perror("Nao foi possivel criar o arquivo de entrada");
            free(sequencia);
            return 1;
        }

        fputs(sequencia, entrada);
        fclose(entrada);
        free(sequencia);
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

    FILE *csv = fopen("resultados/resultados.csv", "w");

    if (csv == NULL) {
        return 1;
    }

    fprintf(csv, "Algoritmo,Sequencia,Quadros,TotalRefs,Faltas,Taxa\n");

    for (size_t a = 0; a < total_algs; a++) {

        if (strcmp(algoritmo, "Todos") != 0 &&
            strcmp(algoritmo, ALGS[a].nome) != 0) {
            continue;
        }

        for (int numero_sequencia = 0;; numero_sequencia++) {
            int tamanho;
            int *seq = carregar_sequencia(
                arquivo, ALGS[a].nome, numero_sequencia, &tamanho);

            if (tamanho == 0) {
                free(seq);
                break;
            }

            for (int q = q_min; q <= q_max; q++) {

                printf("\nExecutando %s, sequencia %d, com %d quadros...\n",
                    ALGS[a].nome, numero_sequencia + 1, q);
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

                fprintf(csv, "%s,%d,%d,%d,%d,%.4f\n",
                    ALGS[a].nome, numero_sequencia + 1, q,
                    t.total_refs, t.total_faltas, taxa);

                destruir_traco(&t);
            }

            free(seq);
        }
    }

    fclose(csv);

    int resultado = system("python3 analise.py");

    if (resultado != 0) {
        return 1;
    }

    return 0;
}
