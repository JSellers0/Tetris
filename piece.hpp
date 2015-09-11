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

#ifndef PIECE_HPP
#define PIECE_HPP

#include <SFML/Graphics.hpp>

#define BLOCK_SIZE 25
#define OUTLINE -1

#define X_OFFSET 275
#define Y_OFFSET 25

class Piece
{
	private:
	
		sf::RectangleShape structure[4];
		int origin_col;
		int origin_col_start;
		int origin_row;
		int origin_row_start;
		sf::Color color;
		char color_code;
		char type;
		
		void init(sf::Color color, char color_code, int row, int col, char type);
		void setStructure(sf::Vector2f, sf::Vector2f, sf::Vector2f, sf::Vector2f);
		
		sf::Vector2f block_1, block_2, block_3, block_4;
	
	public:
		void setType(char type, int preview_number=0);
		
		int getBlockCol(int block);
		int getBlockRow(int block);
		
		sf::Color getColor();
		char getColorCode() {return color_code;}
		char getType() {return type;}
		
		void moveDown();
		void moveLeft();
		void moveRight();
		void rotateLeft();
		void rotateRight();
		
		sf::Vector2f convertBoardtoPixel(int row, int column);
		
		sf::RectangleShape getBlock(int block) {return structure[block];}
		
		void reset();
};
	
#endif

