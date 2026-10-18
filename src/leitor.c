#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "leitor.h"

  /*Usar o free(seq) dps de usar
  para chamar 
  int tamanho;
  
  int *seq = carregar_sequencia("sequencia.txt", &tamanho);*/

/*Carrega uma sequencia de números, de um arq.txt *caminho* e salva o total de elementos
lidos em *tamanho*, retornando um ponteiro para o vetor preenchido.

Exemplos:
int tamanho = 0;
int *seq = carregar_sequencia(seq.txt, &tamanho);
*/
int *carregar_sequencia(const char *caminho, int *tamanho) {
    FILE *f = fopen(caminho, "r");
    if (!f) {
        perror(caminho);
        return NULL;
    }

    int cap = 64, n = 0;
    int *seq = malloc(cap * sizeof(int));

    if (seq == NULL) {
        printf("Falha ao carregar a sequencia.\n");
        fclose(f);
        return NULL;
    }

    char linha[4096];

    while (fgets(linha, sizeof linha, f)) {
        if (linha[0] == '#') continue;

        char *resto;
        char *token = strtok(linha, " \t\n\r");

        while (token != NULL) {
            errno = 0;
            long numero = strtol(token, &resto, 10);

            /* validar */
          
            if (resto == token || *resto != '\0' ||
                errno == ERANGE ||
                numero < 0 ||
                numero > INT_MAX) {
                fprintf(stderr, "Número inválido: %s\n", token);
                free(seq);
                fclose(f);
                return NULL;
            }

            if (n == cap) {
                cap *= 2;

                int *temp = realloc(seq, cap * sizeof(int));

                if (temp == NULL) {
                    fprintf(stderr, "Erro: Falha de memória ao tentar expandir o array.\n");
                    free(seq);
                    fclose(f);
                    return NULL;
                }

                seq = temp;
            }
            /* Incrementa */
            seq[n] = (int)numero;
            n++;

            token = strtok(NULL, " \t\n\r");
        }
    }

    if (ferror(f)) {
        perror("Erro ao ler o ficheiro");
        free(seq);
        fclose(f);
        return NULL;
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
void imprimir_txt( const int *seq, int tamanho){
printf("Sequencia carregada: [");
    for (int i = 0; i < tamanho; i++) {
        if (i > 0) {
            printf(", ");
        }
        printf("%d", seq[i]);
    }
    printf("]\n");
}