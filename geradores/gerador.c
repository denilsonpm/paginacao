#include <stdio.h>
#include <stdlib.h>
#include "gerador.h"

int gerar_sequencia(const char *arquivo, int n, int num_paginas, unsigned int semente) {

    FILE *f = fopen(arquivo, "w");
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
