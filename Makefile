all: simulador gerador

simulador:
	gcc -Wall -Wextra src/*.c -o simulador

gerador: 
	gcc -Wall -Wextra geradores/*.c -o gerador

clean:
	rm -f simulador gerador

.PHONY: all clean simulador gerador
