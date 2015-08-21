#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>

#include "piece.hpp"
#include "board.hpp"
#include "game.hpp"
#include "dbg.h"

using namespace std;

int main()
{
	Piece piece_list[7];
	Piece *piece;
	Piece preview_one;
	Piece preview_two;
	Board pboard;
	Board *board = &pboard;
	Game game;
	
	piece_list[0].setType('Z');
	piece_list[1].setType('S');
	piece_list[2].setType('J');
	piece_list[3].setType('L');
	piece_list[4].setType('T');
	piece_list[5].setType('O');
	piece_list[6].setType('I');
	
	int piece_index = 0;
	
	piece = &piece_list[game.getPieceIndex(piece_index)];
	
	game.window.clear(sf::Color(160, 160, 160));
	game.drawPanels();
	
	clock_t start_time = clock();
	clock_t current_time;
	
	while (game.window.isOpen()) {
		sf::Event event;
		while (game.window.pollEvent(event)) {
			switch(event.type)
			{
				case sf::Event::Closed:
					game.window.close();
					break;
					
				case sf::Event::KeyPressed: {
					if (event.key.code == sf::Keyboard::Escape) {
						game.window.close();
					} else if (event.key.code == sf::Keyboard::P) {
						board->printBoard();
					} else if (event.key.code == sf::Keyboard::Down) {
						if (game.checkDown(board, piece)) {
							piece->moveDown();
						}
					} else if (event.key.code == sf::Keyboard::Left) {
						if (game.checkLeft(board, piece)) {
							piece->moveLeft();
						}
					} else if (event.key.code == sf::Keyboard::Right) {
						if (game.checkRight(board, piece)) {
							piece->moveRight();
						}
					} else if (event.key.code == sf::Keyboard::Space) {
						if (game.checkDown(board, piece)) {
							piece->rotateLeft();
							int check = game.edgeCheck(piece);
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
								check = game.edgeCheck(piece);
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
					} else if (event.key.code == sf::Keyboard::Up) {
						game.dropPiece(board, piece);
					}
					break;
				}
			}
		}
		
		if (clock() - start_time > game.getDropRate()) {
			if (game.checkDown(board, piece)) {
				piece->moveDown();
			}
			start_time = clock();
		}
		
		if (not board->getCanDrop()) {
			board->logPiece(piece);
			piece->reset();
			int clear = board->checkRows();
			if (clear) {
				int lines = board->clearRows(clear);
				if (game.text.update(lines)) {
					game.levelUp();
				}
				game.drawPanels();
			}
			piece_index++;
			if (piece_index == 7) {
				game.setOrder();
				piece_index = 0;
			}
			piece = &piece_list[game.getPieceIndex(piece_index)];
			
			bool lost = game.checkLose(board);
			if (!lost) {
				board->setCanDrop(true);
			}
		}
			
		game.draw(board, piece);
	}
}

