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
	Piece preview_three;
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
	
	piece = &piece_list[game.getPieceIndex(piece_index, 'c')];
	preview_one.setType(piece_list[game.getPieceIndex(piece_index + 1, 'c')].getType(), 1);
	preview_two.setType(piece_list[game.getPieceIndex(piece_index + 2, 'c')].getType(), 2);
	preview_three.setType(piece_list[game.getPieceIndex(piece_index + 3, 'c')].getType(), 3);
	
	game.window.clear(sf::Color(160, 160, 160));
	game.drawPanels();
	game.drawPreview(preview_one, preview_two, preview_three);
	
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
						
			switch(piece_index)
			{
				case 4:
					preview_one.setType(piece_list[game.getPieceIndex(piece_index + 1, 'c')].getType(), 1);
					preview_two.setType(piece_list[game.getPieceIndex(piece_index + 2, 'c')].getType(), 2);
					preview_three.setType(piece_list[game.getPieceIndex(0, 'n')].getType(), 3);
					break;
					
				case 5:
					preview_one.setType(piece_list[game.getPieceIndex(piece_index + 1, 'c')].getType(), 1);
					preview_two.setType(piece_list[game.getPieceIndex(0, 'n')].getType(), 2);
					preview_three.setType(piece_list[game.getPieceIndex(1,'n')].getType(),3);
					break;
					
				case 6:
					preview_one.setType(piece_list[game.getPieceIndex(0, 'n')].getType(), 1);
					preview_two.setType(piece_list[game.getPieceIndex(1, 'n')].getType(), 2);
					preview_three.setType(piece_list[game.getPieceIndex(2,'n')].getType(),3);
					break;
				
				case 7:
					game.setOrder('c');
					game.setOrder('n');
					piece_index = 0;
					preview_one.setType(piece_list[game.getPieceIndex(piece_index + 1, 'c')].getType(), 1);
					preview_two.setType(piece_list[game.getPieceIndex(piece_index + 2, 'c')].getType(), 2);
					preview_three.setType(piece_list[game.getPieceIndex(piece_index + 3,'c')].getType(),3);
					break;
					
				default:
					preview_one.setType(piece_list[game.getPieceIndex(piece_index + 1, 'c')].getType(), 1);
					preview_two.setType(piece_list[game.getPieceIndex(piece_index + 2, 'c')].getType(), 2);
					preview_three.setType(piece_list[game.getPieceIndex(piece_index + 3,'c')].getType(),3);
					break;
			}
			
			piece = &piece_list[game.getPieceIndex(piece_index, 'c')];
			game.drawPreview(preview_one, preview_two, preview_three);
			
			bool lost = game.checkLose(board);
			if (!lost) {
				board->setCanDrop(true);
			}
		}
			
		game.draw(board, piece);
	}
}

