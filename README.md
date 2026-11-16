# Trabalho de paginação da disciplina de SO

### Exigências:
-gcc e make
-python3 com matplotlib

`pip install matplotlib`

### Execução:
Sempre execute na raiz do projeto. \
A seguir estão os passos de como executar o gerador de sequência e o simulador. 

## Gerador:
gcc -Wall -Wextra geradores/main_gerador.c geradores/gerador.c -o gerador \
*A nova sequencia será criada na *sequencia2.txt*.

### Forma de execução:
./gerador q v

Em que: \
q->quantidade de referencias \
v->variedade do número de páginas. Obs.: começa em 0 e vai até v - 1

### Exemplos:
./gerador 10 10 \
6 0 1 1 2 8 1 0 5 3

./gerador 10 15 \
6 0 11 6 7 13 1 0 5 8 

./gerador 16 10 \
6 0 1 1 2 8 1 0 5 3 4 3 7 4 6 2

## Simulador:

make

### Forma de execução e flags disponíveis:

./ simulador flags

Tipos de flags:
| flag curta/longa | argumento | o que faz | Obrigatória? |
| :--- | :---: | :--- | :---: |
|- f ou --arquivo | caminho. Ex.: sequencia1.txt | LÊ as referencias do arq.txt escolhido. | Sim, exceto se usar -m|
|- m ou --manual | - | Depois de executar abre um campo para escrever números. Ex.: 1,2,3,4,5. Depois lê as referencias digitadas. | Sim, exceto se usar -f |
|- q ou --qmin | número maior que 0 | Número mínimo de quadros | Sim, exceto se usar -p |
|- Q ou --qmax | número maior que 0 | Número máximo de quadros, tem que ser maior que o mínimo de quadros | Não |
|- p ou --porcentagem | número entre 1-100 | Varre de 1 até P% do tamanho da sequência (arredondado para cima) | Sim, exceto se usar -q/-Q |
|- a ou --arquivo | nome ou combinação separada por virgulas. Ex.: FIFO,LRU,LFU,SegundaChance,Otimo | Algoritmo. Por padrão: Todos | Não, mas executa todos |
|- v ou --verbose | - | Algoritmo. Por padrão: Todos | Não |
|- l ou --log | nome | Salva a saída em resultados/"nome".log. Indicar somente o nome. | Não |

### Exemplos:
./simulador -f entradas/sequencia1.txt -q 6

./simulador -v -f entradas/sequencia2.txt -q 7 -a LRU,FIFO

./simulador -v -f entradas/sequencia2.txt --qmin 2 --qmax 4 -a LRU,FIFO

./simulador -f entradas/sequencia3.txt -p 50

./simulador -v --arquivo entradas/Belady.txt -q 1 -Q 8 -a ToDos --log teste

./simulador -v --arquivo entradas/Belady.txt -q 1 -Q 8 -a ToDos

./simulador -v -f entradas/Belady.txt -q 1 -Q 8 -a TodoS -l historico

./simulador -m -q 3 -a SegundaChance


## Saída

Depois de executar confira as saídas em resultados/

resultados.csv

*.log

graficos/*.png
