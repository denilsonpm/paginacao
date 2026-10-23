all:
	gcc -Wall -Wextra src/*.c -o simulador

clean:
	rm -f src/*.c

.PHONY: all clean
