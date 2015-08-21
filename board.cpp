#include <iostream>

#include "board.hpp"
#include "piece.hpp"
#include "dbg.h"

Board::Board()
{
	this->setColorMap();
	this->setBoard();
	this->can_drop = true;
}

void Board::setColorMap()
{
	color_map['r'] = sf::Color(255, 000, 000);
    color_map['g'] = sf::Color(000, 255, 000);
    color_map['o'] = sf::Color(255, 128, 000);
    color_map['b'] = sf::Color(000, 000, 255);
    color_map['p'] = sf::Color(160, 032, 240);
    color_map['y'] = sf::Color(255, 255, 000);
    color_map['c'] = sf::Color(000, 255, 255);
}

void Board::setBoard()
{
	// log_info("Initializing Board");
    for (int row=0; row<BOARD_HEIGHT; row++)
    {
        for (int col=0; col<BOARD_WIDTH; col++)
        {
            board[row][col] = 'n';
        }
    }
}

void Board::setCell(int row, int col, char color_code)
{
	board[row][col] = tolower(color_code);
}

char Board::getCell(int row, int col)
{
	return board[row][col];
}

sf::Color Board::getMapColor(char color_key)
{
	return color_map[color_key];
}

void Board::logPiece(Piece *piece)
{
	for(int block=0; block<4; block++) {
		int row = piece->getBlockRow(block);
		int col = piece->getBlockCol(block);
		this->board[row][col] = tolower(piece->getColorCode());
	}
}

void Board::setCanDrop(bool droppable)
{
	this->can_drop = droppable;
}

void Board::printBoard()
{
    for (int row=0; row<BOARD_HEIGHT; row++)
    {
		if (row < 10) {
			std::cout << 0;
		}
		std::cout << row << " ";
        for (int col=0; col<BOARD_WIDTH; col++)
        {
            std::cout << this->getCell(row, col) <<" ";
        }

        std::cout << "\n";
    }

    std::cout << "~~~~~~~~~~~~~~~~~~~~\n";
    std::cout << "~~~~~~~~~~~~~~~~~~~~\n";
}

int Board::checkRows()
{
	int n_count;
	for (int row = BOARD_HEIGHT -1; row > -1; row--) {
		n_count = BOARD_WIDTH;
		for (int col=0; col<BOARD_WIDTH; col++) {
			if (this->board[row][col] != 'n') {
				n_count--;
			}
		}
		
		if (!n_count) {
			return row;
		}
	}
	return 0;
}

bool Board::checkRow(int row)
{
	int n_count = BOARD_WIDTH;
	for (int col=0; col<BOARD_WIDTH; col++) {
		if (this->board[row][col] != 'n') {
			n_count--;
		}
	}
	if (!n_count) {
		return true;
	} else {
		return false;
	}
}

int Board::clearRows(int row)
{
	int total_rows = 0;
	
	for (int check=row; check>(row-4); check--) {
		if (checkRow(check)) {
		
			total_rows++;
		}
	}
	
	// log_info("And it should clear %d total rows", total_rows);
	
	for (int clear=row; clear>(-1+total_rows); clear--) {
		int copy = (clear - total_rows);
		for (int col=0; col<BOARD_WIDTH; col++) {
			this->board[clear][col] = this->board[copy][col];
		}
	}
	
	return total_rows;
}


