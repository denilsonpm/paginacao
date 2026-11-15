# paginacao
Trabalho de paginação da disciplina de SO

Forma de execução:

## Gerador:
gcc -Wall -Wextra geradores/main_gerador.c geradores/gerador.c -o gerador
Ele criara a nova sequencia na sequencia2.txt.
## Forma de execução:
./gerador (número de referencias) (variedade do número de páginas. Obs.: vai de 0 até n-1)

## Exemplos:
./gerador 10 10
6 0 1 1 2 8 1 0 5 3

./gerador 10 15
6 0 11 6 7 13 1 0 5 8 

./gerador 16 10
6 0 1 1 2 8 1 0 5 3 4 3 7 4 6 2

## Simulador:

make

## Forma de execução e flags disponíveis:
./simulador -v --arquivo entradas/Belady.txt -q 1 -Q 8 -a ToDos -l teste

## Exemplos:
./simulador -v --arquivo entradas/Belady.txt -q 1 -Q 8 -a ToDos -l teste
./simulador -v --arquivo entradas/Belady.txt -q 1 -Q 8 -a ToDos
