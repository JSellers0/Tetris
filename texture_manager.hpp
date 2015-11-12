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

#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class TextureManager
{
	private:
	
	/* Array of textures used */
	std::map<std::string, sf::Texture>textures;
	
	/* Font Object */
	sf::Font font;
	
	public:
	
	/* Add a texture from a file */
	void loadTexture(const std::string& name, const std::string& filename);
	
	/* Add a font from a file */
	void loadFont(const std::string& filename);
	
	/* Translate an id into a reference */
	sf::Texture& getRef(const std::string& texture);
	
	/* Constructor */
	TextureManager()
	{
	}
	
};

#endif
