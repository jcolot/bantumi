CC=gcc

CFLAGS=-std=c99 -Wall

all: bantumi

bantumi: 
	$(CC) game.c ui.c -o bantumi

clean: 
	rm bantumi 
