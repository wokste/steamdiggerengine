/*
The MIT License (MIT)

Copyright (c) 2013, Steven Wokke

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "font.h"
#include "texture.h"
#include "../game.h"

FontChar::FontChar(pugi::xml_node& node){
	x = node.attribute("x").as_int();
	y = node.attribute("y").as_int();
	width = node.attribute("width").as_int();
	xOffset = node.attribute("xoffset").as_int();
}

FontChar::~FontChar(){}

Font::Font(){
	height  = 0;
	yOffset = 0;
	firstID = 0;
	lastID  = -1;
}

Font::~Font(){
}

void Font::load(pugi::xml_node& node){
	const std::string textureName = node.attribute("texture").value();
	texture.reset(new Texture(GameGlobals::fileSystem.fullpath(textureName)));

	for (auto childNode: node.children())
    {
        charsData.emplace_back(FontChar(childNode));
    }

	height  = node.attribute("height").as_int();
	yOffset = node.attribute("yoffset").as_int();
	firstID =node.attribute("start").as_int();
	lastID  = firstID + charsData.size() - 1;
}

void Font::draw(const std::string& text, Rect4i area) const{
	Vector2i pos(area.left,area.top);

	for (auto& ascii : text){
		if (ascii < firstID || ascii > lastID){
			// TODO: Add an assert
			continue;
		}
		auto charData = charsData[ascii - firstID];

		texture->draw(Vector2i(charData.x,charData.y),Vector2i(charData.width,height), pos);

		pos.x += charData.xOffset;
	}
}
