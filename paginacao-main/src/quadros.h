#ifndef QUADROS_H
#define QUADROS_H

/* VAZIO como -1, representa inexistência de página*/
#define VAZIO -1

// Representa um quadro da Memoria
typedef struct {
    int pagina; /* página carregada ou VAZIO, Não pode ser negativo. */
    int ultimo_uso; /* relógio lógico do último acesso (LRU) */
    int bit_ref; /* bit de referência (Segunda Chance), pode ser 0 ou 1 */
    int contador; /* número de acessos (LFU, opcional). Não pode ser negativo. */
} Quadro;

// Representar a memória
typedef struct {
    Quadro *q; /* vetor com n_quadros posições */
    int n_quadros;/* Não pode ser negativo */
    int ocupados;/* Não pode ser negativo */
} Memoria;

Memoria *criar_memoria(int n_quadros); /* todos VAZIO por padrão */
void destruir_memoria(Memoria *m);
int buscar(const Memoria *m, int pagina); /* índice ou -1 */
#endif
