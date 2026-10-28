#include <stdio.h>
#include <stdlib.h>
#include "gerador.h"

// Para compilar: gcc -Wall -Wextra geradores/main_gerador.c geradores/gerador.c -o gerador
int main(int argc, char *argv[]) {

    const char *arquivo = "entradas/sequencia2.txt";
    int quantidade = atoi(argv[1]);
    int num_paginas = atoi(argv[2]);

    gerar_sequencia(arquivo, quantidade, num_paginas, 42);

    return 0;
}
