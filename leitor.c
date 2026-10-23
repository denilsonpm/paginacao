#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitor.h"

/*Usar o free(seq) dps de usar
para chamar
int tamanho;

int *seq = carregar_sequencia("sequencia.txt", &tamanho);*/

/*Carrega uma sequencia de números, de um arq.txt *caminho* e salva o total de elementos
lidos em *tamanho*, retornando um ponteiro para o vetor preenchido.

Exemplos:
int tamanho = 0;
int *seq = carregar_sequencia("seq.txt", &tamanho);
*/
int *carregar_sequencia(const char *caminho, int *tamanho) {
    *tamanho = 0;

    FILE *f = fopen(caminho, "r");

    int cap = 64, n = 0;
    int *seq = malloc((size_t)cap * sizeof(int));


    char linha[4096];

    while (fgets(linha, sizeof linha, f)) {
        char *resto;
        char *token = strtok(linha, " \t\n\r");

        while (token != NULL) {
            if (token[0] == '#') break;

            long numero = strtol(token, &resto, 10);

            if (n == cap) {
                cap *= 2;

                seq = realloc(seq, (size_t)cap * sizeof(int));
            }

            /* Incrementa */
            seq[n] = (int)numero;
            n++;

            token = strtok(NULL, " \t\n\r");
        }
    }

    fclose(f);
    *tamanho = n;

    return seq;
}

/* Imprime os numeros de uma sequencia, dado um ponteiro *seq* e um *tamanho*.

Exemplos:
arq.txt
1 2 3 4      5
int tamanho = 0;
int *seq = carregar_sequencia("seq.txt", &tamanho);
imprimir_txt(seq,tamanho);
[1, 2, 3, 4, 5]
*/
void imprimir_txt(const int *seq, int tamanho) {
    printf("Sequencia carregada: [");

    for (int i = 0; i < tamanho; i++) {
        if (i > 0) {
            printf(", ");
        }

        printf("%d", seq[i]);
    }

    printf("]\n");
}
