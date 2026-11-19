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
FIFO,LFU
1 2 3 4

LRU,Otimo
5 6 7

SegundaChance
8 9

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

    if (sequencia == NULL)
        return NULL;

    FILE *arquivo = fopen(caminho, "r");

    if (arquivo == NULL) {
        free(sequencia);
        return NULL;
    }

    char linha[4096];
    int sequencias_encontradas = 0;
    int numero_bloco = 0;
    int bloco_ativo = 0;
    int selecionado = 0;

    while (fgets(linha, sizeof linha, arquivo) != NULL) {

        char *inicio = linha;

        while (*inicio == ' ' || *inicio == '\t')
            inicio++;

        if (*inicio == '\0' || *inicio == '\n' || *inicio == '#')
            continue;

        int novo_bloco =
            ((*inicio >= 'A' && *inicio <= 'Z') ||
             (*inicio >= 'a' && *inicio <= 'z'));

        if (novo_bloco) {

            if (bloco_ativo) {

                if (selecionado) {

                    if (sequencias_encontradas == indice_sequencia) {
                        *numero_sequencia = numero_bloco;
                        fclose(arquivo);
                        return sequencia;
                    }

                    sequencias_encontradas++;
                }
            }

            bloco_ativo = 1;
            selecionado = 0;
            numero_bloco++;

            char *numeros = inicio;

            while (*numeros != '\0' &&
                   !(*numeros >= '0' && *numeros <= '9')) {
                numeros++;
            }

            char separador = *numeros;
            *numeros = '\0';

            char nomes[4096];
            strcpy(nomes, inicio);

            char *nome = strtok(nomes, ",");

            while (nome != NULL) {

                while (*nome == ' ' || *nome == '\t')
                    nome++;

                char *fim = nome + strlen(nome);

                while (fim > nome &&
                       (fim[-1] == ' ' || fim[-1] == '\t' ||
                        fim[-1] == '\n' || fim[-1] == '\r')) {
                    *--fim = '\0';
                }

                if (strcasecmp(nome, algoritmo) == 0)
                    selecionado = 1;

                nome = strtok(NULL, ",");
            }

            *numeros = separador;

            inicio = numeros;

        } else if (!bloco_ativo) {

            bloco_ativo = 1;
            selecionado = 1;
            numero_bloco++;
        }

        if (selecionado &&
            sequencias_encontradas == indice_sequencia) {

            char *numero = strtok(inicio, " ,\t\r\n");

            while (numero != NULL) {

                char *fim_numero;
                long valor = strtol(numero, &fim_numero, 10);

                if (numero == fim_numero ||
                    *fim_numero != '\0' ||
                    valor < 0) {

                    fprintf(stderr, "Erro: sequencia invalida.\n");
                    fclose(arquivo);
                    free(sequencia);
                    *tamanho = 0;
                    *numero_sequencia = -1;
                    return NULL;
                }

                if (*tamanho == capacidade) {

                    capacidade *= 2;

                    int *temporario = realloc(
                        sequencia,
                        (size_t)capacidade * sizeof(int)
                    );

                    if (temporario == NULL) {
                        free(sequencia);
                        fclose(arquivo);
                        *tamanho = 0;
                        return NULL;
                    }

                    sequencia = temporario;
                }

                sequencia[(*tamanho)++] = (int)valor;

                numero = strtok(NULL, " ,\t\r\n");
            }
        }
    }

    if (bloco_ativo && selecionado &&
        sequencias_encontradas == indice_sequencia) {

        *numero_sequencia = numero_bloco;
        fclose(arquivo);
        return sequencia;
    }

    fclose(arquivo);
    free(sequencia);

    return NULL;
}
