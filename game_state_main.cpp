/* Adapted from https://www.binpress.com/tutorial/creating-a-city-building-game-with-sfml-part-2-the-first-state/124
 * 
 * Copyright (c) 2014 Daniel Mansfield under the MIT License (MIT)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 */ 
 
 #include <SFML/Graphics.hpp>
 
 #include "game_state.hpp"
 #include "game_state_main.hpp"
 #include "game_state_pause.hpp"
 
 void GameStateMain::pauseGame()
 {
	 this->game->pushState(new GameStatePause(this->game));
	 
	 return;
 }
 
 void GameStateMain::draw(const float dt)
 {
	 this->game->window.clear(sf::Color(160,160,160));
	 //clear the board area
	 this->game->window.draw(this->game->board_background);
	 //draw any set blocks
	 for (int row=0; row<BOARD_HEIGHT; row++) {
	 	 for (int col=0; col<BOARD_WIDTH; col++) {
			 sf::RectangleShape block;
			 block.setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
			 block.setOutlineThickness(OUTLINE);
			 block.setOutlineColor(sf::Color::Black);
			 block.setFillColor(this->game->board.getMapColor(this->game->board.getCell(row, col)));
			 block.setPosition(this->game->piece.convertBoardtoPixel(row, col));
			 this->game->window.draw(block);
		 }
	 }
	 //draw the piece
	 for (int block=0; block<4; block++) {
		 this->game->window.draw(this->game->piece.getBlock(block));
	 }
	
	 this->game->window.draw(this->game->hidden_rows);
	
	 //draw the panels
	 this->game->drawPanels();
	
	 //draw preview pieces
	 this->game->drawPreview();
	 return;
}

void GameStateMain::update(const float dt)
{
	if(not this->game->board.getCanDrop())
	{
		this->game->board.logPiece(this->game->piece);
		this->game->piece.reset();
		int clear = this->game->board.checkRows();
		if (clear) {
			int lines = this->game->board.clearRows(clear);
			if (this->game->text.update(lines)) {
				this->game->levelUp();
			}
			this->game->drawPanels();
		}
		
		this->game->updatePieces();
		this->game->drawPreview();
		
		bool lost = this->game->checkLose();
		if (!lost) this->game->board.setCanDrop(true);
	}
	return;
}

void GameStateMain::handleInput()
{
	sf::Event event;
	
	while(this->game->window.pollEvent(event))
	{
		switch(event.type)
		{
			/* Close the window */
			case sf::Event::Closed:
			{
				this->game->window.close();
				break;
			}
			/* Resize the window */
			case sf::Event::Resized:
			{
				gameView.setSize(event.size.width, event.size.height);
				this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0,0)));
				this->game->background.setScale(
					float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
					float(event.size.height)/ float(this->game->background.getTexture()->getSize().y));
				break;
			}
			/* Keyboard Events */
			case sf::Event::KeyPressed:
			{
				if(event.key.code == sf::Keyboard::Escape) {
					this->game->window.close();
				}else if(event.key.code == sf::Keyboard::P) {
					this->pauseGame();
				}else if(event.key.code == sf::Keyboard::Down) {
					if(this->game->checkDown()) {
						this->game->piece.moveDown();
					}
				}else if(event.key.code == sf::Keyboard::Left) {
					if(this->game->checkLeft()) {
						this->game->piece.moveLeft();
					}
				}else if(event.key.code == sf::Keyboard::Right) {
					if(this->game->checkRight()) {
						this->game->piece.moveRight();
					}
				}else if(event.key.code == sf::Keyboard::Space) {
					if(this->game->checkDown()) {
						this->game->piece.rotateLeft();
						int check = this->game->edgeCheck();
						switch(check) {
							case -1:
								this->game->piece.moveRight();
								break;
							case 1:
								this->game->piece.moveLeft();
								break;
						}
						//Double check for I piece.  I imagine a better
						//way to handle this exists.
						if (this->game->piece.getType() == 'I') {
							check == this->game->edgeCheck();
							switch(check) {
								case -1:
									this->game->piece.moveRight();
									break;
								case 1:
									this->game->piece.moveLeft();
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

GameStateMain::GameStateMain(Game* game)
{
	this->game = game;
	game->window.setTitle("Tetris");
	sf::Vector2f pos = sf::Vector2f(game->window.getSize());
	gameView.setSize(pos);
	pos *= 0.5f;
	gameView.setCenter(pos);
}
