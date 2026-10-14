#include <stdlib.h>
#include "quadros.h"

/*
Como chamar na main.
Memoria *m = criar_memoria(4);

    if (m == NULL) {
        printf("Erro ao criar memoria.\n");
        return 1;
    }

    printf("Memoria criada com %d quadros.\n", m->n_quadros);

    destruir_memoria(m);
     
    int indice = buscar(m,4);
    // procura a pagina 4 na sequencia e devolve o indice da pagina ou -1 se n tiver.
    */
Memoria *criar_memoria(int n_quadros) {
    if (n_quadros <= 0) {
        return NULL;
    }

    Memoria *m = malloc(sizeof(Memoria));

    if (m == NULL) {
        return NULL;
    }

    m->q = calloc(n_quadros, sizeof(Quadro));

    if (m->q == NULL) {
        free(m);
        return NULL;
    }

    m->n_quadros = n_quadros;
    m->ocupados = 0;

    for (int i = 0; i < n_quadros; i++) {
        m->q[i].pagina = VAZIO;
        m->q[i].ultimo_uso = 0;
        m->q[i].bit_ref = 0;
        m->q[i].contador = 0;
    }

    return m;
}

void destruir_memoria(Memoria *m) {
    if (m == NULL){
        return;
    }

    free(m->q);
    free(m);
}

int buscar(const Memoria *m, int pagina) {
    if (m == NULL) {
        return -1;
    }

    for (int i = 0; i < m->n_quadros; i++) {
        if (m->q[i].pagina == pagina) {
            return i;
        }
    }

    return -1;
}
