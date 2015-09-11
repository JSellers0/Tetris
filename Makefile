PREFIX?=/usr/local
CFLAGS=-g -Wall -std=gnu++11 -I${PREFIX}/include/SFML/
LDFLAGS=-L${PREFIX}/lib -lsfml-graphics -lsfml-window -lsfml-system

all: tetris

tetris: piece.o board.o game.o text.o

piece: piece.o

board: board.o

game: game.o

text: text.o

clean:
	rm -f *.o
	rm -f tetris

backup:
	cp Makefile bak
	cp *.* bak
