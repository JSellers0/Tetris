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

#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include <ctime>

#include "piece.hpp"
#include "text.hpp"

class Board;

class Game
{
	private:
		void initializeBackgrounds();
		std::vector<int> current_order;
		std::vector<int> next_order;
		void setVectors();
		
		int drop_rate;
		int drop_decrement;
		
		clock_t start_time;
		clock_t current_time;
		
		sf::RectangleShape board_background;
		sf::RectangleShape left_panel;
		sf::RectangleShape right_panel;
		sf::RectangleShape piece_preview;
		sf::RectangleShape hidden_rows;
		sf::RectangleShape level_panel;
	
	public:
		Game();
		
		Text text;
		
		sf::RenderWindow window;
			
		bool checkDown(Board* board, Piece* piece);
		bool checkLeft(Board* board, Piece *piece);
		bool checkRight(Board* board, Piece *piece);
		int edgeCheck(Piece *piece);
		bool checkLose(Board* board);
		
		
		void draw(Board* board, Piece* piece);
		void drawPreview(Piece piece_one, Piece piece_two, Piece piece_three);
		void drawPanels();
		void update(int);
		void handleInput(Board* board, Piece* piece);
		void gameloop(Board* board, Piece* piece);
		void dropPiece(Board* board, Piece* piece);
		
		int levelUp();
		
		void setOrder(char order);
		int getPieceIndex(int index, char vector);
		
		int getDropRate() {return drop_rate;}
};
#endif

