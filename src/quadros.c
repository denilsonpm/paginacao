#include <stdlib.h>
#include "quadros.h"
#include <stdlib.h>

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

/* Cria uma memoria dado um número de quadros *n_quadros*.
Retorna um ponteiro para a memoria se sucesso, se não NULL. 
As página por padrão nascem com VAZIO. 

Exemplos:
Memória *m = criar_memoria(-3);
NULL
Memória *m = criar_memoria(0);
NULL
Memória *m =criar_memoria(3);
*m, tal que: m->n_quadros == 3, m->ocupados == 0, m->q[{0,1,2}].pagina == VAZIO
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

/* Destroi uma Memoria liberando espaço, dado um ponteiro *m* passado.

Exemplos:
Memória *m =criar_memoria(3);
destruir_memoria(m);
*/
void destruir_memoria(Memoria *m) {
    if (m == NULL){
        return;
    }

    free(m->q);
    free(m);
}

/* Busca dentro de uma *Memoria* uma *pagina*, retornando o índice da página. 
Caso não exista Memoria retorna -1 (VAZIO).

Exemplos:
Memória *m =criar_memoria(3);
m->q[1].pagina = 6;
int buscado = buscar(m, 6);
1
int buscado = buscar(m, 2);
-1
*/
int buscar(const Memoria *m, int pagina) {
    if (m == NULL) {
        return -1;
    }

    for (int i = 0; i < m->n_quadros; i++) {
        if (m->q[i].pagina == pagina) {
            return i;
        }
    }

    return VAZIO;
}

/* Imprime as paginas de uma mémoria *m*.

Exemplos:
Memoria *m = criar_memoria(3);
imprimir_memoria(m);
[-,-,-]
m->q[0].pagina = 2;
imprimir_memoria(m);
[2,-,-]
*/
void imprimir_memoria(Memoria *m) {
    if (m == NULL) {
        printf("[]\n");
        return;
    }
    if (m->q[0].pagina == VAZIO){
        printf(", -");
    } else{
    printf("[%d", m->q[0].pagina);
    }
    for (int i = 1; i < m->n_quadros; i++){
    	if (m->q[i].pagina == VAZIO){
    		printf(", -");
		}
		else{
			printf(", %d",m->q[i].pagina);
		}
    }
    printf("]\n");
}