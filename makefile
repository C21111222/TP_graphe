# Ce makefile compile un programme C simple

# Définition du compilateur et des options
CC = gcc
CFLAGS = -Wall -Wextra -pedantic

# Définition des cibles
all: prog

prog: main.o
	$(CC) $(CFLAGS) -o prog main.o -lm

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

# Règle pour nettoyer le projet
clean:
	rm -f prog main.o

display: prog
	dot -Txlib exemple2.dot 2>/dev/null

doc:
	doxygen doxyfile