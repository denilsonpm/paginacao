#include <stdio.h>
#include <stdlib.h>
#include "gerador.h"

int main(int argc, char *argv[]) {
    (void)argc;


    int quantidade = atoi(argv[1]);
    int num_paginas = atoi(argv[2]);
    const char *arquivo = "entradas/sequencia2.txt";

    return gerar_sequencia(arquivo, quantidade, num_paginas, 42);
}
