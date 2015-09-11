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


