CC=gcc

CFLAGS=-std=c99 -Wall

all: bantumi

bantumi: 
	$(CC) $(CFLAGS) main.c game.c ui.c strategy.c -o bantumi

clean: 
	rm bantumi 
