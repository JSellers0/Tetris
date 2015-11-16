/** 
 * tetris.cpp
 * Copyright 2015 Jeffrey L. Sellers
 * 
 * @author Jeffrey L. Sellers
 * @version 0.9
 * @copyright 2015
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
#include "dbg.h"

#include "game.hpp"
#include "game_state_menu.hpp"

int main()
{
	/* Debugging, run the old game */
	//old();
	
	Game game;
	game.window.setTitle("Tetris Main Menu");
	game.pushState(new GameStateMenu(&game));
	//game.run();
	
	return 0;
	
}

/*
void old()
{
	
	clock_t start_time = clock();
	clock_t current_time;
		
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

*/
