#include <stdlib.h>
#include "quadros.h"

/*
Como chamar na main.
Memoria *m = memoria_criar(4);

    if (m == NULL) {
        printf("Erro ao criar memoria.\n");
        return 1;
    }

    printf("Memoria criada com %d quadros.\n", m->n_quadros);

    memoria_destruir(m);

    return 0;
 } */
Memoria *memoria_criar(int n_quadros) {
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

void memoria_destruir(Memoria *m) {
    // Implementar funcao
}

int memoria_buscar(const Memoria *m, int pagina) {
    // Implementar funcao
}
