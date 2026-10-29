#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "leitor.h"

/* Libere a sequencia com free(seq) depois de usa-la.

Exemplo:
int tamanho = 0;
int *seq = carregar_sequencia(
    "entradas/sequencia.txt", "FIFO", 0, &tamanho);
*/

/* Carrega a sequencia da linha correspondente ao algoritmo e ao indice.
Linhas repetidas para o mesmo algoritmo sao sequencias independentes. Linhas
com somente numeros sao compartilhadas por todos os algoritmos.

Exemplo de arquivo:
(FIFO), 1 2 3 4
(FIFO), 5 6 7
(LFU), 8 9

A virgula separa o nome do algoritmo; os numeros sao separados por espacos.
*/
int *carregar_sequencia(const char *caminho, const char *algoritmo, int indice_sequencia, int *tamanho) {
    int capacidade = 64;
    int *sequencia = malloc((size_t)capacidade * sizeof(int));
    *tamanho = 0;

    FILE *arquivo = fopen(caminho, "r");
    char linha[4096];
    int sequencias_encontradas = 0;

    while (fgets(linha, sizeof linha, arquivo) != NULL) {
        char *nome = strtok(linha, "(), \t\r\n");
        char *valores = strtok(NULL, "\n");
        if (nome == NULL) continue;

        char *fim_nome;
        long primeiro_numero = strtol(nome, &fim_nome, 10);
        int sequencia_comum = *fim_nome == '\0';

        if (sequencia_comum) {
            if (indice_sequencia != 0) continue;
        } else {
            if (strcasecmp(nome, algoritmo) != 0) continue;
            if (sequencias_encontradas++ != indice_sequencia) continue;
        }

        if (sequencia_comum) {
            if (*tamanho == capacidade) {
                capacidade *= 2;
                sequencia = realloc(sequencia, (size_t)capacidade * sizeof(int));
            }
            sequencia[(*tamanho)++] = (int)primeiro_numero;
        }

        char *numero = strtok(valores, ", \t\r");
        while (numero != NULL) {
            if (*tamanho == capacidade) {
                capacidade *= 2;
                sequencia = realloc(sequencia, (size_t)capacidade * sizeof(int));
            }

            sequencia[(*tamanho)++] = (int)strtol(numero, NULL, 10);
            numero = strtok(NULL, ", \t\r");
        }

        if (!sequencia_comum) break;
    }

    fclose(arquivo);
    return sequencia;
}
