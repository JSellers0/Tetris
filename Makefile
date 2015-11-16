# 	 Copyright 2015 Jeffrey L. Sellers
#
# 	 This file is part of Jeff's C++ tetris clone.
#
# 	 Jeff's C++ tetris clone is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Jeff's C++ tetris clone is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with Jeff's C++ tetris clone.  If not, see <http://www.gnu.org/licenses/>.
# 

PREFIX?=/usr/local
CXX = g++
CXXFLAGS=-g -Wall -std=c++11 -I${PREFIX}/include/SFML/
LDFLAGS=-L${PREFIX}/lib -lsfml-graphics -lsfml-window -lsfml-system

all: tetris

tetris: piece.o board.o game.o text.o game_state_menu.o game_state_main.o texture_manager.o game_state_pause.o gui.o

piece: piece.o

board: board.o

game: game.o

text: text.o

game_state_menu: game_state_menu.o

game_state_main: game_state_main.o

game_state_pause: game_state_pause.0

texture_manager: texture_manager.o

gui: gui.o



clean:
	rm -f *.o
	rm -f tetris

backup:
	cp Makefile bak
	cp *.* bak
