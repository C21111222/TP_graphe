# Ce makefile compile un programme C simple

# Définition du compilateur et des options
CC = gcc
CFLAGS = -Wall -Wextra -pedantic

# Définition des cibles
all: prog

prog: TP2.o
	$(CC) $(CFLAGS) -o prog TP2.o -lm

TP2.o: TP2.c
	$(CC) $(CFLAGS) -c TP2.c

# Règle pour nettoyer le projet
clean:
	rm -f prog TP2.o

display: prog
	dot -Txlib exemple2.dot 2>/dev/null

doc:
	doxygen doxyfile