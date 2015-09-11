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

