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
	void setType(char type);
	
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

