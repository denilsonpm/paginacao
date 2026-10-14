#ifndef QUADROS_H
#define QUADROS_H

#define VAZIO -1

typedef struct {
int pagina; /* página carregada ou VAZIO */
int ultimo_uso; /* relógio lógico do último acesso (LRU) */
int bit_ref; /* bit de referência (Segunda Chance) */
int contador; /* número de acessos (LFU, opcional) */
} Quadro;

typedef struct {
Quadro *q; /* vetor com n_quadros posições */
int n_quadros;
int ocupados;
} Memoria;

Memoria *criar_memoria(int n_quadros); /* todos VAZIO */
void destruir_memoria(Memoria *m);
int buscar(const Memoria *m, int pagina); /* índice ou -1 */

#endif
