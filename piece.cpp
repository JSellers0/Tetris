#include "piece.hpp"
#include "dbg.h"

void Piece::setType(char type)
{	
	switch(type)
	{
		case 'Z':
			block_1 = sf::Vector2f(convertBoardtoPixel(0, 3));
			block_2 = sf::Vector2f(convertBoardtoPixel(0, 4));
			block_3 = sf::Vector2f(convertBoardtoPixel(1, 4));
			block_4 = sf::Vector2f(convertBoardtoPixel(1, 5));
			
			init(sf::Color(255,0,0), 'R', 4, 1, type);
			setStructure(block_1, block_2, block_3, block_4);
			break;
		
		case 'S':
			block_1 = sf::Vector2f(convertBoardtoPixel(0, 4));
			block_2 = sf::Vector2f(convertBoardtoPixel(0, 5));
			block_3 = sf::Vector2f(convertBoardtoPixel(1, 3));
			block_4 = sf::Vector2f(convertBoardtoPixel(1, 4));
			
			init(sf::Color(0,255,0), 'G', 4, 1, type);
			setStructure(block_1, block_2, block_3, block_4);
			break;
			
		case 'J':
			block_1 = sf::Vector2f(convertBoardtoPixel(0, 3));
			block_2 = sf::Vector2f(convertBoardtoPixel(1, 3));
			block_3 = sf::Vector2f(convertBoardtoPixel(1, 4));
			block_4 = sf::Vector2f(convertBoardtoPixel(1, 5));
			
			init(sf::Color(0,0,255), 'B', 4, 1, type);
			setStructure(block_1, block_2, block_3, block_4);
			break;
			
		case 'L':
			block_1 = sf::Vector2f(convertBoardtoPixel(0, 5));
			block_2 = sf::Vector2f(convertBoardtoPixel(1, 3));
			block_3 = sf::Vector2f(convertBoardtoPixel(1, 4));
			block_4 = sf::Vector2f(convertBoardtoPixel(1, 5));
			
			init(sf::Color(255,128,0), 'O', 4, 1, type);
			setStructure(block_1, block_2, block_3, block_4);
			break;
			
		case 'T':
			block_1 = sf::Vector2f(convertBoardtoPixel(0, 4));
			block_2 = sf::Vector2f(convertBoardtoPixel(1, 3));
			block_3 = sf::Vector2f(convertBoardtoPixel(1, 4));
			block_4 = sf::Vector2f(convertBoardtoPixel(1, 5));
			
			init(sf::Color(160,32,240), 'P', 4, 1, type);
			setStructure(block_1, block_2, block_3, block_4);
			break;
			
		case 'O':
			block_1 = sf::Vector2f(convertBoardtoPixel(0, 4));
			block_2 = sf::Vector2f(convertBoardtoPixel(0, 5));
			block_3 = sf::Vector2f(convertBoardtoPixel(1, 4));
			block_4 = sf::Vector2f(convertBoardtoPixel(1, 5));
			
			init(sf::Color(255,255,0), 'Y', 4, 1, type);
			setStructure(block_1, block_2, block_3, block_4);
			break;
			
		case 'I':
			block_1 = sf::Vector2f(convertBoardtoPixel(1, 3));
			block_2 = sf::Vector2f(convertBoardtoPixel(1, 4));
			block_3 = sf::Vector2f(convertBoardtoPixel(1, 5));
			block_4 = sf::Vector2f(convertBoardtoPixel(1, 6));
			
			init(sf::Color(0,255,255), 'C', 4, 1, type);
			setStructure(block_1, block_2, block_3, block_4);
			break;
	}
}
			
void Piece::init(sf::Color color, char color_code, int origin_col, int origin_row, char type)
{
	this->type = type;
	this->color = color;
	this->color_code = color_code;
	// Probably easier to handle rotation with board system and then convertBoardtoPixel.
	this->origin_col = origin_col;
	this->origin_row = origin_row;
	this->origin_col_start = origin_col;
	this->origin_row_start = origin_row;
	
	for (int block = 0; block < 4; block++) {
		this->structure[block].setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
		this->structure[block].setOutlineThickness(OUTLINE);
		this->structure[block].setOutlineColor(sf::Color::Black);
		this->structure[block].setFillColor(color);
	}
	
}

void Piece::setStructure(sf::Vector2f block_1, sf::Vector2f block_2, 
						sf::Vector2f block_3, sf::Vector2f block_4)
{
	this->structure[0].setPosition(block_1);
	this->structure[1].setPosition(block_2);
	this->structure[2].setPosition(block_3);
	this->structure[3].setPosition(block_4);
}

sf::Vector2f Piece::convertBoardtoPixel(int row, int col)
{
	sf::Vector2f position = sf::Vector2f(col * BLOCK_SIZE + X_OFFSET, row * BLOCK_SIZE + Y_OFFSET);
	return position;
}

int Piece::getBlockCol(int block)
{
	sf::Vector2f pos = this->structure[block].getPosition();
	int x = pos.x;
	return (x - X_OFFSET) / BLOCK_SIZE;
}

int Piece::getBlockRow(int block)
{
	sf::Vector2f pos = this->structure[block].getPosition();
	int y = pos.y;
	return (y - Y_OFFSET) / BLOCK_SIZE;
}

void Piece::moveDown()
{
	for (int block=0; block<4; block++) {
		this->structure[block].move(0,BLOCK_SIZE);
	}
	this->origin_row += 1;
}

void Piece::moveLeft()
{
	for (int block=0; block<4; block++) {
		this->structure[block].move(-BLOCK_SIZE,0);
	}
	this->origin_col -= 1;
}

void Piece::moveRight()
{
	for (int block=0; block<4; block++) {
		this->structure[block].move(BLOCK_SIZE,0);
	}
	this->origin_col += 1;
}

void Piece::rotateLeft()
{
	for (int block=0; block<4; block++) {
		int rotated_row = this->origin_row - this->origin_col + this->getBlockCol(block);
		int rotated_col = this->origin_col + this->origin_row - this->getBlockRow(block);
		
		this->structure[block].setPosition(this->convertBoardtoPixel(rotated_row, rotated_col));
	}
}

void Piece::reset()
{
	this->setStructure(this->block_1, this->block_2, this->block_3, this->block_4);
	this->origin_col = this->origin_col_start;
	this->origin_row = this->origin_row_start;
}

