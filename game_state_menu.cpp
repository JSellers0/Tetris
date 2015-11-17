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
 #include <utility>
 #include <iostream>
 
 #include "game_state_menu.hpp"
 #include "game_state_main.hpp"
 #include "game_state.hpp"
 #include "gui.hpp"
 
 void GameStateMenu::loadGame()
 {
	 this->game->pushState(new GameStateMain(this->game));
	 
	 return;
}
 
 void GameStateMenu::draw(const float dt)
 {
	 this->game->window.setView(this->menuView);
	 
	 this->game->window.clear(sf::Color::Black);
	 this->game->window.draw(this->game->background);
	 
	 for(auto gui : this->guiSystem) this->game->window.draw(gui.second);
	 
	 this->game->window.display();
	 
	 return;
}

void GameStateMenu::update(const float dt)
{
	
}

void GameStateMenu::handleInput()
{
	sf::Event event;
	
	sf::Vector2f mousePos = this->game->window.mapPixelToCoords(
		sf::Mouse::getPosition(this->game->window), this->menuView);
	
	while(this->game->window.pollEvent(event))
	{
		switch(event.type)
		{
			/* Close the window */
			case sf::Event::Closed:
			{
				game->window.close();
				break;
			}
			/* Resize the window */
			/* This will take some work to integrate into existing structures */
			case sf::Event::Resized:
			{
				this->menuView.setSize(event.size.width, event.size.height);
				this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0,0), this->menuView));
				this->game->background.setScale(
					float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
					float(event.size.height)/ float(this->game->background.getTexture()->getSize().y));
				break;
			}
			/* Highlight menu items. */
			case sf::Event::MouseMoved:
			{
				this->guiSystem.at("menu").highlight(this->guiSystem.at("menu").getEntry(mousePos));
				break;
			}
			
			/* Click on menu items. */
			case sf::Event::MouseButtonPressed:
			{
				if(event.mouseButton.button == sf::Mouse::Left)
				{
					std::string msg = this->guiSystem.at("menu").activate(mousePos);
					
					if(msg == "load_game")
					{
						this->loadGame();
					}
				}
				break;
			}
			
			/* Key Events */
			case sf::Event::KeyPressed:
			{
				if(event.key.code == sf::Keyboard::Escape) this->game->window.close();
				else if(event.key.code == sf::Keyboard::Space) this->loadGame();
				break;
			}
			default: break;
			
		}
	}
	
	return;
}

GameStateMenu::GameStateMenu(Game* game)
{
	this->game = game;
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	this->menuView.setSize(pos);
	pos *= 0.5f;
	this->menuView.setCenter(pos);
	
	Gui gui = Gui(sf::Vector2f(192,32), 4, false, game->stylesheets.at("button"), {std::make_pair("Load Game", "load_game")} );
	this->guiSystem.insert(std::make_pair("menu", gui));
			
	this->guiSystem.at("menu").setPosition(pos);
	this->guiSystem.at("menu").setOrigin(96,32*1/2);
	this->guiSystem.at("menu").show();
	
	while(game->window.isOpen())
	{
		handleInput();
		draw(0);
	}
	
}
