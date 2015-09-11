#ifndef TEXT_HPP
#define TEXT_HPP

#include <sstream>
#include <SFML/Graphics.hpp>

class Text {
	
	private:
	
		std::string formatString(int convert);
		int lr_increment;
	
	public:
		Text();
		
		std::string toString(int convert);
		sf::Font font;
		void initializeFont();
		
		//Left Panel
		sf::Text score_label;
		sf::Text score_text;
		int score;
		
		sf::Text lc_label;
		sf::Text lc_text;
		int lines_clear;
		
		
		sf::Text lr_label;
		sf::Text lr_text;
		int lines_remain;
		
		//Right Panel
		sf::Text next_label;
		
		//Level Panel
		sf::Text lvl_label;
		sf::Text lvl_text;
		int level;
		
		//Initialize
		void initializeLeftPanel();
		void initializeRightPanel();

			
		bool update(int lines);
		
		//Get & Set
		void setLevelMetrics(int level);
		int getLevel() {return level;}	
};

#endif

