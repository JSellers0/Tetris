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
	std::vector<int> piece_order;
	void setVector();
	
	int drop_rate;
	int drop_decrement;
	
	clock_t start_time;
	clock_t current_time;
	
	public:
	Game();
	
	Text text;
	
	sf::RenderWindow window;
	sf::RectangleShape board_background;
	sf::RectangleShape left_panel;
	sf::RectangleShape right_panel;
	sf::RectangleShape piece_preview;
	sf::RectangleShape hidden_rows;
	sf::RectangleShape level_panel;
	
	bool checkDown(Board* board, Piece* piece);
	bool checkLeft(Board* board, Piece *piece);
	bool checkRight(Board* board, Piece *piece);
	int edgeCheck(Piece *piece);
	bool checkLose(Board* board);
	
	
	void draw(Board* board, Piece* piece);
	void drawPanels(Piece );
	void update(int);
	void handleInput(Board* board, Piece* piece);
	void gameloop(Board* board, Piece* piece);
	void dropPiece(Board* board, Piece* piece);
	
	int levelUp();
	
	void setOrder();
	int getPieceIndex(int index);
	
	int getDropRate() {return drop_rate;}
};
#endif

