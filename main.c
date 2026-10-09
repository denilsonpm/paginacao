#call functions here

typedef int (*AlgoritmoFn)(const int *seq, int n, int n_quadros, Traco *t);
static const struct { const char *nome; AlgoritmoFn fn; } ALGS[] = {
{"FIFO", executar_fifo}, {"LRU", executar_lru},
{"Otimo", executar_otimo}, {"SegundaChance", executar_segunda_chance},
};
...
for (int q = q_min; q <= q_max; q++)
for (size_t a = 0; a < sizeof ALGS / sizeof ALGS[0]; a++) {
/* TODO: executar, imprimir, gravar linha no CSV:
algoritmo,num_quadros,total_refs,total_faltas,taxa */

}
