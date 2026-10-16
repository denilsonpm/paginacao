CC = gcc
CFLAGS = -Wall -Wextra -MMD -MP

# provisorio dps substituir para SRC = $(wildcard src/*.c)
SRC = src/main.c src/leitor.c src/quadros.c src/traco.c src/fifo.c src/lru.c src/otimo.c
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)
EXEC = simulador

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(DEP) $(EXEC)

-include $(DEP)

.PHONY: all clean
