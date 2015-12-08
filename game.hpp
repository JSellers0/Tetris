/** 
 * game.hpp
 * Copyright 2015 Jeffrey L. Sellers
 */

/* This file is part of Jeff's C++ tetris clone.
 *
 *  Jeff's C++ tetris clone is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Jeff's C++ tetris clone is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Jeff's C++ tetris clone.  If not, see <http://www.gnu.org/licenses/>.
 */ 
 
//This file uses code from Copyright (c) 2014 Daniel Mansfield under the MIT License (MIT)

#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <ctime>
#include <stack>

#include "texture_manager.hpp"

#include "text.hpp"
#include "piece.hpp"
#include "board.hpp"
#include "gui.hpp"

class GameState;

class Game
{
	// Private Methods
	private:
		/** Initializes the following for drawing the main game screen:
		  * board_background
		  * hidden_rows
		  * left_panel
		  * right_panel
		  * piece_preview
		  */
		void initializeBackgrounds();
		
		/** Initializes the vectors current_order and next_order to contain
		  * 7 integers from 0 to 6.
		  */
		  
		void initializePieces();
		void setVectors();
		
		void setPieceList();
		
		void loadTextures();
		void loadStyleSheets();
		void loadFonts();
		
		void updatePieces();
		
	// Private Members
	private:	
	
		std::vector<int> current_order;
		std::vector<int> next_order;
		
		float drop_rate;
		int drop_decrement;
		int piece_index;
		
		sf::Clock clock;
		sf::Time elapsed;
		float dt;
		clock_t start_time;
		clock_t current_time;
		
		sf::RectangleShape board_background;
		sf::RectangleShape left_panel;
		//This should be eliminated; only the piece preview rect needs
		//to be redrawn each piece change.
		sf::RectangleShape right_panel;
		sf::RectangleShape piece_preview;
		sf::RectangleShape hidden_rows;
		
		Piece piece_list[7];
		
		Board board;
		Piece piece;
		Piece preview_one;
		Piece preview_two;
		Piece preview_thr;
	
	public:
		// Public Constructors
		Game();
		~Game();
		 
		//Public Members
		Text text;
		std::stack<GameState*> states;
		
		sf::RenderWindow window;
		TextureManager texmgr;
		sf::Sprite background;
		
		std::map<std::string, GuiStyle> stylesheets;
		std::map<std::string, sf::Font> fonts;
		
		//Public Methods
		
		/* GameState Changes */
		void pushState(GameState* state);
		void popState();
		void changeState(GameState* state);
		GameState* peekState();
		
		/** Returns the availability of space below the piece.  Currently
		  * uses pointer parameters because of not having game.run() set
		  * up.
		  * 
		  * @param *board The address of a board
		  * @param *piece The address of a piece
		  * @return true if there is space available below the piece otherwise false
		  */ 
		bool checkDown();
		
		/** Returns the availability of space to the left of the piece.  
		  * Currently uses pointer parameters because of not having 
		  * game.run() set up.
		  * 
		  * @param *board The address of a board
		  * @param *piece The address of a piece
		  * @return true if there is space available left of the piece otherwise false
		  */
		bool checkLeft();
		
		/** Returns the availability of space to the right of the piece.  
		  * Currently uses pointer parameters because of not having 
		  * game.run() set up.
		  * 
		  * @param *board The address of a board
		  * @param *piece The address of a piece
		  * @return true if there is space available right of the piece otherwise false
		  */
		bool checkRight();
		
		/** Checks for edge overlap following piece rotation.
		  * Currently uses pointer parameters because of not having 
		  * game.run() set up.
		  * 
		  * @param *piece The address of a piece
		  * @return -1 if the piece is overlapping the left edge
		  * @return 1 if the piece is overlapping the right edge
		  */
		int edgeCheck();
		
		/** Checks the top row for pieces after tetris.cpp gets a new piece.
		  * This probably creates a bug wherein you can stack pieces all the
		  * way on the left or right and cause game loss even though there
		  * is space for a new piece to drop.  Will need to revise.
		  * 
		  * @param *board the address of a board
		  * @return true if there is a piece in the top row already
		  */
		bool checkLose();
		
		void draw();
		void drawPreview();
		void drawPanels();
		void update(int);
		void handleInput();
		void dropPiece(Board* board, Piece* piece);
		
		int levelUp();
		
		void setOrder(char order);
		int getPieceIndex(int index, char vector);
		
		int getDropRate() {return drop_rate;}
		
		void run();
		
		
		
};
#endif

