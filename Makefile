CC=gcc

CFLAGS=-c -Wall

all: bantumi

bantumi: game.o
    $(CC) game.o -o bantumi

game.o: game.c
    $(CC) $(CFLAGS) game.c

clean: 
    rm *o game
