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

//This file uses code from Copyright (c) 2014 Daniel Mansfield under the MIT License (MIT)

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <SFML/System.hpp>


#include "game.hpp"
#include "game_state.hpp"
#include "texture_manager.hpp"

#include "board.hpp"
#include "piece.hpp"
#include "text.hpp"
#include "dbg.h"

Game::Game()
{
	this->loadTextures();
	
	this->window.create(sf::VideoMode(800,600), "Tetris via SFML");
	this->window.setFramerateLimit(60);
	
	this->background.setTexture(this->texmgr.getRef("background"));
	
	initializeBackgrounds();
	setPieceList();
	setVectors();
	setOrder('n');
	setOrder('c');
	this->drop_decrement = .5;
	this->drop_rate = CLOCKS_PER_SEC * .35;
	
}

Game::~Game()
{
	while(!this->states.empty()) popState();
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
}

bool Game::newCheckDown()
{
	for (int block=0; block<4; block++) {
		int row = this->piece.getBlockRow(block) + 1;
		
		if (row == BOARD_HEIGHT) {
			this->board.setCanDrop(false);
			return false;
		} else if (this->board.getCell(row, this->piece.getBlockCol(block)) != 'n') {
			this->board.setCanDrop(false);
			return false;
		}
	}
	return true;
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

bool Game::newCheckLeft()
{
	for (int block=0; block<4; block++) {
		int col = this->piece.getBlockCol(block) - 1;
		
		if (col < 0) {
			return false;
		} else if (this->board.getCell(this->piece.getBlockRow(block),col) != 'n') {
			return false;
		}
	}
	return true;
}

bool Game::newCheckRight()
{
	for (int block=0; block<4; block++) {
		int col = this->piece.getBlockCol(block) + 1;
		
		if (col == BOARD_WIDTH) {
			return false;
		} else if (this->board.getCell(this->piece.getBlockRow(block), col) != 'n') {
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

int Game::newEdgeCheck()
{
	for (int block=0; block<4; block++) {
		int col = this->piece.getBlockCol(block);
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

void Game::handleInput(sf::Event event)
{
	switch(event.type)
	{
		case sf::Event::Closed:
			this->text.window.close();
			break;
			
		case sf::Event::KeyPressed:
		{
			if (event.key.code == sf::Keyboard::Escape) {
				this->text.window.close();
			} else if (event.key.code == sf::Keyboard::P) {
				this->board.printBoard();
			} else if (event.key.code == sf::Keyboard::Down) {
				if (newCheckDown()) {
					this->piece.moveDown();
				}
			} else if (event.key.code == sf::Keyboard::Left) {
				if (newCheckLeft()) {
					this->piece.moveLeft();
				}
			} else if (event.key.code == sf::Keyboard::Right) {
				if (newCheckRight()) {
					this->piece.moveRight();
				}
			} else if (event.key.code == sf::Keyboard::Space) {
				if (newCheckDown()) {
					this->piece.rotateLeft();
					int check = newEdgeCheck();
					switch(check) {
						case -1:
							this->piece.moveRight();
							break;
						case 1:
							this->piece.moveLeft();
							break;
					}
					//Double check for I piece.  I imagine a better
					//way to handle this exists.
					if (this->piece.getType() == 'I') {
						check == newEdgeCheck();
						switch(check) {
							case -1:
								this->piece.moveRight();
								break;
							case 1:
								this->piece.moveLeft();
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

void Game::setPieceList()
{
	this->piece_list[0].setType('Z');
	this->piece_list[1].setType('S');
	this->piece_list[2].setType('J');
	this->piece_list[3].setType('L');
	this->piece_list[4].setType('T');
	this->piece_list[5].setType('O');
	this->piece_list[6].setType('I');
}

void Game::run()
{
	initializePieces();
	
	sf::Clock clock;
	
	this->text.window.clear(sf::Color(160, 160, 160));
	this->text.window.display();	
	
	while(this->text.window.isOpen())
	{
		sf::Time elapsed = clock.restart();
		float dt = elapsed.asSeconds();
		sf::Event event;
		while(this->text.window.pollEvent(event))
		{
			handleInput(event);
		}
	}
}

void Game::pushState(GameState* state)
{
	this->states.push(state);
	
	return;
}

void Game::popState()
{
	delete this->states.top();
	this->states.pop();
	
	return;
}

GameState* Game::peekState()
{
	if(this->states.empty()) return nullptr;
	return this->states.top();
}

void Game::initializePieces()
{
	this->piece.setType(this->piece_list[getPieceIndex(this->piece_index, 'c')].getType());
	this->preview_one.setType(this->piece_list[getPieceIndex(this->piece_index + 1, 'c')].getType(), 1);
	this->preview_two.setType(this->piece_list[getPieceIndex(this->piece_index + 2, 'c')].getType(), 2);
	this->preview_thr.setType(this->piece_list[getPieceIndex(this->piece_index + 3, 'c')].getType(), 3);
}

void Game::loadTextures()
{
	this->texmgr.loadTexture("background", "media/background.png");
}
