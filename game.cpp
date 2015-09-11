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

#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "game.hpp"
#include "board.hpp"
#include "dbg.h"

Game::Game()
{
	this->window.create(sf::VideoMode(800,600), "Tetris via SFML");
	this->window.setFramerateLimit(60);
	this->initializeBackgrounds();
	setVectors();
	setOrder('n');
	setOrder('c');
	this->drop_decrement = .5;
	this->drop_rate = CLOCKS_PER_SEC * .35;
}

int Game::levelUp()
{
	this->drop_rate -= this->drop_decrement;
	//Will eventually be used to change the level background, piece fall speed, etc.
	return 1;
}

void Game::draw(Board* board, Piece* piece)
{
	//clear the board area
	this->window.draw(this->board_background);
	//draw any set blocks
	for (int row=0; row<BOARD_HEIGHT; row++) {
		for (int col=0; col<BOARD_WIDTH; col++) {
			sf::RectangleShape block;
			block.setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
			block.setOutlineThickness(OUTLINE);
			block.setOutlineColor(sf::Color::Black);
			block.setFillColor(board->getMapColor(board->getCell(row, col)));
			block.setPosition(piece->convertBoardtoPixel(row, col));
			this->window.draw(block);
		}
	}
	//draw the piece
	for (int block=0; block<4; block++) {
		this->window.draw(piece->getBlock(block));
	}
	//draw the rectangle that hides the hidden rows
	this->window.draw(this->hidden_rows);
	
	this->window.display();
}

void Game::drawPreview(Piece piece_one, Piece piece_two, Piece piece_three)
{
	this->window.draw(this->piece_preview);
	for (int block=0; block<4; block++) {
		this->window.draw(piece_one.getBlock(block));
		this->window.draw(piece_two.getBlock(block));
		this->window.draw(piece_three.getBlock(block));
	}
	
	this->window.display();
}

void Game::drawPanels()
{
	//Left Panel
	this->window.draw(this->left_panel);
	this->window.draw(text.lvl_label);
	this->window.draw(text.lvl_text);
	this->window.draw(text.score_label);
	this->window.draw(text.score_text);
	this->window.draw(text.lc_label);
	this->window.draw(text.lc_text);
	this->window.draw(text.lr_label);
	this->window.draw(text.lr_text);
	
	//Right Panel
	this->window.draw(this->right_panel);
	this->window.draw(text.next_label);
	this->window.draw(this->piece_preview);
}

void Game::initializeBackgrounds()
{
	
	this->board_background.setFillColor(sf::Color::Black);
	this->board_background.setPosition(sf::Vector2f(X_OFFSET, Y_OFFSET));
	this->board_background.setSize(sf::Vector2f((BOARD_WIDTH * BLOCK_SIZE), (BOARD_HEIGHT * BLOCK_SIZE)));

	this->hidden_rows.setFillColor(sf::Color(160, 160, 160));
	this->hidden_rows.setPosition(sf::Vector2f(X_OFFSET, Y_OFFSET));
	this->hidden_rows.setSize(sf::Vector2f((BOARD_WIDTH * BLOCK_SIZE), (BLANK_ROWS * BLOCK_SIZE)));
	
	this->left_panel.setFillColor(sf::Color(160, 160, 160));
	this->left_panel.setPosition(sf::Vector2f(100, 75));
	this->left_panel.setSize(sf::Vector2f(200, BOARD_HEIGHT * BLOCK_SIZE));
	
	this->right_panel.setFillColor(sf::Color(160, 160, 160));
	this->right_panel.setPosition(sf::Vector2f(600, 75));
	this->right_panel.setSize(sf::Vector2f(200, BOARD_HEIGHT * BLOCK_SIZE));
	
	this->piece_preview.setFillColor(sf::Color::Black);
	this->piece_preview.setPosition(sf::Vector2f(575,110));
	this->piece_preview.setSize(sf::Vector2f(175, 400));
	
	this->level_panel.setFillColor(sf::Color::Green);
	this->level_panel.setPosition(sf::Vector2f(X_OFFSET, 0));
	this->level_panel.setSize(sf::Vector2f(300, 150));
}

bool Game::checkDown(Board* board, Piece* piece)
{
	for (int block=0; block<4; block++) {
		int row = piece->getBlockRow(block) + 1;
		
		if (row == BOARD_HEIGHT) {
			board->setCanDrop(false);
			return false;
		} else if (board->getCell(row, piece->getBlockCol(block)) != 'n') {
			board->setCanDrop(false);
			return false;
		}
	}
	return true;
}

bool Game::checkLeft(Board* board, Piece* piece)
{
	for (int block=0; block<4; block++) {
		int col = piece->getBlockCol(block) - 1;
		
		if (col < 0) {
			return false;
		}else if (board->getCell(piece->getBlockRow(block), col) != 'n') {
			return false;
		}
	}
	return true;
}

bool Game::checkRight(Board* board, Piece* piece)
{
	for (int block=0; block<4; block++) {
		int col = piece->getBlockCol(block) + 1;
		
		if (col == BOARD_WIDTH) {
			return false;
		}else if (board->getCell(piece->getBlockRow(block), col) != 'n') {
			return false;
		}
	}
	return true;
}

int Game::edgeCheck(Piece* piece)
{
	for (int block=0; block<4; block++) {
		int col = piece->getBlockCol(block);
		if (col < 0) {
			return -1;
		} else if (col >= BOARD_WIDTH) {
			return 1;
		}
	}
}

bool Game::checkLose(Board* board)
{
	int n_count = BOARD_WIDTH;
	for (int col=0; col<BOARD_WIDTH; col++) {
		if (board->getCell(1, col) != 'n') {
			n_count--;
		}
	}
	
	if (n_count < BOARD_WIDTH) {
		return true;
	} else {
		return false;
	}
}

void Game::handleInput(Board* board, Piece* piece)
{
	sf::Event event;
	
	while (this->window.pollEvent(event)) {
		switch(event.type)
		{
			case sf::Event::Closed:
				this->window.close();
				break;
				
			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::Escape) {
					this->window.close();
				} else if (event.key.code == sf::Keyboard::P) {
					board->printBoard();
				} else if (event.key.code == sf::Keyboard::Down) {
					if (checkDown(board, piece)) {
						piece->moveDown();
					}
				} else if (event.key.code == sf::Keyboard::Left) {
					if (checkLeft(board, piece)) {
						piece->moveLeft();
					}
				} else if (event.key.code == sf::Keyboard::Right) {
					if (checkRight(board, piece)) {
						piece->moveRight();
					}
				} else if (event.key.code == sf::Keyboard::Space) {
					if (checkDown(board, piece)) {
						piece->rotateLeft();
						int check = edgeCheck(piece);
						switch(check) {
							case -1:
								piece->moveRight();
								break;
							case 1:
								piece->moveLeft();
								break;
						}
						//Double check for I piece.  I imagine a better
						//way to handle this exists.
						if (piece->getType() == 'I') {
							check == edgeCheck(piece);
							switch(check) {
								case -1:
									piece->moveRight();
									break;
								case 1:
									piece->moveLeft();
									break;
							}
						}
					}
				}
				break;
			}
				
			default: break;
		}
	}
	
	return;
}

void Game::dropPiece(Board* board, Piece* piece)
{
	while (board->getCanDrop()) {
		if (this->checkDown(board, piece)) {
			piece->moveDown();
		}
	}
}

void Game::setVectors()
{
	for (int i=0; i<7; i++) {
		this->current_order.push_back(i);
	}
	
	for (int i=0; i<7; i++) {
		this->next_order.push_back(i);
	}
}

void Game::setOrder(char order)
{
	switch(order)
	{
		case 'c':
			for (int i=0; i<7; i++) {
				this->current_order[i] = this->next_order[i];
			}
			break;
			
		case 'n':
			std::srand(unsigned (std::time(0)));
			std::random_shuffle (this->next_order.begin(), this->next_order.end());
			break;
	}
}

int Game::getPieceIndex(int index, char vector)
{
	if (vector == 'c') {
		return this->current_order[index];
	} else if (vector == 'n') {
		return this->next_order[index];
	}
}
