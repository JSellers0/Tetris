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

#include "game.hpp"
#include "game_state_menu.hpp"

int main()
{
	Game game;

	game.pushState(new GameStateMenu(&game));
	game.gameloop();
	
	return 0;
	
}
