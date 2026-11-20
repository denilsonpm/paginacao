#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "gerador.h"

int main(int argc, char *argv[]) {

    const char *arquivo = "entradas/sequencia2.txt";
    const char *modo = "aleatorio";

    int quantidade = 0;
    int num_paginas = 0;
    int opcao;

    static struct option opcoes[] = {
        {"sequencial", no_argument, 0, 's'},
        {"aleatorio", no_argument, 0, 'a'},
        {"localidade", no_argument, 0, 'l'},
        {"quantidade", required_argument, 0, 'q'},
        {"paginas", required_argument, 0, 'p'},
        {0, 0, 0, 0}
    };

    while ((opcao = getopt_long(argc, argv, "salq:p:", opcoes, NULL)) != -1) {

        switch (opcao) {

            case 's':
                modo = "sequencial";
                break;

            case 'a':
                modo = "aleatorio";
                break;

            case 'l':
                modo = "localidade";
                break;

            case 'q':
                quantidade = atoi(optarg);
                break;

            case 'p':
                num_paginas = atoi(optarg);
                break;

            default:
                return 1;
        }
    }

    if (quantidade <= 0 || num_paginas <= 0) {
        printf("qtd e numero de paginas tem que ser maior que 0!.\n");
        return 1;
    }

    return gerar_sequencia(arquivo, quantidade, num_paginas, 42, modo);
}