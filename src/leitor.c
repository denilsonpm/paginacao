#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "leitor.h"

/* Libere a sequencia com free(seq) depois de usa-la.

Exemplo:
int tamanho = 0;
int numero_sequencia = 0;

int *seq = carregar_sequencia(
    "entradas/sequencia.txt", "FIFO", 0, &tamanho, &numero_sequencia);
*/

/* Carrega a sequencia da linha correspondente ao algoritmo e ao indice.
Uma linha pode conter varios algoritmos separados por virgula.
Linhas com somente numeros sao compartilhadas por todos os algoritmos.

Exemplo de arquivo:
(FIFO,LFU) 1 2 3 4
(LRU,Otimo) 5 6 7
(SegundaChance) 8 9

Os nomes dos algoritmos sao separados por virgulas.
Os numeros sao separados por espacos.
Linhas iniciadas com # sao ignoradas.
*/
int *carregar_sequencia(const char *caminho, const char *algoritmo,
                        int indice_sequencia, int *tamanho,
                        int *numero_sequencia) {

    int capacidade = 64;
    int *sequencia = malloc((size_t)capacidade * sizeof(int));

    *tamanho = 0;
    *numero_sequencia = 0;

    FILE *arquivo = fopen(caminho, "r");

    char linha[4096];
    int sequencias_encontradas = 0;
    int numero_linha = 0;

    while (fgets(linha, sizeof linha, arquivo) != NULL) {

        char *inicio = linha;

        while (*inicio == ' ' || *inicio == '\t')
            inicio++;

        if (*inicio == '\0' || *inicio == '\n' || *inicio == '#')
            continue;

        numero_linha++;

        char *fim_algoritmos = inicio;

        while (*fim_algoritmos != ' ' &&
            *fim_algoritmos != '\t' &&
            *fim_algoritmos != '\n' &&
            *fim_algoritmos != '\0') {
            fim_algoritmos++;
        }

        char nome_algoritmos[256];

        int tamanho_nome = fim_algoritmos - inicio;

        memcpy(nome_algoritmos, inicio, tamanho_nome);
        nome_algoritmos[tamanho_nome] = '\0';

        char *fim_numero;
        strtol(nome_algoritmos, &fim_numero, 10);

        int sequencia_comum = (*fim_numero == '\0');
        int selecionado = sequencia_comum;

        if (!sequencia_comum) {

            char *nome = strtok(nome_algoritmos, "(),");

            while (nome != NULL) {

                while (*nome == ' ')
                    nome++;

                if (strcasecmp(nome, algoritmo) == 0)
                    selecionado = 1;

                nome = strtok(NULL, "(),");
            }
        }

        if (!selecionado)
            continue;

        char *valores;

        if (sequencia_comum) {
            valores = inicio;
        } else {
            *fim_algoritmos = '\0';
            valores = fim_algoritmos + 1;
        }

        if (sequencias_encontradas++ != indice_sequencia)
            continue;

        if (sequencia_comum)
            valores = inicio;

        *numero_sequencia = numero_linha;

        char *numero = strtok(valores, ", \t\r\n");

        while (numero != NULL) {

            if (*tamanho == capacidade) {
                capacidade *= 2;

                sequencia = realloc(
                    sequencia,
                    (size_t)capacidade * sizeof(int)
                );
            }

            sequencia[(*tamanho)++] = (int)strtol(numero, NULL, 10);

            numero = strtok(NULL, ", \t\r\n");
        }

        break;
    }

    fclose(arquivo);

    return sequencia;
}