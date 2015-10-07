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

#include "text.hpp"

Text::Text()
{
	this->window.create(sf::VideoMode(800,600), "Tetris via SFML");
	this->window.setFramerateLimit(60);
	lr_increment = 5;
	level = 20;
	initializeFont();
	initializeLeftPanel();
	initializeRightPanel();
	this->lines_remain = this->lr_increment * this->level + this->level;
	this->lr_text.setString(this->formatString(this->lines_remain));
}

void Text::initializeFont()
{
	this->font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSerif-Bold.ttf");
}

void Text::initializeLeftPanel()
{
	//Left Panel Variables
	int lp_x = 100;
	int lp_y = 75;
	int lp_space = 10;
	int lp_gap = 20;
	int lp_size = 25;
	int lvl_size = 35;
	sf::Color lp_color = sf::Color::Black;
	
	this->lvl_label.setFont(font);
	this->lvl_label.setCharacterSize(lvl_size);
	this->lvl_label.setColor(lp_color);
	this->lvl_label.setPosition(sf::Vector2f(lp_x, lp_y));
	this->lvl_label.setString("Level ");
	
	this->lvl_text.setFont(font);
	this->lvl_text.setCharacterSize(lvl_size);
	this->lvl_text.setColor(lp_color);
	this->lvl_text.setPosition(sf::Vector2f(lp_x + 100, lp_y));
	this->lvl_text.setString(this->toString(this->level));
	
	lp_y += (lvl_size + lp_gap);
	
	this->score_label.setFont(font);
	this->score_label.setCharacterSize(lp_size);
	this->score_label.setColor(lp_color);
	this->score_label.setPosition(sf::Vector2f(lp_x,lp_y));
	this->score_label.setString("Score:");
	
	// Increase lp_y by size of score_label characters plus label-field spacer.
	lp_y += (lp_size + lp_space);
	
	this->score_text.setFont(font);
	this->score_text.setCharacterSize(lp_size);
	this->score_text.setColor(lp_color);
	this->score_text.setPosition(sf::Vector2f(lp_x, lp_y));
	this->score_text.setString("00000000");
	
	// Increase lp_y by size of score characters plus field gap.
	lp_y += (lp_size + lp_gap);
	
	this->lc_label.setFont(font);
	this->lc_label.setCharacterSize(lp_size);
	this->lc_label.setColor(lp_color);
	this->lc_label.setPosition(sf::Vector2f(lp_x, lp_y));
	this->lc_label.setString("Lines Clear");
	
	lp_y += (lp_size + lp_space);
	
	this->lc_text.setFont(font);
	this->lc_text.setCharacterSize(lp_size);
	this->lc_text.setColor(lp_color);
	this->lc_text.setPosition(sf::Vector2f(lp_x, lp_y));
	this->lc_text.setString("00000000");
	
	lp_y += (lp_size + lp_gap);
	
	this->lr_label.setFont(font);
	this->lr_label.setCharacterSize(lp_size);
	this->lr_label.setColor(lp_color);
	this->lr_label.setPosition(sf::Vector2f(lp_x, lp_y));
	this->lr_label.setString("Lines Remain");
	
	lp_y += (lp_size + lp_space);
	
	this->lr_text.setFont(font);
	this->lr_text.setCharacterSize(lp_size);
	this->lr_text.setColor(lp_color);
	this->lr_text.setPosition(sf::Vector2f(lp_x, lp_y));
	this->lr_text.setString("00000000");
}

void Text::initializeRightPanel()
{
	int rp_x = 600;
	int rp_y = 75;
	int rp_size = 25;
	sf::Color rp_color = sf::Color::Black;
	
	this->next_label.setFont(font);
	this->next_label.setCharacterSize(rp_size);
	this->next_label.setColor(rp_color);
	this->next_label.setPosition(sf::Vector2f(rp_x, rp_y));
	this->next_label.setString("Next Piece:");
}

std::string Text::toString(int convert)
{
	std::stringstream ss;
	ss << convert;
	return ss.str();
}

std::string Text::formatString(int convert)
{
	std::string new_string = this->toString(convert);
	
	if (convert < 10) {
		new_string = "0000000" + new_string;
	} else if (convert < 100) {
		new_string = "000000" + new_string;
	} else if (convert < 1000) {
		new_string = "00000" + new_string;
	} else if (convert < 10000) {
		new_string = "0000" + new_string;
	} else if (convert < 100000) {
		new_string = "000" + new_string;
	} else if (convert < 1000000) {
		new_string = "00" + new_string;
	} else if (convert < 10000000) {
		new_string = "0" + new_string;
	}
	
	return new_string;
}
	

bool Text::update(int lines)
{
	this->lines_clear += lines;
	this->lines_remain -= lines;
	//set score modifier
	int modifier = 0;
	switch(lines)
	{
		case 2:
			modifier = 10;
			break;
		case 3:
			modifier = 20;
			break;
		case 4:
			modifier = 40;
			break;
	}
	this->score += lines * 10 + modifier;
	
	this->score_text.setString(this->formatString(this->score));
	this->lc_text.setString(this->formatString(this->lines_clear));
	this->lr_text.setString(this->formatString(this->lines_remain));
	
	if (this->lines_remain <= 0) {
		this->level += 1;
		this->lvl_text.setString(toString(this->level));
		
		this->lines_remain = this->lr_increment * this->level + this->level;
		this->lr_text.setString(this->formatString(this->lines_remain));
		
		return true;
	}
	
	return false;
}
