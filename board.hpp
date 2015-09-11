/* 
 * Copyright 2015 Jeffrey L. Sellers
*/

/* This file is part of Jeff's C++ tetris clone.

    Jeff's C++ tetris clone is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Jeff's C++ tetris clone is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Jeff's C++ tetris clone.  If not, see <http://www.gnu.org/licenses/>.
*/ 

#ifndef BOARD_HPP
#define BOARD_HPP

#include <map>
#include <SFML/Graphics.hpp>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 22
#define BLANK_ROWS 2

class Piece;

class Board
{
	private:
		std::map <char, sf::Color> color_map;
		void setColorMap();
		char board[BOARD_HEIGHT][BOARD_WIDTH];
		void setBoard();
		bool can_drop;
	
	public:
		Board();
		void setCell(int row, int col, char color_code);
		char getCell(int row, int col);
		sf::Color getMapColor(char color_key);
		void logPiece(Piece* piece);
		bool getCanDrop() {return can_drop;};
		void setCanDrop(bool droppable);
		void printBoard();
		bool checkRow(int row);
		
		int checkRows();
		int clearRows(int row);
};

#endif


