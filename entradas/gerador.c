#include <stdio.h>
#include <stdlib.h>
#include "gerador.h"

void gerar_sequencia(const char *arquivo, int n, int num_paginas, unsigned int semente) {

    if (n <= 0 || num_paginas <= 0) {
        printf("Parametros invalidos.\n");
        return;
    }

    FILE *f = fopen(arquivo, "w");

    if (f == NULL) {
        perror("Erro ao criar arquivo");
        return;
    }

    srand(semente);
 
    for (int i = 0; i < n; i++) {
        int pagina = rand() % num_paginas;

        fprintf(f, "%d ", pagina);

        if ((i + 1) % 70 == 0) {
            fprintf(f, "\n");
        }
    }
    fclose(f);
}
