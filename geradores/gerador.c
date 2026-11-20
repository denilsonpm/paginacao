#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "gerador.h"

int gerar_sequencia(const char *arquivo, int n, int num_paginas, unsigned int semente, const char *modo) {

    FILE *f = fopen(arquivo, "w");
    srand(semente);
 
int i = 0;

    while (i < n) {

        if (strcasecmp(modo, "sequencial") == 0) {

            int pagina = i % num_paginas;
            fprintf(f, "%d ", pagina);
            i++;

        } else if (strcasecmp(modo, "aleatorio") == 0) {

            int pagina = rand() % num_paginas;
            fprintf(f, "%d ", pagina);
            i++;

        } else if (strcasecmp(modo, "localidade") == 0) {

            int conjunto_paginas[5];
            int tamanho = 3 + rand() % 3;

            if (tamanho > num_paginas) {
                tamanho = num_paginas;
            }

            for (int j = 0; j < tamanho; j++) {
                int pagina;
                int repetida;

                do {
                    pagina = rand() % num_paginas;
                    repetida = 0;

                    for (int k = 0; k < j; k++) {
                        if (conjunto_paginas[k] == pagina) {
                            repetida = 1;
                        }
                    }

                } while (repetida);

                conjunto_paginas[j] = pagina;
            }

            int rajada = 10 + rand() % 21;

            for (int j = 0; j < rajada && i < n; j++) {
                int pagina = conjunto_paginas[rand() % tamanho];
                fprintf(f, "%d ", pagina);
                i++;
            }
        }

        if (i % 70 == 0) {
            fprintf(f, "\n");
        }
    }

    fclose(f);
    return 0;
}
