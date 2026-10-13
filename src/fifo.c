typedef struct { int *v; int ini, fim, qtd, cap; } Fila;
void fila_inserir(Fila *f, int x) {
f->v[f->fim] = x;
f->fim = (f->fim + 1) % f->cap;
f->qtd++;
}
int fila_remover(Fila *f); /* TODO: análoga, a partir de ’ini’ */
