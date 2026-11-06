#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <getopt.h>
#include <time.h>
#include "leitor.h"
#include "traco.h"
#include "algoritmos.h"

FILE *historico = NULL;

typedef int (*AlgoritmoFn)(const int *, int, int, Traco *);

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

int main(int argc, char *argv[]) {

    char *arquivo = NULL;
    char *algoritmo = "Todos";

    int q_min = 0;
    int q_max = 0;
    int max_informado = 0;
    int verbose = 0;
    int entrada_manual = 0;
    int porcentagem = 0;

    static struct option opcoes[] = {
        {"verbose", no_argument, 0, 'v'},
        {"arquivo", required_argument, 0, 'f'},
        {"manual", no_argument, 0, 'm'},
        {"qmin", required_argument, 0, 'q'},
        {"qmax", required_argument, 0, 'Q'},
        {"algoritmo", required_argument, 0, 'a'},
        {"porcentagem", required_argument, 0, 'p'},
        {0, 0, 0, 0}
    };

    int opcao;

    while ((opcao = getopt_long(argc, argv, "vf:mq:Q:a:p:", opcoes, NULL)) != -1) {
        switch (opcao) {
            case 'v':
                verbose = 1;
                break;

            case 'f':
                arquivo = optarg;
                break;

            case 'm':
                entrada_manual = 1;
                break;

            case 'q':
                q_min = atoi(optarg);
                break;

            case 'Q':
                q_max = atoi(optarg);
                max_informado = 1;
                break;

            case 'a':
                algoritmo = optarg;
                break;

            case 'p':
                porcentagem = atoi(optarg);
                break;
        }
    }

    if (entrada_manual) {
        char *sequencia = NULL;
        size_t capacidade = 0;

        printf("Digite a sequencia de paginas separada por virgulas: ");
        getline(&sequencia, &capacidade, stdin);

        FILE *entrada = fopen("entradas/sequencia2.txt", "w");
        fprintf(entrada, "%s", sequencia);
        fclose(entrada);

        arquivo = "entradas/sequencia2.txt";

        free(sequencia);
    }

    if (!max_informado)
        q_max = q_min;

    if (verbose) {
        historico = fopen("resultados/historico.log", "a");

        time_t agora = time(NULL);
        struct tm *data = localtime(&agora);

        const char *meses[] = {
            "janeiro", "fevereiro", "marco", "abril",
            "maio", "junho", "julho", "agosto",
            "setembro", "outubro", "novembro", "dezembro"
        };

        printf("\nData: %02d de %s de %d\n",
               data->tm_mday, meses[data->tm_mon], data->tm_year + 1900);
        printf("Horario: %02d:%02d:%02d\n",
               data->tm_hour, data->tm_min, data->tm_sec);
        printf("Arquivo: %s\n", arquivo);
        printf("Algoritmo: %s\n", algoritmo);

        fprintf(historico, "\nData: %02d de %s de %d\n",
                data->tm_mday, meses[data->tm_mon], data->tm_year + 1900);
        fprintf(historico, "Horario: %02d:%02d:%02d\n",
                data->tm_hour, data->tm_min, data->tm_sec);
        fprintf(historico, "Arquivo: %s\n", arquivo);
        fprintf(historico, "Algoritmo: %s\n", algoritmo);
    }

    FILE *csv = fopen("resultados/resultados.csv", "w");

    fprintf(csv, "Algoritmo,Sequencia,Quadros,TotalRefs,Faltas,Taxa\n");

    int total_algoritmos = sizeof(ALGS) / sizeof(ALGS[0]);

    for (int a = 0; a < total_algoritmos; a++) {

        int selecionado = (strcasecmp(algoritmo, "Todos") == 0);

        if (!selecionado) {
            char lista[256];
            snprintf(lista, sizeof(lista), "%s", algoritmo);

            char *nome = strtok(lista, ",");

            while (nome != NULL) {
                while (*nome == ' ')
                    nome++;

                if (strcasecmp(nome, ALGS[a].nome) == 0) {
                    selecionado = 1;
                    break;
                }

                nome = strtok(NULL, ",");
            }
        }

        if (!selecionado)
            continue;

        for (int indice_sequencia = 0; ; indice_sequencia++) {

            int tamanho = 0;
            int numero_sequencia = 0;

            int *seq = carregar_sequencia(
                arquivo,
                ALGS[a].nome,
                indice_sequencia,
                &tamanho,
                &numero_sequencia
            );

            if (tamanho == 0) {
                free(seq);
                break;
            }

            int q_ini = q_min;
            int q_fim = q_max;

            if (porcentagem > 0) {
                q_ini = 1;
                q_fim = (tamanho * porcentagem + 99) / 100;

                if (q_fim < 1)
                    q_fim = 1;
            }

            for (int q = q_ini; q <= q_fim; q++) {

                printf("\nExecutando %s, sequencia %d, com %d quadros...\n",
                       ALGS[a].nome, numero_sequencia, q);

                if (historico != NULL) {
                    fprintf(historico,
                            "\nExecutando %s, sequencia %d, com %d quadros...\n",
                            ALGS[a].nome, numero_sequencia, q);
                }

                Traco t;

                iniciar_traco(&t, tamanho, q, verbose);

                ALGS[a].fn(seq, tamanho, q, &t);

                imprimir_resumo(&t, ALGS[a].nome);

                double taxa = t.total_refs > 0
                    ? (double)t.total_faltas / t.total_refs
                    : 0.0;

                fprintf(csv, "%s,%d,%d,%d,%d,%.4f\n",
                        ALGS[a].nome,
                        numero_sequencia,
                        q,
                        t.total_refs,
                        t.total_faltas,
                        taxa);

                destruir_traco(&t);
            }

            free(seq);
        }
    }

    fclose(csv);

    if (historico != NULL) {
        fflush(historico);
        fclose(historico);
        historico = NULL;
    }

    char comando[100];
    snprintf(comando, sizeof(comando), "python3 analise.py %s", algoritmo);
    system(comando);

    return 0;
}