#include <stdio.h>
#include <stdlib.h>
#include "gerador.h"

int gerar_sequencia(const char *arquivo, int n, int num_paginas, unsigned int semente) {

    if (n <= 0 || num_paginas <= 0) {
        fprintf(stderr, "A quantidade de referencias e paginas deve ser positiva.\n");
        return 1;
    }

    FILE *f = fopen(arquivo, "w");
    if (f == NULL) {
        perror("Nao foi possivel criar o arquivo");
        return 1;
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
    return 0;
}
