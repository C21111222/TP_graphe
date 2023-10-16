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

run: prog
	./prog $(ARG1) $(ARG2)
	dot -Txlib exemple2.dot 2>/dev/null

# Extrait les arguments de la ligne de commande et les stocke dans des variables
ifeq ($(filter RUN,$(firstword $(MAKECMDGOALS))),RUN)
    ARG1 := $(word 2,$(MAKECMDGOALS))
    ARG2 := $(word 3,$(MAKECMDGOALS))
    ARGS := $(ARG1) $(ARG2)
endif

$(eval $(ARGS):;@:)
