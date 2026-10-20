#include <stdio.h>
#include <stdlib.h>
#include "gerador.h"

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Uso: %s <arquivo> <quantidade> <num_paginas>\n", argv[0]);
        return 1;
    }

    const char *arquivo = "entradas/sequencia2";
    int quantidade = atoi(argv[1]);
    int num_paginas = atoi(argv[2]);

    gerar_sequencia(arquivo, quantidade, num_paginas, 42);

    return 0;
}
