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
	loadTextures();
	
	window.create(sf::VideoMode(800,600), "Tetris via SFML");
	window.setFramerateLimit(60);
	
	background.setTexture(texmgr.getRef("background"));
	loadFonts();
	loadStyleSheets();
	
	initializeBackgrounds();
	setPieceList();
	setVectors();
	setOrder('n');
	setOrder('c');
	initializePieces();
	drop_decrement = .5;
	drop_rate = CLOCKS_PER_SEC * .35;
	
}

Game::~Game()
{
	while(!this->states.empty()) popState();
}

void Game::run()
{	
	this->window.clear(sf::Color(160,160,160));
	//this->drawPanels();
	//this->drawPreview();
	
	this->clock.restart();
	
	while(this->window.isOpen())
	{
		//float dt = elapsed.asSeconds();
		handleInput();
		/*
		this->current_time = this->clock.restart();
		if (current_time.asSeconds() > this->drop_rate)
		{
			if(this->checkDown()) this->piece.moveDown();
		}
		*/
		if (not this->board.getCanDrop())
		{
			this->board.logPiece(this->piece);
			this->piece.reset();
			int clear = this->board.checkRows();
			if (clear)
			{
				int lines = this->board.clearRows(clear);
				if (this->text.update(lines)) this->levelUp();
				
				this->drawPanels();
			}
			
			this->updatePieces();
			this->drawPreview();
			
			bool lost = checkLose();
			if (!lost) this->board.setCanDrop(true);
		}
		this->draw();
	}
}

int Game::levelUp()
{
	this->drop_rate -= this->drop_decrement;
	//Will eventually be used to change the level background, piece fall speed, etc.
	return 1;
}

void Game::draw()
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
			block.setFillColor(this->board.getMapColor(this->board.getCell(row, col)));
			block.setPosition(this->piece.convertBoardtoPixel(row, col));
			this->window.draw(block);
		}
	}
	//draw the piece
	for (int block=0; block<4; block++) {
		this->window.draw(this->piece.getBlock(block));
	}
	//draw the rectangle that hides the hidden rows
	this->window.draw(this->hidden_rows);
	
	//draw the panels
	drawPanels();
	
	//draw preview pieces
	drawPreview();
	
	this->window.display();
}

void Game::drawPreview()
{
	this->window.draw(this->piece_preview);
	for (int block=0; block<4; block++) {
		this->window.draw(this->preview_one.getBlock(block));
		this->window.draw(this->preview_two.getBlock(block));
		this->window.draw(this->preview_thr.getBlock(block));
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
	
	this->left_panel.setFillColor(sf::Color(161, 212, 144));
	this->left_panel.setPosition(sf::Vector2f(64, 75));
	this->left_panel.setSize(sf::Vector2f(200, BOARD_HEIGHT * BLOCK_SIZE));
	
	this->right_panel.setFillColor(sf::Color(195, 144, 212));
	this->right_panel.setPosition(sf::Vector2f(600, 75));
	this->right_panel.setSize(sf::Vector2f(175, BOARD_HEIGHT * BLOCK_SIZE));
	
	this->piece_preview.setFillColor(sf::Color::Black);
	this->piece_preview.setPosition(sf::Vector2f(575,110));
	this->piece_preview.setSize(sf::Vector2f(175, 400));
}



void Game::handleInput()
{
	sf::Event event;
	
	while(this->window.pollEvent(event))
	{	
		switch(event.type)
		{
			case sf::Event::Closed:
			{
				this->window.close();
				break;
			}	
			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::Escape) {
					this->window.close();
				} else if (event.key.code == sf::Keyboard::P) {
					this->board.printBoard();
				} else if (event.key.code == sf::Keyboard::Down) {
					if (checkDown()) {
						this->piece.moveDown();
					}
				} else if (event.key.code == sf::Keyboard::Left) {
					if (checkLeft()) {
						this->piece.moveLeft();
					}
				} else if (event.key.code == sf::Keyboard::Right) {
					if (checkRight()) {
						this->piece.moveRight();
					}
				} else if (event.key.code == sf::Keyboard::Space) {
					if (checkDown()) {
						this->piece.rotateLeft();
						int check = edgeCheck();
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
							check == edgeCheck();
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
}

void Game::dropPiece(Board* board, Piece* piece)
{
	while (board->getCanDrop()) {
		if (this->checkDown()) {
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
	this->piece_index = 0;
	this->piece.setType(this->piece_list[getPieceIndex(this->piece_index, 'c')].getType());
	this->preview_one.setType(this->piece_list[getPieceIndex(this->piece_index + 1, 'c')].getType(), 1);
	this->preview_two.setType(this->piece_list[getPieceIndex(this->piece_index + 2, 'c')].getType(), 2);
	this->preview_thr.setType(this->piece_list[getPieceIndex(this->piece_index + 3, 'c')].getType(), 3);
}

void Game::loadTextures()
{
	this->texmgr.loadTexture("background", "media/background.png");
}

void Game::loadFonts()
{
	sf::Font font;
	font.loadFromFile("media/font.ttf");
	this->fonts["main_font"] = font;
	
	return;
}

void Game::loadStyleSheets()
{
	this->stylesheets["button"] = GuiStyle(&this->fonts.at("main_font"), 1,
		sf::Color(0xc6,0xc6,0xc6), sf::Color(0x94,0x94,0x94),
		sf::Color(0x00,0x00,0x00), sf::Color(0x61,0x61,0x61),
		sf::Color(0x94,0x94,0x94), sf::Color(0x00,0x00,0x00));
	this->stylesheets["text"] = GuiStyle(&this->fonts.at("main_font"), 0,
		sf::Color(0x00,0x00,0x00,0x00), sf::Color(0x00,0x00,0x00),
		sf::Color(0xff,0xff,0xff), sf::Color(0x00,0x00,0x00,0x00),
		sf::Color(0x00,0x00,0x00), sf::Color(0xff,0x00,0x00));
		
	return;
}

void Game::updatePieces()
{
	this->piece_index += 1;
	if (this->piece_index == 4)
	{
		this->preview_one.setType(this->piece_list[
			getPieceIndex(5, 'c')].getType(), 1);
		this->preview_two.setType(this->piece_list[
			getPieceIndex(6, 'c')].getType(), 2);
		this->preview_thr.setType(this->piece_list[
			getPieceIndex(0, 'n')].getType(), 3);
	} 
	else if (this->piece_index == 5)
	{
		this->preview_one.setType(this->piece_list[
			getPieceIndex(6, 'c')].getType(), 1);
		this->preview_two.setType(this->piece_list[
			getPieceIndex(0, 'n')].getType(), 2);
		this->preview_thr.setType(this->piece_list[
			getPieceIndex(1, 'n')].getType(), 3);
	}
	else if (this->piece_index == 6)
	{
		this->preview_one.setType(this->piece_list[
			getPieceIndex(0, 'n')].getType(), 1);
		this->preview_two.setType(this->piece_list[
			getPieceIndex(1, 'n')].getType(), 2);
		this->preview_thr.setType(this->piece_list[
			getPieceIndex(2, 'n')].getType(), 3);
	}
	else if (this->piece_index == 7)
	{
		this->setOrder('c');
		this->setOrder('n');
		this->piece_index = 0;
		this->preview_one.setType(this->piece_list[
			getPieceIndex(1, 'c')].getType(), 1);
		this->preview_two.setType(this->piece_list[
			getPieceIndex(2, 'c')].getType(), 2);
		this->preview_thr.setType(this->piece_list[
			getPieceIndex(3, 'c')].getType(), 3);
	}
	else
	{
		this->preview_one.setType(this->piece_list[
			getPieceIndex(this->piece_index + 1, 'c')].getType(), 1);
		this->preview_two.setType(this->piece_list[
			getPieceIndex(this->piece_index + 2, 'c')].getType(), 2);
		this->preview_thr.setType(this->piece_list[
			getPieceIndex(this->piece_index + 3, 'c')].getType(), 3);
	}
	
	this->piece.setType(this->piece_list[getPieceIndex(this->piece_index, 'c')].getType());
	
}

bool Game::checkDown()
{
	for (int block = 0; block < 4; block++) {
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

bool Game::checkLeft()
{
	for (int block = 0; block < 4; block++) {
		int col = this->piece.getBlockCol(block) - 1;
		
		if (col < 0) {
			return false;
		} else if (this->board.getCell(this->piece.getBlockRow(block),col) != 'n') {
			return false;
		}
	}
	return true;
}

bool Game::checkRight()
{
	for (int block = 0; block < 4; block++) {
		int col = this->piece.getBlockCol(block) + 1;
		
		if (col == BOARD_WIDTH) {
			return false;
		} else if (this->board.getCell(this->piece.getBlockRow(block), col) != 'n') {
			return false;
		}
	}
	return true;
}

int Game::edgeCheck()
{
	for (int block = 0; block < 4; block++) {
		int col = this->piece.getBlockCol(block);
		if (col < 0) {
			return -1;
		} else if (col >= BOARD_WIDTH) {
			return 1;
		}
	}
}

bool Game::checkLose()
{
	int n_count = BOARD_WIDTH;
	for (int col = 0; col < BOARD_WIDTH; col++) {
		if (this->board.getCell(1, col) != 'n') {
			n_count--;
		}
	}
	
	if (n_count < BOARD_WIDTH) {
		return true;
	} else {
		return false;
	}
}
