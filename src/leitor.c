
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "leitor.h"

int *carregar_sequencia(const char *caminho, int *tamanho) {
    FILE *arquivo = fopen(caminho, "r");

    if (tamanho == NULL) {
        if (arquivo != NULL) fclose(arquivo);
        return NULL;
    }

    *tamanho = 0;

    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    int capacidade = 16;
    int *seq = malloc(capacidade * sizeof(int));

    if (seq == NULL) {
        fclose(arquivo);
        return NULL;
    }

    char linha[4096];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char *token = strtok(linha, " \t\n\r");

        while (token != NULL) {
            if (token[0] == '#') {
                break;
            }

            errno = 0;
            char *fim;
            long numero = strtol(token, &fim, 10);

            if (errno == ERANGE ||
                fim == token ||
                *fim != '\0' ||
                numero < 0 ||
                numero > INT_MAX) {

                fprintf(stderr, "Referencia invalida: %s\n", token);
                free(seq);
                fclose(arquivo);
                *tamanho = 0;
                return NULL;
            }

            if (*tamanho == capacidade) {
                if (capacidade > INT_MAX / 2) {
                    free(seq);
                    fclose(arquivo);
                    *tamanho = 0;
                    return NULL;
                }

                capacidade *= 2;

                int *novo = realloc(seq, capacidade * sizeof(int));

                if (novo == NULL) {
                    free(seq);
                    fclose(arquivo);
                    *tamanho = 0;
                    return NULL;
                }

                seq = novo;
            }

            seq[*tamanho] = (int)numero;
            (*tamanho)++;

            token = strtok(NULL, " \t\n\r");
        }
    }

    if (ferror(arquivo)) {
        perror("Erro ao ler arquivo");
        free(seq);
        fclose(arquivo);
        *tamanho = 0;
        return NULL;
    }

    fclose(arquivo);

    return seq;
}
