#include <stdio.h>
#include <stdlib.h>
#include "gerador.h"

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("Uso: %s <arquivo> <quantidade> <num_paginas>\n", argv[0]);
        return 1;
    }

    const char *arquivo = argv[1];
    int quantidade = atoi(argv[2]);
    int num_paginas = atoi(argv[3]);

    gerar_sequencia(arquivo, quantidade, num_paginas, 42);

    return 0;
}
