int *carregar_sequencia(const char *caminho, int *tamanho) {
FILE *f = fopen(caminho, "r");
if (!f) { perror(caminho); return NULL; }
int cap = 64, n = 0;
int *seq = malloc(cap * sizeof(int));
char linha[4096];
while (fgets(linha, sizeof linha, f)) {
if (linha[0] == ’#’) continue;
/* TODO: percorrer a linha com strtok (ou sscanf com %n),
converter cada token com strtol, validar,
crescer ’seq’ com realloc quando n == cap */
}
...
}